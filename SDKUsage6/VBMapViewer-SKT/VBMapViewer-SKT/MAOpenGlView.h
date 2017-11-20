//
//  MAOpenGlView.h
//  VbNavKit-iOS-iiAC
//
//  Created by 이주현 on 12. 11. 26..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol GLViewDelegate2;
@class GLView;

@interface MAOpenGlView : UIView
- (id)initWithFrame:(CGRect)frame withDelegate2:(id<GLViewDelegate2>)delegate2;
-(GLView*)GLView;
-(void)uninit;
@end
