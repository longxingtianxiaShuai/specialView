//
//  MessageWindow.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 7..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MessageWindow.h"
#import "MAPoi.h"

#import <QuartzCore/CALayer.h>

static const float WIDTH=200;
static const float HEIGHT=80;

@implementation MessageWindow {
    UILabel *textLabel;
    MAPoi* maPoi;
}

- (id)init:(NSString*)text
{
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    CGRect frame=CGRectMake((screenSize.width-WIDTH)/2, (screenSize.height-HEIGHT)/2, WIDTH, HEIGHT);
    
    self = [super initWithFrame:frame];
    
    if (self) {
        
		self.userInteractionEnabled = NO;
		self.clipsToBounds = YES;
		self.layer.cornerRadius = 8;
		self.layer.borderWidth = 0.5;
		self.layer.borderColor = [[UIColor blackColor] CGColor];
        self.windowLevel = UIWindowLevelAlert;
        
        NSString *labelString = text;
        
        CGSize textSize = [labelString sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(320, 44) lineBreakMode:NSLineBreakByWordWrapping];
        
        //self.frame=CGRectMake((screenSize.width-textSize.width)/2, (screenSize.height-textSize.height)/2, textSize.width+20, textSize.height+20);
        self.frame=CGRectMake((screenSize.width-textSize.width)/2, screenSize.height-(textSize.height*2)-10, textSize.width+20, textSize.height+20);
        
        UIView* backgroundView=[[UIView alloc]initWithFrame:CGRectMake(0,0,textSize.width+20,textSize.height+20)];
        backgroundView.alpha=0.7;
        backgroundView.backgroundColor=[UIColor blackColor];
        [self addSubview:backgroundView];
        
        textLabel=[[UILabel alloc]initWithFrame:CGRectMake(10,10,textSize.width,textSize.height)];
        textLabel.textAlignment=NSTextAlignmentCenter;
        textLabel.alpha=0.7;
        textLabel.backgroundColor=[UIColor clearColor];
        textLabel.textColor=[UIColor whiteColor];
        textLabel.font=[UIFont systemFontOfSize:14];
        //label.text=NSLocalizedString(@"Loading...",@"Loading...");
        textLabel.text = labelString;
        [self addSubview:textLabel];
    }
    
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

-(void)Show
{
    self.hidden=NO;
    self.alpha = 0;
    
    [UIView transitionWithView:self
                      duration:0.3
                       options:UIViewAnimationOptionCurveEaseOut
                    animations:^{
                        self.alpha = 1.0;
                    }
                    completion:^(BOOL finished){
                        [self performSelector:@selector(Hide) withObject:nil afterDelay:2.0];
                    }];
    
}

-(void)Hide
{
    
    [UIView transitionWithView:self
                      duration:0.3
                       options:UIViewAnimationOptionCurveEaseIn
                    animations:^{
                        self.alpha = 0;
                    }
                    completion:^(BOOL finished){
                        self.hidden=YES;
                        [self removeFromSuperview];
                    }];
}

@end
