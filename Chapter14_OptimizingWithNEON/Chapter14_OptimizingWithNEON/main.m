/*****************************************************************************
 *   main.m
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 19th May 2013
 ******************************************************************************
 *   Chapter 14 of the "OpenCV for iOS" book
 *
 *   Optimizing Performance with ARM NEON explains how to use
 *   SIMD instructions to vectorize you code and improve performance.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>

#import "AppDelegate.h"

int main(int argc, char *argv[])
{
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
