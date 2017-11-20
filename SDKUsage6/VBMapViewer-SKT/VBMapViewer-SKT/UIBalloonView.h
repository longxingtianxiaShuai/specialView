//
//  UIBalloonWindow.h
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 5. 26..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>
@class BalloonItem;

@protocol BalloonDelegate <NSObject>
- (void)selectBalloon:(BalloonItem *)item;
@end

@interface UIBalloonView : UIView

@property (nonatomic, assign) id <BalloonDelegate> delegate;
@property (nonatomic, strong) BalloonItem *item;

-(id)init:(BalloonItem *)item posX:(int)x posY:(int)y;

@end
