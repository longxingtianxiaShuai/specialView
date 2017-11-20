//
//  ImageBtnView.h
//  PISAiOS
//
//  Created by daejin lee on 11. 11. 17..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol ImageBtnDelegate;


@interface ImageBtnView : UIImageView {
    
    id <ImageBtnDelegate> delegate;
    
    
    int mID;
    
    CGPoint mptLocation;
}

@property (nonatomic, assign) id <ImageBtnDelegate> delegate;

-(void)SetID:(int)nID;

@end


@protocol ImageBtnDelegate

-(void)fireTouch_fromImageBtnView : (int)nID;

@end
