//
//  PoiIcon.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 7..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "PoiIcon.h"
#import "CommonLib.h"
#import "MAStateManager.h"
#import "MAMapView.h"
#import "TouchInfo.h"


@implementation PoiIcon
{
    UILabel *labelText;
    UITouchImageView *iv;
    CGSize nameSize;
    
    UIColor *textColor;
    int textSize;
    CGPoint ptTouched;
    int iconPoiSize;
    NSString *targetPath;
}

- (id)initWithFrame:(CGRect)frame andPoi:(MAPoi*)poi andIconSize:(int)iconSize andTextColor:(UIColor *)color andTextSize:(int)size;
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
        self.maPoi = poi;
        textColor = color;
        textSize = size;
        [self CreatePoi:iconSize];
        
        //[self setBackgroundColor:[UIColor redColor]];
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
- (BOOL)isVisibleName
{
    return !labelText.hidden;
    
}


- (void)setTextColor:(UIColor *)color
{
    labelText.textColor=color;
}

- (void)setTextSize:(int)size
{
    labelText.font=[UIFont systemFontOfSize:size];

    nameSize = [self.maPoi.name sizeWithFont:[UIFont systemFontOfSize:size] constrainedToSize:CGSizeMake(320, 44) lineBreakMode:NSLineBreakByWordWrapping];
    self.frame=CGRectMake(self.center.x-nameSize.width/2, self.frame.origin.y, (iv.frame.size.width > nameSize.width) ? iv.frame.size.width : nameSize.width, iv.frame.size.height + nameSize.height);
    if (iv.hidden == NO)
    iv.center = CGPointMake(self.center.x - self.frame.origin.x, iv.center.y);
    labelText.frame = CGRectMake(0,iv.frame.size.height,(iv.frame.size.width > nameSize.width) ? iv.frame.size.width : nameSize.width,nameSize.height);
    
}

- (void) setBoldText
{
    labelText.font = [UIFont boldSystemFontOfSize:textSize];
}

- (void)setIconSize:(int)size
{
    iconPoiSize = size;
    if (labelText.hidden) {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, size, size);
       // if (iv.hidden == NO)
            iv.frame = CGRectMake(0,0,size,size);
    } else {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, nameSize.width, iconPoiSize + nameSize.height);
       // if (iv.hidden == NO)
        iv.frame = CGRectMake(self.center.x - iconPoiSize/2 - self.frame.origin.x,0, size, size);

    }
    
    [self showText:YES];
    //self.frame=CGRectMake(self.frame.origin.x, self.frame.origin.y, (size > nameSize.width) ? size : nameSize.width, size * 2);
    //iv.frame = CGRectMake(self.center.x - size/2 - self.frame.origin.x, 0, size, size);
    //labelText.frame = CGRectMake(0,size,(size > nameSize.width) ? size : nameSize.width,nameSize.height);
}

- (void)showText:(BOOL)show
{
    labelText.hidden = !show;
    if (!show) {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, iv.frame.size.width, iv.frame.size.height);
      //  if (iv.hidden == NO)
        iv.frame = CGRectMake(0,0,iv.frame.size.width,iv.frame.size.height);
    } else {
        self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, nameSize.width, iconPoiSize + nameSize.height);
     //   if (iv.hidden == NO)
        iv.frame = CGRectMake(self.center.x - iconPoiSize/2 - self.frame.origin.x,0, iv.frame.size.width, iv.frame.size.height);
        labelText.frame = CGRectMake(0, iconPoiSize, nameSize.width, nameSize.height);
    }
}

-(void)CreatePoi:(int)iconSize
{
    iconPoiSize = iconSize;
    MAStateManager *stateManager = [MAStateManager Instance];
    
    nameSize = [self.maPoi.name sizeWithFont:[UIFont systemFontOfSize:textSize] constrainedToSize:CGSizeMake(320, 44) lineBreakMode:NSLineBreakByWordWrapping];
    
    self.frame=CGRectMake(self.center.x - nameSize.width/2, self.frame.origin.y, (iconSize > nameSize.width) ? iconSize : nameSize.width, iconSize * 2);
    iv = [[UITouchImageView alloc] initWithFrame:CGRectMake(self.center.x - iconSize/2 - self.frame.origin.x, 0, iconSize, iconSize)];
    iv.delegate = self;
    [iv setUserInteractionEnabled:YES];
    
    if (self.maPoi.image_name == nil) {
        targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/icons/%@",
                                                               [CommonLib InternalIdValueToExternalIdValue:self.maPoi.prj_id type:@"P"], self.maPoi.icon_stored_name]];
    } else {
        targetPath = [[NSBundle mainBundle] pathForResource:self.maPoi.image_name ofType:nil];
    }
    UIImage *iconImage = [UIImage imageWithContentsOfFile:targetPath];
    
    if (self.maPoi.image_name != nil) {
        iconSize = iconImage.size.width;
    }
    iv.image = iconImage;
    //if  (targetPath != nil) {
        [self addSubview:iv];
    //}
    
    if (targetPath == nil) {
        iv.hidden = YES;
    }
    
    
    
    labelText =[[UILabel alloc]initWithFrame:CGRectMake(0,iconSize,(iconSize > nameSize.width) ? iconSize : nameSize.width,nameSize.height)];
    labelText.textAlignment=NSTextAlignmentCenter;
    
    labelText.backgroundColor=[UIColor clearColor];
    labelText.textColor=textColor;
    labelText.font=[UIFont systemFontOfSize:textSize];
    labelText.text = self.maPoi.name;
    [self addSubview:labelText];
 
   // [self setUserInteractionEnabled:YES];
}



#pragma mark - touches

- (void) didTouchImageView
{
    [self.delegate selectPoiIcon:self withPoi:self.maPoi];
    /*[UIView transitionWithView:self
                          duration:0.05
                           options:UIViewAnimationOptionCurveEaseIn
                        animations:^{
                            iv.transform = CGAffineTransformMakeScale(1.5, 1.5);
                        }
                        completion:^(BOOL finished){
     
    }];*/
}
    //}
    
    //[((MAMapView *)[[self superview] superview]) GLView];
    //[[((MAMapView *)[[self superview] superview]) GLView] exTouchesBegan:touches withEvent:event];
    //NSLog(@"poi touch begin");
    /*
    UITouch* touch = [touches anyObject];
    ptTouched  = [touch locationInView:self];
    
    TouchInfo *info = [[TouchInfo alloc] init];
    info.touches = touches;
    info.events = event;
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"touchBegin" object:info];*/
    //}
/*
-  (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [UIView transitionWithView:self
                      duration:0.05
                       options:UIViewAnimationOptionCurveEaseIn
                    animations:^{
                        iv.transform = CGAffineTransformMakeScale(1.0, 1.0);
                    }
                    completion:^(BOOL finished){
                        
                    }];
    
}*/
@end
