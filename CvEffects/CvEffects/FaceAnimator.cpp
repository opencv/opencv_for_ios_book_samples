/*****************************************************************************
 *   FaceAnimator.cpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#include "FaceAnimator.hpp"
#include "Processing.hpp"

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

FaceAnimator::FaceAnimator(Parameters parameters)
{
    parameters_ = parameters;

    ExtractAlpha(parameters_.glasses, maskOrig_);
    ExtractAlpha(parameters_.mustache, maskMust_);
}

void FaceAnimator::putImage(Mat& frame, const Mat& image,
                            const Mat& alpha, Rect face,
                            Rect feature, float shift)
{
    // Scale animation image
    float scale = 1.1;
    Size size;
    size.width = scale * feature.width;
    size.height = scale * feature.height;
    Size newSz = Size(size.width,
                      float(image.rows) / image.cols * size.width);
    Mat glasses;
    Mat mask;
    resize(image, glasses, newSz);
    resize(alpha, mask, newSz);

    // Find place for animation
    float coeff = (scale - 1.) / 2.;
    Point origin(face.x + feature.x - coeff * feature.width,
                 face.y + feature.y - coeff * feature.height +
                 newSz.height * shift);
    Rect roi(origin, newSz);
    Mat roi4glass = frame(roi);
    
    alphaBlendC4(glasses, roi4glass, mask);
}

static bool FaceSizeComparer(const Rect& r1, const Rect& r2)
{
    return r1.area() > r2.area();
}

void FaceAnimator::PreprocessToGray(Mat& frame)
{
    cvtColor(frame, grayFrame_, CV_RGBA2GRAY);
    equalizeHist(grayFrame_, grayFrame_);
}

void FaceAnimator::PreprocessToGray_optimized(Mat& frame)
{
    grayFrame_.create(frame.size(), CV_8UC1);
    accBuffer1_.create(frame.size(), frame.type());
    accBuffer2_.create(frame.size(), CV_8UC1);
        
    cvtColor_Accelerate(frame, grayFrame_, accBuffer1_, accBuffer2_);
    equalizeHist_Accelerate(grayFrame_, grayFrame_);
}

void FaceAnimator::detectAndAnimateFaces(Mat& frame)
{
    TS(Preprocessing);
    //PreprocessToGray(frame);
    PreprocessToGray_optimized(frame);
    TE(Preprocessing);
    
    // Detect faces
    TS(DetectFaces);
    std::vector<Rect> faces;
    parameters_.faceCascade.detectMultiScale(grayFrame_, faces, 1.1,
                                              2, 0, Size(100, 100));
    TE(DetectFaces);
    printf("Detected %lu faces\n", faces.size());

    // Sort faces by size in descending order
    sort(faces.begin(), faces.end(), FaceSizeComparer);

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Mat faceROI = grayFrame_( faces[i] );

        std::vector<Rect> facialFeature;
        if (i % 2 == 0)
        {// Detect eyes
            Point origin(0, faces[i].height/4);
            Mat eyesArea = faceROI(Rect(origin,
                        Size(faces[i].width, faces[i].height/4)));

            TS(DetectEyes);
            parameters_.eyesCascade.detectMultiScale(eyesArea,
                facialFeature, 1.1, 2, CV_HAAR_FIND_BIGGEST_OBJECT,
                Size(faces[i].width * 0.55, faces[i].height * 0.13));
            TE(DetectEyes);
            
            if (facialFeature.size())
            {
                TS(DrawGlasses);
                putImage(frame, parameters_.glasses, maskOrig_,
                         faces[i], facialFeature[0] + origin, -0.1f);
                TE(DrawGlasses);
            }
        }
        else
        {// Detect mouth
            Point origin(0, faces[i].height/2);
            Mat mouthArea = faceROI(Rect(origin,
                Size(faces[i].width, faces[i].height/2)));

            parameters_.mouthCascade.detectMultiScale(
                mouthArea, facialFeature, 1.1, 2,
                CV_HAAR_FIND_BIGGEST_OBJECT,
                Size(faces[i].width * 0.2, faces[i].height * 0.13) );
            
            if (facialFeature.size())
            {
                putImage(frame, parameters_.mustache, maskMust_,
                         faces[i], facialFeature[0] + origin, 0.3f);
            }
        }
    }
}
