/*****************************************************************************
 *   RetroFilter.cpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 15th May 2013
 ******************************************************************************
 *   Chapter 7 of the "OpenCV for iOS" book
 *
 *   Applying a retro effect demonstrates another interesting photo
 *   effect that makes photos look old.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#include "RetroFilter.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void alphaBlendC1(Mat src, Mat dst, Mat alpha)
{
    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
        {
            uchar alphaVal = alpha.at<uchar>(i, j);
            if (alphaVal != 0)
            {
                float weight = static_cast<float>(alphaVal) / 255.0f;
                dst.at<uchar>(i, j) = weight * src.at<uchar>(i, j) +
                (1 - weight) * dst.at<uchar>(i, j);
            }
        }
}

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
