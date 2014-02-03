/*****************************************************************************
 *   ViewController.h
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 11th May 2013
 ******************************************************************************
 *   Chapter 6 of the "OpenCV for iOS" book
 *
 *   Working with Images in Gallery explains how to load and save
 *   images from/to Gallery.
 *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#import <UIKit/UIKit.h>

@interface ViewController :
    UIViewController<UIImagePickerControllerDelegate,
                     UINavigationControllerDelegate,
                     UIPopoverControllerDelegate>
{
        UIPopoverController* popoverController;
        UIImageView* imageView;
        UIImage* postcardImage;
        cv::CascadeClassifier faceDetector;
}

@property (nonatomic, strong) IBOutlet UIImageView* imageView;
@property (nonatomic, strong) IBOutlet UIToolbar* toolbar;
@property (nonatomic, strong) UIPopoverController* popoverController;
@property (nonatomic, weak) IBOutlet UIBarButtonItem* loadButton;
@property (nonatomic, weak) IBOutlet UIBarButtonItem* saveButton;

-(IBAction)loadButtonPressed:(id)sender;
-(IBAction)saveButtonPressed:(id)sender;

- (UIImage*)printPostcard:(UIImage*)image;

@end
