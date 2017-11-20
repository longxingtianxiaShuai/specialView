//
//  MAManager.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 17..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MAManager.h"

#import "MAMapView.h"
#import "MAStateManager.h"
#import "MAUpdate.h"

#import "CommonLib.h"
#import "DebugOutput.h"
#import "MAVector.h"

@implementation MAManager {
    
    MAStateManager *stateManager;
    MAUpdate *maUpdate;
    MAMapView *mapView;
    NSString *serverUrl;
    unit unitValue;
    int readyForUpdate;
}

-(void)dealloc{
	NSLog(@"%s", __func__);
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super init];
    
    if (self) {
        stateManager = [MAStateManager Instance];
        mapView = [[MAMapView alloc] initWithFrame:frame];
        mapView.touchDelegate = self;
        mapView.mapDelegate = self;
        
        unitValue = meter;
        
        readyForUpdate = 0;
        
    }
    
    return self;
}


-(UIView*)getMapView
{
    return mapView;
}

#pragma mark - SETTING
// 서버주소 설정
-(void)setServerUrl:(NSString*)url
{
    serverUrl = url;
}

// 저장경로설정 - iOS는 사용안함

// Prject ID 설정
-(void)setProjectId:(int)prjId
{
   readyForUpdate = 0;
   stateManager.current_project_id = prjId;
   if (maUpdate == nil) {
        maUpdate = [[MAUpdate alloc] init];
        maUpdate.delegate = self;
   }
    maUpdate.serverUrl = serverUrl;
    maUpdate.projectId = [CommonLib InternalIdValueToExternalIdValue:prjId type:@"P"];
        
    [maUpdate initDataFromServer];
}

#pragma mark - DOWNLOAD
// Zone 다운로드/업데이트 여부 체크
-(void)needUpdate:(int)prjId
{
     [self.delegate onNeedUpdate:([maUpdate isNeedZoneUpdate] || [maUpdate isNeedBuildingUpdate] || [maUpdate isNeedDBUpdate])];
}

- (void) onNetworkFailed
{
    [self.delegate onNetworkFailed];
}

-(void)onReadyForUpdate
{
    readyForUpdate++;
    if (readyForUpdate == 3) {
        [[DebugOutput instance] output:@"서버로부터 데이터 수신 완료..."];
        [self.delegate onReadyForUpdate];
    }
}
// Zone 다운로드 시작
-(void)startUpdate
{
    if (maUpdate == nil) {
        NSAssert(YES, @"MAUpdate 없음!");
    }
    
    [maUpdate startDbUpdate];
    [maUpdate startZoneUpdate];
    [maUpdate startBuildingUpdate];
    [maUpdate updateData];
}

// Building 다운로드 시작


// 다운로드 취소
// Justin - cancel update 시 다운로드 받던 파일들을 지워주어야 하나?
-(void)cancelUpdate
{
    [maUpdate cancelUpdate];
    
}

-(BOOL)isExistProjectData:(int)prjId
{
    return [maUpdate isExistProjectData:prjId];
}
// Zone 하위 데이터 삭제
-(BOOL)deleteProjectData:(int)prjId
{
    [maUpdate deleteData:prjId];
    return YES;
}

-(void)onUpdateStateChanged:(MAUpdate*)state
{
    [self.delegate onUpdateStateChanged:state];
}


#pragma mark - MAP
// Zone 뷰를 로딩한다
-(void)loadZone
{
    [mapView loadZone];
}

// Map 뷰를 로딩한다
-(void)loadMap
{
    [mapView loadMap];
}

-(void)loadMap:(int)buildingId floorId:(NSString *)floorId
{
    [mapView loadMap:buildingId floorId:floorId];
}

// Map의 층을 변경
-(void)changeFloor:(int)floor
{
    
    [mapView changeFloor:floor];
    [mapView updatePanBoundAsAABB];
}

// show texture
-(void) showTexture:(BOOL)show
{
    [mapView showTexture:show];
}

- (void)mapMoveTo:(MAVector *)pos
{
    [mapView mapMoveToPx:pos.x Py:pos.y Pz:pos.z];
}

#warning WONIL Add
- (void)mapCenterAs:(MAVector *)pos //애니메이션 없음.
{
    [mapView mapCenterAsPx:pos.x Py:pos.y Pz:pos.z];
}


- (void)mapMoveOnPoiId:(int)poiId
{
    [mapView showSelectPoi:poiId];
}

- (void)mapMoveOnPoiInfo:(MAPoi *)poiInfo
{
    [mapView showSelectPoi:poiInfo.poi_id];
}

- (NSArray *)listDynamicPoi
{
    return [mapView listDynamicPoi];
}

- (void) removeDynamicPoiAll
{
    [mapView removeDynamicPoiAll];
}

- (void)showPoi:(BOOL)show
{
    [mapView showPoi:show];
}

- (void)showPoiById:(NSArray *)Ids show:(BOOL)show
{
    [mapView showPoiById:Ids show:show];
}

