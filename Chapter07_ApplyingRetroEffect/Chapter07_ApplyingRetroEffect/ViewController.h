/*****************************************************************************
 *   ViewController.h
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 15th May 2013
 ******************************************************************************
 *   Chapter 7 of the "OpenCV for iOS" book
 *
 *   Applying a retro effect demonstrates another interesting photo
 *   effect that makes photos look old.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>
#import "RetroFilter.hpp"

@interface ViewController :
    UIViewController<UIImagePickerControllerDelegate,
                     UINavigationControllerDelegate,
                     UIPopoverControllerDelegate>
{
    UIPopoverController* popoverController;
    UIImageView* imageView;
    UIImage* image;
    RetroFilter::Parameters params;
}

@property (nonatomic, strong) IBOutlet UIImageView* imageView;
@property (nonatomic, strong) IBOutlet UIToolbar* toolbar;
@property (nonatomic, strong) UIPopoverController* popoverController;
@property (nonatomic, weak) IBOutlet UIBarButtonItem* loadButton;
@property (nonatomic, weak) IBOutlet UIBarButtonItem* saveButton;

-(IBAction)loadButtonPressed:(id)sender;
-(IBAction)saveButtonPressed:(id)sender;

- (UIImage*)applyFilter:(UIImage*)image;

@end
