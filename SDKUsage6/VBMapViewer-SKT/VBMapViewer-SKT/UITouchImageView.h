//
//  UITouchImageView.h
//  VBMapViewer-SKT
//
//  Created by 병희 이 on 2015. 3. 17..
//  Copyright (c) 2015년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol ImageViewToucDelegate <NSObject>

- (void) didTouchImageView;

@end

@interface UITouchImageView : UIImageView{
	BOOL showingPOIInfo;
}

@property (nonatomic, assign) id<ImageViewToucDelegate> delegate;

@end
