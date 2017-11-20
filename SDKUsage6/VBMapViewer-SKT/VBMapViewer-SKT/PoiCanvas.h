//
//  PoiCanvas.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 3..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "GLViewDelegate2.h"
#import "PoiIcon.h"
#import "PoiFilter.h"
#import "UIBalloonView.h"
#import "MAManager.h"
#import "MALodInfo.h"

#define CAMERA_MIN_DIST 1000.0      // static const float kCameraMinDist = 1000.0 으로 하면 이상하게 됨...

@class MAVector;
@protocol PoiCanvasDelegate
//- (void)CameraMoveToMyPosition:(IDPFPosition*)myPosition;
@end

@interface PoiCanvas : UIView <GLViewDelegate2, PoiIconDelegate, PoiFilterDelegate, BalloonDelegate>

@property (nonatomic,strong) PoiFilter* poiFilter;
@property (nonatomic,assign) id <PoiCanvasDelegate> delegate;
@property (nonatomic,assign) id touchDelegate;
@property (nonatomic,strong) MALodInfo *lodInfo;

-(void)refresh;
-(void)initPoiList:(NSArray *)list;
-(void)SetCategories:(NSArray *)catList;
-(void)SelectedFloorChanged:(int)floor_id;
-(void)selectPoiIcon:(PoiIcon*)poiIcon withPoi:(MAPoi*)maPoi;
-(void)setDynamicPoi:(NSArray *)pois;
-(NSArray *)listDynamicPoi;
-(void)removeDynamicPoiAll;
-(void)removeDynamicPoiById:(int)Id;
-(void)removeDynamicPoiByArray:(NSArray *)pois;
-(CGPoint)getScreenPoint:(MAVector *)pos;
-(BOOL)containsBalloonItem:(int)poiId;
-(void)showPoi:(BOOL)show;
-(void)showPoiById:(NSArray *)Ids show:(BOOL)show;
-(void)showPoiByInfo:(NSArray *)pois show:(BOOL)show;
-(MAPoi *)getPoiInfoById:(int)poiId;
-(void)loadLodInfo;
- (void)setPoiTextSize:(int)size;
- (void)setPoiTextColor:(UIColor *)color;
- (NSArray *)getPoiCategory;
- (NSArray *)getPoiInfoListByFloorId:(NSString *)floorId;
- (void)changePoiIcon:(int)poiId name:(NSString *)imageName;
- (void)restorePoiIcon:(int)poiId;
- (NSArray *)getPoiInfoList;
- (NSArray *) getPoiInfoListByCategoryId:(int)categoryId;
- (void) setGroundLevel:(float)level;
- (void) showDynamicPoi:(BOOL)show x:(float)x y:(float)y z:(float)z;
@end
