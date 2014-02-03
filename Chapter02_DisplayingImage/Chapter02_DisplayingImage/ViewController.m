/*****************************************************************************
 *   ViewController.m
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

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize imageView;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Read the image
    image = [UIImage imageNamed:@"lena.png"];
    if (image != nil)
        imageView.image = image;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
