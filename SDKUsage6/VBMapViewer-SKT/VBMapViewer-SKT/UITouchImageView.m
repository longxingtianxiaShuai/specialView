//
//  UITouchImageView.m
//  VBMapViewer-SKT
//
//  Created by 병희 이 on 2015. 3. 17..
//  Copyright (c) 2015년 Virtual Builders. All rights reserved.
//

#import "UITouchImageView.h"

@implementation UITouchImageView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

//- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    if ([self.delegate respondsToSelector:@selector(didTouchImageView)]) {
//        [self.delegate didTouchImageView];
//    }
//}
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
	showingPOIInfo = YES;
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
	if (([self.delegate respondsToSelector:@selector(didTouchImageView)]) && (showingPOIInfo == NO)) {
		[self.delegate didTouchImageView];
	}
	showingPOIInfo = NO;
}

@end
