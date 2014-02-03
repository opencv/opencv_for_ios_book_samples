/*****************************************************************************
 *   ViewController.m
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ******************************************************************************
 *   Chapter 9 of the "OpenCV for iOS" book
 *
 *   Creating Static Library explains how to create
 *   a Static Library project in Xcode.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/


#import "ViewController.h"
#import "CvEffects/PostcardPrinter.hpp"
#import "opencv2/highgui/ios.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize imageView;

// Macros for time measurements
#if 1
  #define TS(name) int64 t_##name = cv::getTickCount()
  #define TE(name) printf("TIMER_" #name ": %.2fms\n", \
    1000.*((cv::getTickCount() - t_##name) / cv::getTickFrequency()))
#else
  #define TS(name)
  #define TE(name)
#endif

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    PostcardPrinter::Parameters params;
    
    // Load images
    UIImage* image = [UIImage imageNamed:@"lena.jpg"];
    UIImageToMat(image, params.face);
    
    image = [UIImage imageNamed:@"texture.jpg"];
    UIImageToMat(image, params.texture);
    cvtColor(params.texture, params.texture, CV_RGBA2RGB);
    
    image = [UIImage imageNamed:@"text.png"];
    UIImageToMat(image, params.text, true);
    
    // Create PostcardPrinter class
    PostcardPrinter postcardPrinter(params);
    
    cv::Mat postcard;
    TS(PostcardPrinting);
    postcardPrinter.print(postcard);
    TE(PostcardPrinting);
    
    if (!postcard.empty())
        imageView.image = MatToUIImage(postcard);
}

- (NSInteger)supportedInterfaceOrientations
{
    // Only portrait orientation
    return UIInterfaceOrientationMaskPortrait;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
