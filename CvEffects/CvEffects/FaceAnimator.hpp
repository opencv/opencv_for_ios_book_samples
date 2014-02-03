/*****************************************************************************
 *   FaceAnimator.hpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

class FaceAnimator
{
public:
    struct Parameters
    {
        cv::Mat glasses;
        cv::Mat mustache;
        cv::CascadeClassifier faceCascade;
        cv::CascadeClassifier eyesCascade;
        cv::CascadeClassifier mouthCascade;
    };

    FaceAnimator(Parameters params);
    virtual ~FaceAnimator() {};

    void detectAndAnimateFaces(cv::Mat& frame);

protected:
    Parameters parameters_;
    cv::Mat maskOrig_;
    cv::Mat maskMust_;
    cv::Mat grayFrame_;
    
    void putImage(cv::Mat& frame, const cv::Mat& image,
                  const cv::Mat& alpha, cv::Rect face,
                  cv::Rect facialFeature, float shift);
    void PreprocessToGray(cv::Mat& frame);

    // Members needed for optimization with Accelerate Framework
    void PreprocessToGray_optimized(cv::Mat& frame);
    cv::Mat accBuffer1_;
    cv::Mat accBuffer2_;
};
