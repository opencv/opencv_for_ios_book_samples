/*****************************************************************************
 *   ViewController.m
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 5th May 2013
 ******************************************************************************
 *   Chapter 1 of the "OpenCV for iOS" book
 *
 *   Getting started with iOS, helps you to set up your development environment
 *   and run your first "Hello World" iOS application.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Console output
    NSLog(@"Hello, World!");

    // Alert window
    UIAlertView *alert = [UIAlertView alloc];
    alert = [alert initWithTitle:@"Hey there!"
                         message:@"Welcome to OpenCV on iOS \
                                   development community"
                        delegate:nil
               cancelButtonTitle:@"Continue"
               otherButtonTitles:nil];
    
    [alert show];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
