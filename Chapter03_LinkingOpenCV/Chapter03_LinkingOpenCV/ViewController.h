/*****************************************************************************
 *   ViewController.h
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 5th May 2013
 ******************************************************************************
 *   Chapter 3 of the "OpenCV for iOS" book
 *
 *   Linking OpenCV to iOS Project explains how to link OpenCV library
 *   and call any function from it.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController {
    cv::Mat cvImage;
}

@property (nonatomic, weak) IBOutlet UIImageView* imageView;

@end
