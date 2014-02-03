/*****************************************************************************
 *   ViewController.m
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

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize imageView;
@synthesize startCaptureButton;
@synthesize toolbar;
@synthesize videoCamera;
@synthesize lockFocusButton;
@synthesize lockExposureButton;
@synthesize lockBalanceButton;
@synthesize rotationButton;

- (void)viewDidLoad
{
    [super viewDidLoad];

    videoCamera = [[CvVideoCamera alloc]
                        initWithParentView:imageView];
    videoCamera.delegate = self;
    videoCamera.defaultAVCaptureDevicePosition =
                                AVCaptureDevicePositionBack;
    videoCamera.defaultAVCaptureSessionPreset =
                                AVCaptureSessionPreset640x480;
    videoCamera.defaultAVCaptureVideoOrientation =
                                AVCaptureVideoOrientationPortrait;
    videoCamera.defaultFPS = 30;
    
    isCapturing = NO;
    
    [lockFocusButton setEnabled:NO];
    [lockExposureButton setEnabled:NO];
    [lockBalanceButton setEnabled:NO];
    
    isFocusLocked = NO;
    isExposureLocked = NO;
    isBalanceLocked = NO;
}

- (NSInteger)supportedInterfaceOrientations
{
    // Only portrait orientation
    return UIInterfaceOrientationMaskPortrait;
}

-(IBAction)startCaptureButtonPressed:(id)sender
{
    [videoCamera start];
    isCapturing = YES;
    
    [lockFocusButton setEnabled:YES];
    [lockExposureButton setEnabled:YES];
    [lockBalanceButton setEnabled:YES];
}

-(IBAction)stopCaptureButtonPressed:(id)sender
{
    [videoCamera stop];
    isCapturing = NO;
    
    [lockFocusButton setEnabled:NO];
    [lockExposureButton setEnabled:NO];
    [lockBalanceButton setEnabled:NO];
}

- (IBAction)actionLockFocus:(id)sender
{
    if (isFocusLocked)
    {
        [self.videoCamera unlockFocus];
        [lockFocusButton setTitle:@"Lock focus"];
        isFocusLocked = NO;
    }
    else
    {
        [self.videoCamera lockFocus];
        [lockFocusButton setTitle:@"Unlock focus"];
        isFocusLocked = YES;
    }
}

- (IBAction)actionLockExposure:(id)sender
{
    if (isExposureLocked)
    {
        [self.videoCamera unlockExposure];
        [lockExposureButton setTitle:@"Lock exposure"];
        isExposureLocked = NO;
    }
    else
    {
        [self.videoCamera lockExposure];
        [lockExposureButton setTitle:@"Unlock exposure"];
        isExposureLocked = YES;
    }
}

- (IBAction)actionLockBalance:(id)sender
{
    if (isBalanceLocked)
    {
        [self.videoCamera unlockBalance];
        [lockBalanceButton setTitle:@"Lock balance"];
        isBalanceLocked = NO;
    }
    else
    {
        [self.videoCamera lockBalance];
        [lockBalanceButton setTitle:@"Unlock balance"];
        isBalanceLocked = YES;
    }
}


- (IBAction)rotationButtonPressed:(id)sender
{
    videoCamera.rotateVideo = !videoCamera.rotateVideo;
}


- (void)processImage:(cv::Mat&)image
{
    // Do some OpenCV processing with the image
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    if (isCapturing)
    {
        [videoCamera stop];
    }
}

- (void)dealloc
{
    videoCamera.delegate = nil;
}

@end
