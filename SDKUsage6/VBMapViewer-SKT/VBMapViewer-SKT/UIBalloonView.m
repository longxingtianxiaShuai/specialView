//
//  UIBalloonWindow.m
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 5. 26..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "UIBalloonView.h"
#import <QuartzCore/CALayer.h>
#import "BalloonItem.h"

static const float WIDTH=200;
static const float HEIGHT=80;

@implementation UIBalloonView {
    UILabel *textLabel;
    
}


- (id)init:(BalloonItem *)item posX:(int)x posY:(int)y
{
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    CGRect frame=CGRectMake((screenSize.width-WIDTH)/2, (screenSize.height-HEIGHT)/2, WIDTH, HEIGHT);
    
    self = [super initWithFrame:frame];
    
    if (self) {
        self.item = item;
		self.clipsToBounds = YES;
		self.layer.cornerRadius = 8;
		self.layer.borderWidth = 0.5;
		self.layer.borderColor = [[UIColor yellowColor] CGColor];
       // self.windowLevel = UIWindowLevelAlert;
        
        NSString *labelString = item.name;
        
        CGSize textSize = [labelString sizeWithFont:[UIFont systemFontOfSize:13] constrainedToSize:CGSizeMake(320, 44) lineBreakMode:NSLineBreakByWordWrapping];
        
        //self.frame=CGRectMake((screenSize.width-textSize.width)/2, (screenSize.height-textSize.height)/2, textSize.width+20, textSize.height+20);
        self.frame=CGRectMake(x - 10 - textSize.width/2, y, textSize.width+20, textSize.height+10);
        
        UIView* backgroundView=[[UIView alloc]initWithFrame:CGRectMake(0,0,textSize.width+20,textSize.height+10)];
        backgroundView.alpha=0.7;
        backgroundView.backgroundColor=[UIColor yellowColor];
        [self addSubview:backgroundView];
        
        textLabel=[[UILabel alloc]initWithFrame:CGRectMake(10,5,textSize.width,textSize.height)];
        textLabel.textAlignment=NSTextAlignmentCenter;
        textLabel.alpha=0.7;
        textLabel.backgroundColor=[UIColor clearColor];
        textLabel.textColor=[UIColor blackColor];
        textLabel.font=[UIFont systemFontOfSize:13];
        //label.text=NSLocalizedString(@"Loading...",@"Loading...");
        textLabel.text = labelString;
        [self addSubview:textLabel];
        
        
        
    }
    
    return self;
}

-  (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [UIView transitionWithView:self
                      duration:0.1
                       options:UIViewAnimationOptionCurveEaseIn
                    animations:^{
                        self.transform = CGAffineTransformMakeScale(1.1, 1.1);
                    }
                    completion:^(BOOL finished){
                        [self.delegate selectBalloon:self.item];
                    }];

    
}

@end
