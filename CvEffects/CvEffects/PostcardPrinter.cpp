/*****************************************************************************
 *   PostcardPrinter.cpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#include "PostcardPrinter.hpp"

#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

PostcardPrinter::PostcardPrinter(Parameters& params)
{
    params_ = params;
    
    markup();
}

void PostcardPrinter::markup()
{
    // Prepare postcard
    int unit = 320;
    cv::Size postcardSize = cv::Size(2 * unit, 3 * unit);
    int border = unit / 8;
    
    // Resize all input images properly
    resize(params_.texture, params_.texture, postcardSize);
    resize(params_.face, params_.face,
           cv::Size(postcardSize.width - 2 * border,
                    postcardSize.width - 2 * border));
    cv::Size newTextSz;
    newTextSz.width = params_.face.cols * 0.9;
    newTextSz.height =
        newTextSz.width * params_.text.rows / params_.text.cols;
    resize(params_.text, params_.text, newTextSz);
    
    // Choose places for face and text
    cv::Point shift(border, 2 * border);
    faceRoi_ = cv::Rect(shift, params_.face.size());
    
    cv::Point origin(border + params_.face.cols * 0.05,
                     params_.face.rows + 4.5 * border);
    textRoi_ = cv::Rect(origin, params_.text.size());
}

void PostcardPrinter::printFragment(Mat& placeForFragment,
                                    const Mat& fragment) const
{
    // Get alpha channel
    vector<Mat> fragmentPlanes;
    split(fragment, fragmentPlanes);
    CV_Assert(fragmentPlanes.size() == 4);
    Mat alpha = fragmentPlanes[3];
    fragmentPlanes.pop_back();
    Mat bgrFragment;
    merge(fragmentPlanes, bgrFragment);
    
    // Add fragment with crumpling and alpha
    crumple(bgrFragment, placeForFragment, alpha);
    alphaBlendC3(bgrFragment, placeForFragment, alpha);
}

void PostcardPrinter::print(Mat& postcard) const
{
    postcard = params_.texture.clone();
    
    Mat placeForFace = postcard(faceRoi_);
    Mat placeForText = postcard(textRoi_);
    
    printFragment(placeForFace, params_.face);
    printFragment(placeForText, params_.text);
}

void PostcardPrinter::crumple(Mat& image, const Mat& texture,
                              const Mat& mask) const
{
    Mat relief;
    cvtColor(texture, relief, CV_BGR2GRAY);
    relief = 255 - relief;
    
    Mat hsvImage;
    vector<Mat> planes;
    cvtColor(image, hsvImage, CV_BGR2HSV);
    
    split(hsvImage, planes);
    CV_Assert(planes.size() == 3);
    subtract(planes[2], relief, planes[2], mask);
    merge(planes, hsvImage);
    
    cvtColor(hsvImage, image, CV_HSV2BGR);
}

void PostcardPrinter::alphaBlendC3(const Mat& src, Mat& dst,
                                   const Mat& alpha) const
{
    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
        {
            uchar alpha_value = alpha.at<uchar>(i, j);
            if (alpha_value != 0)
            {
                float weight = float(alpha_value) / 255.f;
                dst.at<Vec3b>(i, j) = weight * src.at<Vec3b>(i, j) +
                (1 - weight) * dst.at<Vec3b>(i, j);
            }
        }
}

// This function is from the Packt's "Mastering OpenCV" book, Ch 8.
// Here is the repository: https://github.com/MasteringOpenCV/code
void equalizeLeftAndRightHalves(Mat &faceImg)
{
    // It is common that there is stronger light from one half of the face than the other. In that case,
    // if you simply did histogram equalization on the whole face then it would make one half dark and
    // one half bright. So we will do histogram equalization separately on each face half, so they will
    // both look similar on average. But this would cause a sharp edge in the middle of the face, because
    // the left half and right half would be suddenly different. So we also histogram equalize the whole
    // image, and in the middle part we blend the 3 images together for a smooth brightness transition.
    
    int w = faceImg.cols;
    int h = faceImg.rows;
    
    // 1) First, equalize the whole face.
    Mat wholeFace;
    equalizeHist(faceImg, wholeFace);
    
    // 2) Equalize the left half and the right half of the face separately.
    int midX = w/2;
    Mat leftSide = faceImg(cv::Rect(0,0, midX,h));
    Mat rightSide = faceImg(cv::Rect(midX,0, w-midX,h));
    equalizeHist(leftSide, leftSide);
    equalizeHist(rightSide, rightSide);
    
    // 3) Combine the left half and right half and whole face together, so that it has a smooth transition.
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            int v;
            if (x < w/4) { // Left 25%: just use the left face.
                v = leftSide.at<uchar>(y,x);
            }
            else if (x < w*2/4) { // Mid-left 25%: blend the left face & whole face.
                int lv = leftSide.at<uchar>(y,x);
                int wv = wholeFace.at<uchar>(y,x);
                // Blend more of the whole face as it moves further right along the face.
                float f = (x - w*1/4) / (float)(w*0.25f);
                v = cvRound((1.0f - f) * lv + (f) * wv);
            }
            else if (x < w*3/4) { // Mid-right 25%: blend the right face & whole face.
                int rv = rightSide.at<uchar>(y,x-midX);
                int wv = wholeFace.at<uchar>(y,x);
                // Blend more of the right-side face as it moves further right along the face.
                float f = (x - w*2/4) / (float)(w*0.25f);
                v = cvRound((1.0f - f) * wv + (f) * rv);
            }
            else { // Right 25%: just use the right face.
                v = rightSide.at<uchar>(y,x-midX);
            }
            faceImg.at<uchar>(y,x) = v;
        }// end x loop
    }//end y loop
}

void PostcardPrinter::preprocessFace()
{
    Mat grayFace;
    cvtColor(params_.face, grayFace, CV_RGBA2GRAY);
    
    equalizeLeftAndRightHalves(grayFace);
    
    int quant = 64;
    grayFace = grayFace / quant - 0.5;
    grayFace *= quant;
    
    Mat mask;
    threshold(grayFace, mask, 0, 255, THRESH_OTSU|THRESH_BINARY_INV);
    
    grayFace += 100;
    Mat intensity = grayFace.clone();
    intensity = Scalar::all(255);
    grayFace.copyTo(intensity, mask);
    
    vector<Mat> planes;
    planes.resize(3);
    Mat planeH(grayFace.size(), CV_8UC1, Scalar(10));
    Mat planeS(grayFace.size(), CV_8UC1, Scalar(160));
    planes[0] = planeH;
    planes[1] = planeS;
    planes[2] = intensity;
    
    Mat tmpFace;
    merge(planes, tmpFace);
    cvtColor(tmpFace, tmpFace, CV_HSV2RGB);
    
    planes.clear();
    split(tmpFace, planes);
    planes.push_back(mask);
    merge(planes, params_.face);
}
