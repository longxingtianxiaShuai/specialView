//
//  PoiIcon.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 7..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MAPoi.h"
#import "UITouchImageView.h"

@class PoiIcon;
@class MAMapView;

@protocol PoiIconDelegate <NSObject>
- (void)selectPoiIcon:(PoiIcon*)poiIcon withPoi:(MAPoi*)poi;
@end

@interface PoiIcon : UIView <ImageViewToucDelegate>

@property (nonatomic, assign) id <PoiIconDelegate> delegate;
@property (nonatomic, strong) MAPoi *maPoi;
@property (nonatomic, strong) NSString *type;

- (id)initWithFrame:(CGRect)frame andPoi:(MAPoi*)poi andIconSize:(int)iconSize andTextColor:(UIColor *)color andTextSize:(int)size;
- (void)showText:(BOOL)show;
- (void)setIconSize:(int)size;
- (BOOL)isVisibleName;
- (void)setTextSize:(int)size;
- (void)setTextColor:(UIColor *)color;
- (void)setBoldText;

@end
