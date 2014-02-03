/*****************************************************************************
 *   ViewController.m
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ******************************************************************************
 *   Chapter 13 of the "OpenCV for iOS" book
 *
 *   Saving Video from Camera explains how to save video stream
 *   to the device with hardware encoding.
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

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Initialize camera
    videoCamera = [[CvVideoCamera alloc]
                        initWithParentView:imageView];
    videoCamera.delegate = self;
    videoCamera.defaultAVCaptureDevicePosition =
                                AVCaptureDevicePositionFront;
    videoCamera.defaultAVCaptureSessionPreset =
                                AVCaptureSessionPreset352x288;
    videoCamera.defaultAVCaptureVideoOrientation =
                                AVCaptureVideoOrientationPortrait;
    videoCamera.defaultFPS = 30;
    videoCamera.recordVideo = YES;
 
    isCapturing = NO;
    
    // Load textures
    UIImage* resImage = [UIImage imageNamed:@"scratches.png"];
    UIImageToMat(resImage, params.scratches);
    
    resImage = [UIImage imageNamed:@"fuzzy_border.png"];
    UIImageToMat(resImage, params.fuzzyBorder);
    
    params.frameSize = cv::Size(352, 288);
    filter = new RetroFilter(params);
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
}

-(IBAction)stopCaptureButtonPressed:(id)sender
{
    [videoCamera stop];
    
    NSString* relativePath = [videoCamera.videoFileURL relativePath];
    UISaveVideoAtPathToSavedPhotosAlbum(relativePath, nil, NULL, NULL);
    
    //Alert window
    UIAlertView *alert = [UIAlertView alloc];
    alert = [alert initWithTitle:@"Status"
                         message:@"Saved to the Gallery!"
                        delegate:nil
               cancelButtonTitle:@"Continue"
               otherButtonTitles:nil];
    [alert show];
    
    isCapturing = NO;
}

- (void)processImage:(cv::Mat&)image
{
    cv::Mat inputFrame = image, finalFrame;
    bool isNeedRotation = image.size() != params.frameSize;
    
    if (isNeedRotation)
        inputFrame = image.t();
    
    filter->applyToVideo(inputFrame, finalFrame);
    
    if (isNeedRotation)
        finalFrame = finalFrame.t();
    
    finalFrame.copyTo(image);
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
