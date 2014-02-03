/*****************************************************************************
 *   RetroFilter.hpp
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

#pragma once

#include "opencv2/core/core.hpp"

void alphaBlendC1(cv::Mat src, cv::Mat dst, cv::Mat alpha);

class RetroFilter
{
public:
    struct Parameters
    {
        cv::Size frameSize;
        cv::Mat fuzzyBorder;
        cv::Mat scratches;
    };
    
    RetroFilter(const Parameters& params);
    virtual ~RetroFilter() {};
    void applyToPhoto(const cv::Mat& frame, cv::Mat& retroFrame);
    void applyToVideo(const cv::Mat& frame, cv::Mat& retroFrame);
    
protected:
    Parameters params_;
    
    cv::RNG rng_;
    float multiplier_;
    
    cv::Mat borderColor_;
    cv::Mat scratchColor_;
    
    std::vector<cv::Mat> sepiaPlanes_;
    cv::Mat sepiaH_;
    cv::Mat sepiaS_;
};