- (void)showPoiByInfo:(NSArray *)pois show:(BOOL)show
{
    [mapView showPoiByInfo:pois show:show];
}

- (void)setBackgroundColorWithRed:(float)red green:(float)green blue:(float)blue
{
    [mapView setBackgroundColorWithRed:red green:green blue:blue];
}

- (void)setDynamicPoi:(NSArray*)pois
{
    [mapView setDynamicPoi:pois];
}

- (void)setShowPoiType:(NSArray *)categoryList
{
    [mapView setShowPoiType:categoryList];
}

- (NSArray *)getFloorInfo:(int)bldgId
{
    NSString *Id = [NSString stringWithFormat:@"%d", bldgId];
    return [mapView getFloorInfo:Id];
}

- (NSArray *)getBuildingInfo:(int)zoneId
{
    NSString *Id = [NSString stringWithFormat:@"%d", zoneId];
    return [mapView getBuildingInfo:Id];
}

- (void) showDynamicPoi:(BOOL)show x:(float)x y:(float)y z:(float)z
{
    [mapView showDynamicPoi:show x:x y:y z:z];
}

- (void)setCameraAngle:(float)angle
{
    NSLog(@"Camera angle = %f",angle);
    
    [mapView setCameraAngle:angle];
}

- (float)getCameraAngle
{
  return [mapView getCameraAngle];
}

- (void)setMapRotation:(float)rotation
{
    [mapView setMapRotation:rotation];
}

- (float)getMapRotation
{
    return [mapView getMapRotation];
}

- (void)setMapAzimuth:(float)azimuth
{
    [mapView setMapAzimuth:azimuth];
}

- (float)getMapAzimuth
{
    return [mapView getMapAzimuth];
}

-(void) removeDynamicPoiById:(int)Id
{
    [mapView removeDynamicPoiById:Id];
}

-(void) removeDynamicPoiByArray:(NSArray *)pois
{
    [mapView removeDynamicPoiByArray:pois];
}

- (void)setZoom:(float)offset
{
    [mapView setZoom:offset];
}

- (float)getZoom
{
    return [mapView getZoom];
}

- (MAVector *)getMapPointX:(int)x andY:(int)y
{
    return [mapView getMapPointX:x andY:y];
}

- (CGPoint)getScreenPoint:(MAVector *)pos
{
    return [mapView getScreenPoint:pos];
}

- (void)moveMapToX:(float)x andY:(float)y
{
    [mapView moveMapToX:x andY:y];
}

- (NSString *)getCurrentZoneId
{
    return [NSString stringWithFormat:@"%d", stateManager.current_zone_id];
}

- (NSString *)getCurrentBuildingId
{
    return [NSString stringWithFormat:@"%d", stateManager.current_building_id];
}

- (NSString *)getCurrentFloorId
{
    return [NSString stringWithFormat:@"%d", stateManager.current_floor_id];
}

- (void) setDistanceUnit:(unit)value
{
    unitValue = value;
}

- (unit) getDistanceUnit
{
    return unitValue;
}

- (BOOL)onPoiTouched:(int)poiId  x:(int)x y:(int)y
{
    if ([self.delegate respondsToSelector:@selector(onPoiTouched:x:y:)]) {
        
        return [self.delegate onPoiTouched:poiId x:x y:y];
    }
    return NO;
}

- (BOOL)onBalloonTouched:(int)poiId
{
    if ([self.delegate respondsToSelector:@selector(onBalloonTouched:)]) {
        return [self.delegate onBalloonTouched:poiId];
    }
    return NO;
}

- (void)onMapLoadingSuccess
{
    if ([self.delegate respondsToSelector:@selector(onMapLoadingSuccess)]) {
        return [self.delegate onMapLoadingSuccess];
    }
}

- (void)onMapLoadingFailed
{
    if ([self.delegate respondsToSelector:@selector(onMapLoadingFailed)]) {
        return [self.delegate onMapLoadingFailed];
    }
}

- (void)setDataPath:(NSString *)path
{
//#if TARGET_IPHONE_SIMULATOR
//    stateManager.downloadPath = @"/Users/VB";
//#else
    stateManager.downloadPath = path;
//#endif
    
    [CommonLib CreateFolder:path];
}

