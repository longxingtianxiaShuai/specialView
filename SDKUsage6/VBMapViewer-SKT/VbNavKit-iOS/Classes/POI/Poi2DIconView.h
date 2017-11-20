//
//  Poi2DIcon.h
//  PISAiOS
//
//  Created by daejin lee on 11. 10. 27..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol PoiTouchDelegate;

@interface Poi2DIconView : UIImageView {
    
    id <PoiTouchDelegate> delegate;
    
    CGPoint mptLocation;
    
    char *mSzID;
    
    char *mSzName;
    
    int mIdx; // index
    int mId; // POI id
    int poiId;
}

@property (nonatomic, assign) id <PoiTouchDelegate> delegate;
@property (nonatomic, assign) int poiId;

-(void)SetIdx:(int)nIdx;
-(int)GetIdx;

-(void)SetIdName:(const char*)szID name:(const char *)szName;

-(const char *)GetText;

@end



@protocol PoiTouchDelegate


-(void)fireTouch_fromPoi2DIconView : (int)nIdx shopID:(char *)szShopID name:(char *)szName;

@end


