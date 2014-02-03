/*****************************************************************************
 *   ViewController.h
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 5th May 2013
 ******************************************************************************
 *   Chapter 4 of the "OpenCV for iOS" book
 *
 *   Detecting Faces with Cascade Classifier shows how to detect faces
 *   using OpenCV.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController {
    cv::CascadeClassifier faceDetector;
}

@property (nonatomic, weak) IBOutlet UIImageView* imageView;

@end
