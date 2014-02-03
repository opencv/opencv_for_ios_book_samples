/*****************************************************************************
 *   RetroFilter.cpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#include "RetroFilter.hpp"
#include "Processing.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

RetroFilter::RetroFilter(const Parameters& params) : rng_(time(0))
{
    params_ = params;
    
    multiplier_ = 1.0;
    
    borderColor_.create(params_.frameSize, CV_8UC1);
    scratchColor_.create(params_.frameSize, CV_8UC1);
    
    sepiaH_.create(params_.frameSize, CV_8UC1);
    sepiaH_.setTo(Scalar(19));
    sepiaS_.create(params_.frameSize, CV_8UC1);
    sepiaS_.setTo(Scalar(78));
    sepiaPlanes_.resize(3);
    sepiaPlanes_[0] = sepiaH_;
    sepiaPlanes_[1] = sepiaS_;
    
    resize(params_.fuzzyBorder, params_.fuzzyBorder,
           params_.frameSize);
    
    if (params_.scratches.rows < params_.frameSize.height ||
        params_.scratches.cols < params_.frameSize.width)
    {
        resize(params_.scratches, params_.scratches,
               params_.frameSize);
    }
}

void RetroFilter::applyToPhoto(const Mat& frame, Mat& retroFrame)
{
    Mat luminance;
    cvtColor(frame, luminance, CV_BGR2GRAY);
    
    // Add scratches
    Scalar meanColor = mean(luminance.row(luminance.rows / 2));
    scratchColor_.setTo(meanColor * 2.0);
    int x = rng_.uniform(0, params_.scratches.cols - luminance.cols);
    int y = rng_.uniform(0, params_.scratches.rows - luminance.rows);
    cv::Rect roi(cv::Point(x, y), luminance.size());
    scratchColor_.copyTo(luminance, params_.scratches(roi));
    
    // Add fuzzy border
    borderColor_.setTo(meanColor * 1.5);
    alphaBlendC1(borderColor_, luminance, params_.fuzzyBorder);
    
    // Apply sepia-effect
    sepiaPlanes_[2] = luminance + 20;
    Mat hsvFrame;
    merge(sepiaPlanes_, hsvFrame);
    cvtColor(hsvFrame, retroFrame, CV_HSV2RGB);
}

void RetroFilter::applyToVideo(const Mat& frame, Mat& retroFrame)
{
    // Convert to gray with random shift
    cv::Size shift;
    shift.width  = 2 + (rng_.uniform(0,10) ? 0 : rng_.uniform(-1,2));
    shift.height = 2 + (rng_.uniform(0,10) ? 0 : rng_.uniform(-1,2));
    cv::Rect roiSrc(cv::Point(0, 0), frame.size() - shift);
    cv::Rect roiDst(shift, frame.size() - shift);
    
    retroFrame.create(frame.size(), CV_8UC1);
    cvtColor(frame(roiSrc), retroFrame(roiDst), CV_BGR2GRAY);
    
    // Add intensity variation
    float sign = pow(-1.f, rng_.uniform(0, 2));
    float value = 1.f + sign * rng_.gaussian(0.2);
    multiplier_ = 0.7 * multiplier_ + 0.3 * value;
    retroFrame *= multiplier_;
    
    Scalar meanColor = mean(retroFrame.row(retroFrame.rows / 2));
    
    // Add scratches
    int x = rng_.uniform(0,params_.scratches.cols - retroFrame.cols);
    int y = rng_.uniform(0,params_.scratches.rows - retroFrame.rows);
    cv::Rect roi(cv::Point(x, y), retroFrame.size());
    if (rng_.uniform(0, 2))
        scratchColor_.setTo(meanColor * 2.0);
    else
        scratchColor_.setTo(meanColor / 2.0);
    alphaBlendC1(scratchColor_, retroFrame, params_.scratches(roi));
    
    // Add fuzzy border
    borderColor_.setTo(meanColor * 1.5);
    alphaBlendC1(borderColor_, retroFrame, params_.fuzzyBorder);
    
    // Convert back to 3-channel image
    cvtColor(retroFrame, retroFrame, CV_GRAY2BGR);
}

void RetroFilter::applyToVideo_optimized(const Mat& frame,
                                         Mat& retroFrame)
{
    // Convert to gray with random shift
    TS(ConvertingToGray);
    cv::Size shift;
    shift.width  = 2 + (rng_.uniform(0,10) ? 0 : rng_.uniform(-1,2));
    shift.height = 2 + (rng_.uniform(0,10) ? 0 : rng_.uniform(-1,2));
    cv::Rect roiSrc(cv::Point(0, 0), frame.size() - shift);
    cv::Rect roiDst(shift, frame.size() - shift);
    retroFrame.create(frame.size(), CV_8UC1);
    cvtColor(frame(roiSrc), retroFrame(roiDst), CV_BGRA2GRAY);
    TE(ConvertingToGray);
    
    // Add intensity variation
    TS(IntensityVariation);
    float sign = pow(-1.f, rng_.uniform(0, 2));
    float value = 1.f + sign * rng_.gaussian(0.2);
    multiplier_ = 0.7 * multiplier_ + 0.3 * value;
    multiply_NEON(retroFrame, multiplier_);
    TE(IntensityVariation);
    
    Scalar meanColor = mean(retroFrame.row(retroFrame.rows / 2));
    
    // Add scratches
    TS(AddingScratches);
    int x = rng_.uniform(0,params_.scratches.cols - retroFrame.cols);
    int y = rng_.uniform(0,params_.scratches.rows - retroFrame.rows);
    cv::Rect roi(cv::Point(x, y), retroFrame.size());
    if (rng_.uniform(0, 2))
        scratchColor_.setTo(meanColor * 2.0);
    else
        scratchColor_.setTo(meanColor / 2.0);
    //TODO: use NEON here as well, but it is not continuous
    alphaBlendC1(scratchColor_, retroFrame, params_.scratches(roi));
    TE(AddingScratches);
    
    // Add fuzzy border
    TS(FuzzyBorder);
    borderColor_.setTo(meanColor * 1.5);
    alphaBlendC1_NEON(borderColor_, retroFrame, params_.fuzzyBorder);
    TE(FuzzyBorder);
    
    // Convert back to 3-channel image
    TS(ConvertingToBGR);
    cvtColor(retroFrame, retroFrame, CV_GRAY2BGRA);
    TE(ConvertingToBGR);
}
