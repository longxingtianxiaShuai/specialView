//
//  FloorSlider.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 18..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "FloorSlider.h"
#import "CommonLib.h"

const float POS_Y = 50.0f;

@implementation FloorSlider {
    UIImageView *sliderHandle;
    BOOL isSliderTouch;
    
    UILabel *floorLabel;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
        isSliderTouch = NO;
        
        [self drawUI];
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

-(void)drawUI
{
    [self addSubview:[CommonLib createUIImgeView:CGRectMake(0, 0, 38, 38) PngImageName:@"floor_001.png"]];
    [self addSubview:[CommonLib createUIImgeView:CGRectMake(0, 37, 38, 181) PngImageName:@"floor_back_001.png"]];
    
    sliderHandle = [CommonLib createUIImgeView:CGRectMake(6, 188, 24, 24) PngImageName:@"floor_btn.png"];
    [self addSubview:sliderHandle];
    
    floorLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 9, 38, 20)];
    floorLabel.backgroundColor = [UIColor clearColor];
    floorLabel.text = @"1F";
    floorLabel.adjustsFontSizeToFitWidth = YES;
    floorLabel.font = [UIFont systemFontOfSize:13];
    floorLabel.textColor = [UIColor whiteColor];
    floorLabel.textAlignment = NSTextAlignmentCenter;
    [self addSubview:floorLabel];
}

#pragma mark - touches
-  (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:self];
    
    // 터치되는 부분 확장
    int nSize = 30;
    
    if ( CGRectContainsPoint(CGRectMake(sliderHandle.frame.origin.x-nSize, sliderHandle.frame.origin.y-nSize, sliderHandle.frame.size.width+(nSize*2), sliderHandle.frame.size.height + (nSize*2)), point) ) {
        isSliderTouch = YES;
        sliderHandle.transform = CGAffineTransformMakeScale(1.5, 1.5);
    }
    else {
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    
    if (isSliderTouch) {
        if (location.y >= POS_Y && location.y <= 200) {
            [sliderHandle setCenter:CGPointMake(sliderHandle.center.x, location.y)];
        }
        else if (location.y <= POS_Y) {
            [sliderHandle setCenter:CGPointMake(sliderHandle.center.x, POS_Y)];
        }
        else if (location.y >= 200) {
            [sliderHandle setCenter:CGPointMake(sliderHandle.center.x, 200)];
        }
        
        float fVal = 0;
        
        // 상단이 1, 하단이 0
        fVal = fabs(1.0 / 150.0 * (sliderHandle.center.y - POS_Y) - 1.0);
        
        if (fVal < 0.01) {
            fVal = 0.01;
        }
        else if (fVal > 1.0) {
            fVal = 1.0;
        }
        
        fVal = 1.f - fVal;  //WONIL - 회전 중심 기준에서 카메라 기준으로 변경. 일단 간단히 요렇게 수정.(2013.2.15)
        
        NSLog(@"fval : %f", fVal);
        
        // 층별처리필요..
        //floorLabel.text = [NSString stringWithFormat:@"%fF",fVal];
        
       // [self.delegate changeSliderValue:fVal];
    }
    else {
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    isSliderTouch = NO;
    sliderHandle.transform = CGAffineTransformMakeScale(1, 1);
    //[sliderHandle setImage:[UIImage imageWithContentsOfFile:[[Resource Instance] IconFilePath:@"camera_btn_normal.png"]]];
}

@end
