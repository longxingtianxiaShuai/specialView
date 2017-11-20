//
//  MAManager.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 17..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "MAUpdate.h"

@class MAVector;
@class MAPoi;

static const float kTopView = 90.0;
static const float kBirdView = 45.0;
static const float kWalkingView = 20.0;

typedef enum { eTopView = 0, eBirdView, eWalkingView } cameraMode;
typedef enum { meter = 0, feet = 1 } unit;

@interface MAManager : NSObject <MADelegate>

@property(nonatomic,assign) id<MADelegate> delegate;

- (id)initWithFrame:(CGRect)frame;
- (void)clearMap;
- (UIView*)getMapView;

- (void)setServerUrl:(NSString*)url;
- (void)setProjectId:(int)prjId;
- (void)needUpdate:(int)prjId;
- (void)startUpdate;
- (void)cancelUpdate;
- (BOOL)isExistProjectData:(int)prjId;
- (BOOL)deleteProjectData:(int)prjId;
- (void)onUpdateStateChanged:(MAUpdate*)state;
- (void)onReadyForUpdate;
- (void)onNetworkFailed;
- (void)loadZone;
- (void)loadMap:(int)buildingId floorId:(NSString *)floorId;
- (void)changeFloor:(int)floor;
- (void)changeFloorById:(NSString *)floorId;

- (void)showTexture:(BOOL)show;
- (void)mapMoveTo:(MAVector *)pos;
- (void)mapMoveOnPoiId:(int)poiId;
- (void)mapMoveOnPoiInfo:(MAPoi *)poiInfo;
- (void)showPoi:(BOOL)show;
- (void)showPoiById:(NSArray *)Ids show:(BOOL)show;
- (void)showPoiByInfo:(NSArray *)pois show:(BOOL)show;
- (void)setBackgroundColorWithRed:(float)red green:(float)green blue:(float)blue;
- (void)setDynamicPoi:(NSArray*)pois;
- (void)setShowPoiType:(NSArray *)categoryList;
- (NSArray *)getFloorInfo:(int)bldgId;
- (NSArray *)getBuildingInfo:(int)zoneId;
- (NSArray *)listDynamicPoi;
- (void)removeDynamicPoiById:(int)Id;
- (void)removeDynamicPoiByArray:(NSArray *)pois;
- (void)removeDynamicPoiAll;
- (void)setCameraAngle:(float)angle;
- (float)getCameraAngle;
- (void)setMapRotation:(float)rotation;
- (float)getMapRotation;
- (void)setMapAzimuth:(float)azimuth;
- (float)getMapAzimuth;
- (void)setZoom:(float)zoom;
- (float)getZoom;
- (MAVector *)getMapPointX:(int)x andY:(int)y;
- (CGPoint)getScreenPoint:(MAVector *)pos;
- (void)moveMapToX:(float)x andY:(float)y;
- (BOOL)onPoiTouched:(int)poiId x:(int)x y:(int)y;
- (BOOL)onBalloonTouched:(int)poiId;
- (void)onMapLoadingSuccess;
- (void)onMapLoadingFailed;
- (NSString *)getCurrentZoneId;
- (NSString *)getCurrentBuildingId;
- (NSString *)getCurrentFloorId;
- (void)setDistanceUnit:(unit)value;
- (unit)getDistanceUnit;
- (void)setDataPath:(NSString *)path;
- (NSString *)getDBPath;
- (NSString *)getGmlPath;
- (NSDate *)getLastUpdateDate:(int)prjId;
- (NSString *)getVersion;
- (NSString *)getDefaultZoneId;
- (NSString *)getDefaultBuildingId;
- (NSString *)getDefaultFloorId;
- (void)debugEnable:(BOOL)enable;
- (MAPoi *)getPoiInfoById:(int)poiId;
- (void)addPath:(NSArray *)posList withColor:(UIColor *)color;
- (void)addPath:(NSArray *)posList withColor:(UIColor *)color withFloorId:(int)floorId;
- (void)removePath;
- (void)setPoiTextSize:(int)size;
- (void)setPoiTextColor:(UIColor *)color;
- (NSArray *)getPoiInfoListByFloorId:(NSString *)floorId;
- (NSArray *)getPoiInfoListOfCurrentFloor;
- (void)changePoiIcon:(int)poiId name:(NSString *)imageName;
- (void)restorePoiIcon:(int)poiId;
- (void)setCameraMode:(cameraMode)mode;
- (void)showPath:(BOOL)show;
- (NSArray *)getPoiCategory;
- (void) clearData;
- (NSArray *)getCategoryList:(NSString *)prjId;
- (NSArray *) getPoiInfoList;
- (NSArray *) getPoiInfoListByCategoryId:(int)categoryId;
- (void) loadMapAll:(int)buildingId;
- (void) showMapAll:(BOOL)show;
- (void)setCameraYaw:(float)fYaw PitchRatio:(float)fPitchRatio Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (void)setCameraYaw:(float)fYaw Pitch:(float)fPitch Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (void) setVerticalScale:(float)scale;
- (void) setPathVisibility:(BOOL)show withFloorId:(int)floorId exclusive:(BOOL)exclusive;
- (void) setGroundLevel:(float)groundLevel;
- (void) setHRotationAnimation:(BOOL)play step:(float)step;
- (void) brokeCurrentViewControlPan:(BOOL)panEnable zoom:(BOOL)zoomEnable rotate:(BOOL)rotateEnable;
- (void)mapCenterAs:(MAVector *)pos;
- (void) updatePanBoundAsAABB;
- (void) showMyPos:(BOOL)show x:(float)x y:(float)y z:(float)z red:(float)red green:(float)green blue:(float)blue alpha:(float)alpha rad:(float)rad;
- (void) showDynamicPoi:(BOOL)show x:(float)x y:(float)y z:(float)z;
- (void) refresh;
- (void) initPoiList:(NSArray *)list;
@end
