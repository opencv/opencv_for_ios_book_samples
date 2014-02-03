/*****************************************************************************
 *   ViewController.m
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

#import "ViewController.h"
#import "opencv2/highgui/ios.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize imageView;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UIImage* image = [UIImage imageNamed:@"lena.png"];

    // Convert UIImage* to cv::Mat
    UIImageToMat(image, cvImage);
    
    if (0)
    {
        
        NSString* filePath = [[NSBundle mainBundle]
                              pathForResource:@"lena" ofType:@"png"];
        // Create file handle
        NSFileHandle* handle =
            [NSFileHandle fileHandleForReadingAtPath:filePath];
        // Read content of the file
        NSData* data = [handle readDataToEndOfFile];
        // Decode image from the data buffer
        cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC1,
                               (void*)data.bytes),
                               CV_LOAD_IMAGE_UNCHANGED);
    }
    
    if (0)
    {
        NSData* data = UIImagePNGRepresentation(image);
        // Decode image from the data buffer
        cvImage = cv::imdecode(cv::Mat(1, [data length], CV_8UC1,
                                       (void*)data.bytes),
                               CV_LOAD_IMAGE_UNCHANGED);
    }
    
    if (!cvImage.empty())
    {
        cv::Mat gray;
        // Convert the image to grayscale
        cv::cvtColor(cvImage, gray, CV_RGBA2GRAY);
        // Apply Gaussian filter to remove small edges
        cv::GaussianBlur(gray, gray,
                         cv::Size(5, 5), 1.2, 1.2);
        // Calculate edges with Canny
        cv::Mat edges;
        cv::Canny(gray, edges, 0, 50);
        // Fill image with white color
        cvImage.setTo(cv::Scalar::all(255));
        // Change color on edges
        cvImage.setTo(cv::Scalar(0, 128, 255, 255), edges);
        // Convert cv::Mat to UIImage* and show the resulting image
        imageView.image = MatToUIImage(cvImage);
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
