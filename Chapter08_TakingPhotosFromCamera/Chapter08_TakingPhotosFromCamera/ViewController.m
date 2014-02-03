/*****************************************************************************
 *   ViewController.m
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ******************************************************************************
 *   Chapter 8 of the "OpenCV for iOS" book
 *
 *   Taking Photos From Camera shows how to capture static images
 *   with camera.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import "ViewController.h"
#import "opencv2/highgui/ios.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize imageView;
@synthesize toolbar;
@synthesize photoCamera;
@synthesize takePhotoButton;
@synthesize startCaptureButton;

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Initialize camera
    photoCamera = [[CvPhotoCamera alloc]
                        initWithParentView:imageView];
    photoCamera.delegate = self;
    photoCamera.defaultAVCaptureDevicePosition =
                        AVCaptureDevicePositionFront;
    photoCamera.defaultAVCaptureSessionPreset =
                        AVCaptureSessionPresetPhoto;
    photoCamera.defaultAVCaptureVideoOrientation =
                        AVCaptureVideoOrientationPortrait;
    
    // Load images
    UIImage* resImage = [UIImage imageNamed:@"scratches.png"];
    UIImageToMat(resImage, params.scratches);
    
    resImage = [UIImage imageNamed:@"fuzzy_border.png"];
    UIImageToMat(resImage, params.fuzzyBorder);
    
    [takePhotoButton setEnabled:NO];
}

- (NSInteger)supportedInterfaceOrientations
{
    // Only portrait orientation
    return UIInterfaceOrientationMaskPortrait;
}

-(IBAction)takePhotoButtonPressed:(id)sender;
{
    [photoCamera takePicture];
}

-(IBAction)startCaptureButtonPressed:(id)sender;
{
    [photoCamera start];
    [self.view addSubview:imageView];
    [takePhotoButton setEnabled:YES];
    [startCaptureButton setEnabled:NO];
}

- (UIImage*)applyEffect:(UIImage*)image;
{
    cv::Mat frame;
    UIImageToMat(image, frame);
    
    params.frameSize = frame.size();
    RetroFilter retroFilter(params);
    
    cv::Mat finalFrame;
    retroFilter.applyToPhoto(frame, finalFrame);
    
    UIImage* result = MatToUIImage(finalFrame);
    return [UIImage imageWithCGImage:[result CGImage]
                               scale:1.0
                         orientation:UIImageOrientationLeftMirrored];
}

- (void)photoCamera:(CvPhotoCamera*)camera
                    capturedImage:(UIImage *)image;
{
    [camera stop];
    resultView = [[UIImageView alloc]
                  initWithFrame:imageView.bounds];
    
    UIImage* result = [self applyEffect:image];
   
    [resultView setImage:result];
    [self.view addSubview:resultView];
    
    [takePhotoButton setEnabled:NO];
    [startCaptureButton setEnabled:YES];
}

- (void)photoCameraCancel:(CvPhotoCamera*)camera;
{
}

- (void)viewDidDisappear:(BOOL)animated
{
    [photoCamera stop];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    photoCamera.delegate = nil;
}
@end