- (NSString *)getDBPath
{
    NSString *projectID = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    return [stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", projectID, projectID]];
}

- (NSString *)getGmlPath
{
    return [stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/%@.gml", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:stateManager.current_building_id type:@"B"], stateManager.building_name_e]];
	/* shin begin */
	//return [NSString stringWithFormat:@"%@/P0102/building/B0127/Simincheong_del_Zoo길안내.gml",[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]];
    /* shin end */
}

- (NSDate *)getLastUpdateDate:(int)prjId
{
    NSString* updateDate = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%d",prjId]];
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyyMMddHHmmss"];
    return [dateFormat dateFromString:updateDate];
}

- (NSString *)getVersion
{
    return @"1.0.0";
}

- (NSString *)getDefaultZoneId
{
    return [mapView getDefaultZoneId];
}
- (NSString *)getDefaultBuildingId
{
    return [mapView getDefaultBuildingId];
}

- (NSString *)getDefaultFloorId
{
    return [mapView getDefaultFloorId];
}

- (void)debugEnable:(BOOL)enable
{
    [DebugOutput instance].enable = enable;
    [mapView debugEnable:enable];
}

- (MAPoi *)getPoiInfoById:(int)poiId
{
    return [mapView getPoiInfoById:poiId];
}

- (void)clearMap
{
    [mapView clearMap];
    //[mapView removeFromSuperview];
}

- (void)addPath:(NSArray *)posList withColor:(UIColor *)color
{
    [mapView addPath:posList withColor:color];
}

- (void)removePath
{
    [mapView removePath];
}

- (void)setPoiTextSize:(int)size
{
    [mapView setPoiTextSize:size];
}
- (void)setPoiTextColor:(UIColor *)color
{
    [mapView setPoiTextColor:color];
}

- (NSArray *)getPoiCategory
{
   return [mapView getPoiCategory];
}

- (NSArray *)getPoiInfoListByFloorId:(NSString *)floorId
{
    return [mapView getPoiInfoListByFloorId:floorId];
}

- (NSArray *)getPoiInfoListOfCurrentFloor
{
    return [mapView getPoiInfoListOfCurrentFloor];
}

- (void)changePoiIcon:(int)poiId name:(NSString *)imageName
{
    [mapView changePoiIcon:poiId name:imageName];
}

- (void)restorePoiIcon:(int)poiId
{
    [mapView restorePoiIcon:poiId];
}

- (void)setCameraMode:(cameraMode)mode
{
//    NSLog(@"Camera Mode - %d",(int)mode);
    
    [mapView setCameraMode:mode];
}

- (void)showPath:(BOOL)show
{
    [mapView showPath:show];
}

- (void) clearData{
    maUpdate = nil;
    stateManager.current_building_id = 0;
    stateManager.current_floor_id = 0;
    stateManager.current_zone_id = 0;
    stateManager.default_bldgId = nil;
    stateManager.default_floorId = nil;
    stateManager.default_zoneId = nil;
    
    //stateManager = [MAStateManager Instance];
}

- (NSArray *)getCategoryList:(NSString *)prjId
{
    return [mapView getCategoryList:prjId];
}

- (NSArray *)getPoiInfoList
{
    return [mapView getPoiInfoList];
}

- (void) changeFloorById:(NSString *)floorId
{
    [mapView changeFloorById:floorId];
    [mapView updatePanBoundAsAABB];
}

- (NSArray *) getPoiInfoListByCategoryId:(int)categoryId
{
   return [mapView getPoiInfoListByCategoryId:categoryId];
}

- (void) loadMapAll:(int)buildingId
{
    [mapView loadMapAll:(int)buildingId];
}

- (void) showMapAll:(BOOL)show
{
    [mapView showMapAll:show];
}

- (void)setCameraYaw:(float)fYaw PitchRatio:(float)fPitchRatio Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    return [mapView setCameraYaw:fYaw PitchRatio:fPitchRatio Position3DX:fX Y:fY Z:fZ Offset:offset];
}

- (void)setCameraYaw:(float)fYaw Pitch:(float)fPitch Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    return [mapView setCameraYaw:fYaw Pitch:fPitch Position3DX:fX Y:fY Z:fZ Offset:offset];
}

- (void) setVerticalScale:(float)scale
{
    [mapView setVerticalScale:scale];
}

- (void)addPath:(NSArray *)posList withColor:(UIColor *)color withFloorId:(int)floorId
{
    [mapView addPath:posList withColor:color withFloorId:floorId];
}

- (void) setPathVisibility:(BOOL)show withFloorId:(int)floorId exclusive:(BOOL)exclusive
{
    [mapView setPathVisibility:show withFloorId:floorId exclusive:exclusive];
}

- (void) setGroundLevel:(float)groundLevel
{
    [mapView setGroundLevel:groundLevel];
}

- (void) setHRotationAnimation:(BOOL)play step:(float)step
{
    [mapView setHRotationAnimation:play step:step];
}

- (void) brokeCurrentViewControlPan:(BOOL)panEnable zoom:(BOOL)zoomEnable rotate:(BOOL)rotateEnable
{
    [mapView brokeCurrentViewControlPan:panEnable zoom:zoomEnable rotate:rotateEnable];
}

- (void) updatePanBoundAsAABB
{
    [mapView updatePanBoundAsAABB];
}

- (void) showMyPos:(BOOL)show x:(float)x y:(float)y z:(float)z red:(float)red green:(float)green blue:(float)blue alpha:(float)alpha rad:(float)rad
{
    [mapView showMyPos:show x:x y:y z:z red:red green:green blue:blue alpha:alpha rad:rad];
}

- (void) refresh
{
    [mapView refresh];
}

- (void) initPoiList:(NSArray *)list
{
    [mapView initPoiList:list];
}

@end
