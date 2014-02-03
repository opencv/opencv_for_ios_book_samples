/*****************************************************************************
 *   PostcardPrinter.hpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#pragma once

#include "opencv2/core/core.hpp"

class PostcardPrinter
{
public:
    struct Parameters
    {
        cv::Mat face;
        cv::Mat texture;
        cv::Mat text;
    };
    
    PostcardPrinter(Parameters& parameters);
    virtual ~PostcardPrinter() {}
    
    void print(cv::Mat& postcard) const;
    void preprocessFace();
    
protected:
    void markup();
    void crumple(cv::Mat& image, const cv::Mat& texture,
                 const cv::Mat& mask = cv::Mat()) const;
    void printFragment(cv::Mat& placeForFragment,
                       const cv::Mat& fragment) const;
    void alphaBlendC3(const cv::Mat& src, cv::Mat& dst,
                      const cv::Mat& alpha) const;
    
    Parameters params_;
    cv::Rect faceRoi_;
    cv::Rect textRoi_;
};
