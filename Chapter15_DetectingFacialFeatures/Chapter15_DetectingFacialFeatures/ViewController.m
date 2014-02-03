/*****************************************************************************
 *   ViewController.m
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 19th May 2013
 ******************************************************************************
 *   Chapter 15 of the "OpenCV for iOS" book
 *
 *   Detecting Facial Features presents a simple facial feature
 *   detection demo.
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

    self.videoCamera = [[CvVideoCamera alloc]
                        initWithParentView:imageView];
    self.videoCamera.delegate = self;
    self.videoCamera.defaultAVCaptureDevicePosition =
                            AVCaptureDevicePositionFront;
    self.videoCamera.defaultAVCaptureSessionPreset =
                            AVCaptureSessionPreset352x288;
    self.videoCamera.defaultAVCaptureVideoOrientation =
                            AVCaptureVideoOrientationPortrait;
    self.videoCamera.defaultFPS = 30;
 
    isCapturing = NO;
    
    // Load images
    UIImage* resImage = [UIImage imageNamed:@"glasses.png"];
    UIImageToMat(resImage, parameters.glasses, true);
    cvtColor(parameters.glasses, parameters.glasses, CV_BGRA2RGBA);
    
    resImage = [UIImage imageNamed:@"mustache.png"];
    UIImageToMat(resImage, parameters.mustache, true);
    cvtColor(parameters.mustache, parameters.mustache, CV_BGRA2RGBA);
    
    // Load Cascade Classisiers
    NSString* filename = [[NSBundle mainBundle]
                          pathForResource:@"lbpcascade_frontalface"
                                   ofType:@"xml"];
    parameters.faceCascade.load([filename UTF8String]);
    
    filename = [[NSBundle mainBundle]
                pathForResource:@"haarcascade_mcs_eyepair_big"
                         ofType:@"xml"];
    parameters.eyesCascade.load([filename UTF8String]);
    
    filename = [[NSBundle mainBundle]
                pathForResource:@"haarcascade_mcs_mouth"
                         ofType:@"xml"];
    parameters.mouthCascade.load([filename UTF8String]);
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
    
    faceAnimator = new FaceAnimator(parameters);
}

-(IBAction)stopCaptureButtonPressed:(id)sender
{
    [videoCamera stop];
    isCapturing = NO;
}

// Macros for time measurements
#if 1
  #define TS(name) int64 t_##name = cv::getTickCount()
  #define TE(name) printf("TIMER_" #name ": %.2fms\n", \
    1000.*((cv::getTickCount() - t_##name) / cv::getTickFrequency()))
#else
  #define TS(name)
  #define TE(name)
#endif

- (void)processImage:(cv::Mat&)image
{
    TS(DetectAndAnimateFaces);
    faceAnimator->detectAndAnimateFaces(image);
    TE(DetectAndAnimateFaces);
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
