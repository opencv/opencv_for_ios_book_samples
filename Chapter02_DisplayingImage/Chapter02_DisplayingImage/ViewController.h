/*****************************************************************************
 *   ViewController.h
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 5th May 2013
 ******************************************************************************
 *   Chapter 2 of the "OpenCV for iOS" book
 *
 *   Displaying Image from Resources introduces you to basic GUI concepts on iOS, and covers loading of an
 *   image from resources and displaying it on the screen.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController {
    UIImage* image;
}

@property (nonatomic, weak) IBOutlet UIImageView* imageView;

@end
