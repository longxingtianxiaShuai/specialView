//
//  ActivityWindow.m
//  VbNavKit-iOS-iiAC
//
//  Created by 이주현 on 13. 1. 2..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#import "ActivityWindow.h"

#import <QuartzCore/CALayer.h>

@implementation ActivityWindow {
}

- (id)init
{
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    //CGRect frame=CGRectMake((screenSize.width-WIDTH)/2, (screenSize.height-HEIGHT)/2, WIDTH, HEIGHT);
    CGRect frame=CGRectMake(0, 0, screenSize.width, screenSize.height);
    
    self = [super initWithFrame:frame];
    
    if (self) {
        
        UIView* backgroundView=[[UIView alloc]initWithFrame:CGRectMake(0,0,screenSize.width,screenSize.height)];
        backgroundView.backgroundColor = [UIColor grayColor];
        backgroundView.alpha = 0.5f;
        
        [self addSubview:backgroundView];        
        
        UIActivityIndicatorView *activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
        [activityIndicator setFrame:CGRectMake(0, 0, 100, 100)];
        [activityIndicator setCenter:backgroundView.center];
        [activityIndicator setHidesWhenStopped:YES];
        [activityIndicator setBackgroundColor:[UIColor darkGrayColor]];
        //[activityIndicator setAlpha:0.8];
        [activityIndicator.layer setCornerRadius:10];
        [backgroundView addSubview:activityIndicator];
        
        [activityIndicator startAnimating];
        
        self.userInteractionEnabled = NO;
        backgroundView.userInteractionEnabled = NO;
    }
    
    return self;
}

+(ActivityWindow*)Instance
{
    static ActivityWindow* instance;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance=[[ActivityWindow alloc]init];
    });
    
    return instance;
}

+(void)Show
{
    ActivityWindow* view=[self Instance];
    
    // hidden이 NO라면 이미 띄워진 상태이다..
    if (view.hidden == NO) {
        return;
    }
    
    view.hidden=NO;
    view.alpha = 0;
    
    [UIView transitionWithView:view
                      duration:0.3
                       options:UIViewAnimationOptionCurveEaseOut  //justin
                    animations:^{
                        view.alpha = 1.0;
                    }
                    completion:^(BOOL finished){
                        //[self performSelector:@selector(Hide) withObject:nil afterDelay:2.0];
                    }];
    
}

+(void)Hide
{
    ActivityWindow* view=[self Instance];
    
    [UIView transitionWithView:view
                      duration:0.3
                       options:UIViewAnimationOptionCurveEaseOut  // justin
                    animations:^{
                        view.alpha = 0;
                    }
                    completion:^(BOOL finished){
                        view.hidden=YES;
                    }];
    
    
}

@end
