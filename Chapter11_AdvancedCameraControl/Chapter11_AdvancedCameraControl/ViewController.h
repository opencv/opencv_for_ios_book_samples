/*****************************************************************************
 *   ViewController.h
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ******************************************************************************
 *   Chapter 11 of the "OpenCV for iOS" book
 *
 *   Control Advanced Camera Settings explains how to control advanced
 *   camera settings, like exposure, focus and white balance.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>
#import <opencv2/highgui/ios.h>

@interface ViewController : UIViewController<CvVideoCameraDelegate>
{
    CvVideoCamera* videoCamera;
    BOOL isCapturing;
    
    BOOL isFocusLocked, isExposureLocked, isBalanceLocked;
}

@property (nonatomic, strong) CvVideoCamera* videoCamera;
@property (nonatomic, strong) IBOutlet UIImageView* imageView;
@property (nonatomic, strong) IBOutlet UIToolbar* toolbar;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* startCaptureButton;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* stopCaptureButton;

@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* lockFocusButton;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* lockExposureButton;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* lockBalanceButton;
@property (nonatomic, weak) IBOutlet
    UIBarButtonItem* rotationButton;

-(IBAction)startCaptureButtonPressed:(id)sender;
-(IBAction)stopCaptureButtonPressed:(id)sender;

- (IBAction)actionLockFocus:(id)sender;
- (IBAction)actionLockExposure:(id)sender;
- (IBAction)actionLockBalance:(id)sender;

- (IBAction)rotationButtonPressed:(id)sender;

@end
