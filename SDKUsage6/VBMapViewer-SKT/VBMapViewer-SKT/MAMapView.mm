//
//  MAMapView.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 2..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MAMapView.h"
#import "GLView.h"

#import "PoiCanvas.h"
//#import "MapUiView.h"

#import "ActivityWindow.h"
#import "CommonLib.h"

#import "MALoadMapInfo.h"
#import "MAFloorInfo.h"

#import "MADbPool.h"
#import "MAStateManager.h"

#import "MAPoi.h"
#import "MAVector.h"
#import "MAZoneInfo.h"
#import "DebugOutput.h"
#import "TouchInfo.h"

@implementation MAMapView {
    MAStateManager *stateManager;
    PoiCanvas *poiCanvas;
    MADbPool *dbPool;
    NSArray *sbmList;
    
}

- (id)initWithFrame:(CGRect)frame
{
    //self = [super initWithFrame:frame];
    poiCanvas = [[PoiCanvas alloc] initWithFrame:frame];
    poiCanvas.delegate = self;

    self = [super initWithFrame:frame withDelegate2:poiCanvas];
    
    if (self) {
        // Initialization code
        
        stateManager = [MAStateManager Instance];
        dbPool = [MADbPool Instance];
        
        poiCanvas.touchDelegate = self;
        
        [[self GLView] SetViewControlSet:6];
        [[self GLView] addSubview:poiCanvas];
        [self addSubview:[self GLView]];
        
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(touchBegin:) name:@"touchBegin" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(touchMove:) name:@"touchMove" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(touchEnd:) name:@"touchEnd" object:nil];
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    [poiCanvas showPoi:YES];
}
*/


- (void) touchBegin:(NSNotification *)noti
{
    TouchInfo *info = [noti object];
    [[self GLView] touchesBegan:info.touches withEvent:info.events];
    NSSet* allTouches = [info.events allTouches];
    
    NSLog(@"touch bagan count = %lu", (unsigned long)[allTouches count]);
}

- (void) touchMove:(NSNotification *)noti
{
    TouchInfo *info = [noti object];
    [[self GLView] touchesMoved:info.touches withEvent:info.events];
    NSSet* allTouches = [info.events allTouches];
    
    NSLog(@"touch move count = %lu", (unsigned long)[allTouches count]);
}

- (void) touchEnd:(NSNotification *)noti
{
    TouchInfo *info = [noti object];
    [[self GLView] touchesEnded:info.touches withEvent:info.events];
  
    NSSet* allTouches = [info.events allTouches];
    
    NSLog(@"touch move count = %lu", (unsigned long)[allTouches count]);
}

- (void)ShowActivity {
    // 파일로딩중에 Activity가 보여야 하므로 쓰레드로 호출한다.
    [NSThread detachNewThreadSelector:@selector(ShowActivityInThread) toTarget:self withObject:nil];
}

- (void)ShowActivityInThread {
    [ActivityWindow Show];
}

- (void)HideActivity {
    [ActivityWindow Hide];
}

#pragma mark - MAP
// Zone 뷰를 로딩한다
-(void)loadZone
{
     // 1. 현재 프로젝트의 로딩정보 획득
     MALoadMapInfo *loadingInfo = [dbPool getLoadMapInfo:[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]];
     
     stateManager.current_zone_id = [loadingInfo.zone_id intValue];
     stateManager.current_building_id = [loadingInfo.building_id intValue];
     stateManager.current_floor_id = [loadingInfo.floor_id intValue];
     stateManager.isZone = YES;
     stateManager.building_name_e = loadingInfo.building_name_e;
     // 2. 파일로딩
     // 파일 로드
     [self ShowActivity];
    
     NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/zone/%@.sbm", loadingInfo.project_id, [CommonLib InternalIdValueToExternalIdValue:[loadingInfo.zone_id intValue] type:@"Z"]]];
     NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/zone/ResizeMaps", loadingInfo.project_id]];
    
    [[DebugOutput instance] output:@"sbmPath : %@", sbmPath];
    [[DebugOutput instance] output:@"texturePath : %@", texturePath];
    
     [self sbmLoad:sbmPath texturePath:texturePath];
     
     [self HideActivity];
}

- (void) loadMapAll:(int)buildingId
{
    MALoadMapInfo *loadingInfo = [dbPool getLoadMapInfo:[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]];
    
    stateManager.current_zone_id = [loadingInfo.zone_id intValue];
    stateManager.current_building_id = buildingId;
   // stateManager.current_floor_id = [floorId intValue];
    stateManager.building_name_e = loadingInfo.building_name_e;
    
    stateManager.isZone = NO;
    stateManager.north_axis = [dbPool getNorthAxis:[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]];
    // 2. 파일로딩
    // 파일 로드
    [self ShowActivity];
    /*
    NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/%@", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:buildingId type:@"B"], loadingInfo.sbm_file]];*/
    NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/ResizeMaps", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:buildingId type:@"B"]]];
    
    sbmList = [dbPool getSbmFileList:[NSString stringWithFormat:@"%@", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]]];
   // [[DebugOutput instance] output:@"sbmPath : %@", sbmPath];
    [[DebugOutput instance] output:@"texturePath : %@", texturePath];
    
    NSString *sbmDir = [stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:buildingId type:@"B"]]];
                        
    BOOL ret = [self sbmLoadAll:sbmDir sbmList:sbmList texturePath:texturePath];
    
    //[poiCanvas SelectedFloorChanged:[floorId intValue]];
    [poiCanvas loadLodInfo];
    [[self GLView] SetCameraMin:CAMERA_MIN_DIST MaxOffset:[dbPool getMaxDistance]*1.5f];
    [[self GLView] FitAABB2VisibleGroup:true];
    
    [self HideActivity];
    
    if (ret) {
        if ([self.mapDelegate respondsToSelector:@selector(onMapLoadingSuccess)]) {
            [[DebugOutput instance] output:@"맵 로딩 성공..."];
            return [self.mapDelegate onMapLoadingSuccess];
        }
    } else {
        if ([self.mapDelegate respondsToSelector:@selector(onMapLoadingFailed)]) {
            [[DebugOutput instance] output:@"맵 로딩 실패..."];
            return [self.mapDelegate onMapLoadingFailed];
        }
    }

}
// Map 뷰를 로딩한다
-(void)loadMap:(int)buildingId floorId:(NSString *)floorId
{
    // 1. 현재 프로젝트의 로딩정보 획득
    MALoadMapInfo *loadingInfo = [dbPool getLoadMapInfo:[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]];
    
    stateManager.current_zone_id = [loadingInfo.zone_id intValue];
    stateManager.current_building_id = buildingId;
    stateManager.current_floor_id = [floorId intValue];
     stateManager.building_name_e = loadingInfo.building_name_e;
    
    stateManager.isZone = NO;
    stateManager.north_axis = [dbPool getNorthAxis:[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]];
    // 2. 파일로딩
    // 파일 로드
    [self ShowActivity];
    
    NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/%@", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:buildingId type:@"B"], loadingInfo.sbm_file]];
    NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/ResizeMaps", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:buildingId type:@"B"]]];
    
    [[DebugOutput instance] output:@"sbmPath : %@", sbmPath];
    [[DebugOutput instance] output:@"texturePath : %@", texturePath];
    
    BOOL ret = [self sbmLoad:sbmPath texturePath:texturePath];
    
    [poiCanvas SelectedFloorChanged:[floorId intValue]];
    [poiCanvas loadLodInfo];
    [[self GLView] SetCameraMin:CAMERA_MIN_DIST MaxOffset:poiCanvas.lodInfo.cameraMaxDistance * 1.3f];
    [[self GLView] FitAABB2VisibleGroup:true];
    
    [self HideActivity];
    
    if (ret) {
        if ([self.mapDelegate respondsToSelector:@selector(onMapLoadingSuccess)]) {
            [[DebugOutput instance] output:@"맵 로딩 성공..."];
            return [self.mapDelegate onMapLoadingSuccess];
        }
    } else {
        if ([self.mapDelegate respondsToSelector:@selector(onMapLoadingFailed)]) {
            [[DebugOutput instance] output:@"맵 로딩 실패..."];
            return [self.mapDelegate onMapLoadingFailed];
        }
    }
}

-(void)loadMap
{
     // 1. 현재 프로젝트의 로딩정보 획득
     MALoadMapInfo *loadingInfo = [dbPool getLoadMapInfo:[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"]];
     
     stateManager.current_zone_id = [loadingInfo.zone_id intValue];
     stateManager.current_building_id = [loadingInfo.building_id intValue];
     stateManager.current_floor_id = [loadingInfo.floor_id intValue];
     
     stateManager.isZone = NO;
     
     // 2. 파일로딩
     // 파일 로드
     [self ShowActivity];
    
     NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/%@", loadingInfo.project_id, [CommonLib InternalIdValueToExternalIdValue:[loadingInfo.building_id intValue] type:@"B"], loadingInfo.sbm_file]];
     NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/ResizeMaps", loadingInfo.project_id, [CommonLib InternalIdValueToExternalIdValue:[loadingInfo.building_id intValue] type:@"B"]]];
     
    [[DebugOutput instance] output:@"sbmPath : %@", sbmPath];
    [[DebugOutput instance] output:@"texturePath : %@", texturePath];
    
     
     [self sbmLoad:sbmPath texturePath:texturePath];
     
     [poiCanvas SelectedFloorChanged:[loadingInfo.floor_id intValue]];
     
     [self HideActivity];
}

// Map 뷰를 로딩한다
-(void)loadMap:(NSString*)prjId buildingId:(NSString*)buildingId floor:(int)floor
{
     // 파일 로드
     [self ShowActivity];
    
     NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/B0090_m_1F_0.sbm", prjId, buildingId]];
     NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/ResizeMaps",prjId,buildingId]];
     
     [self sbmLoad:sbmPath texturePath:texturePath];
     
     [self HideActivity];
}


// Map의 Texture Show/Hide
-(void)showTexture:(BOOL)show
{
    vbGLRenderMode mode = show ? RENDERMODE_DEFAULT : RENDERMODE_SOLID_COLOR;
    [[self GLView] setRenderMode:mode];
}

- (void) changeFloorById:(NSString *)floorId
{
    [self ShowActivity];
    
    MAFloorInfo *floorInfo = [dbPool getFloorById:floorId];
    
    if (floorInfo.building_id == Nil) {
        NSAssert(NO, @"층이 로딩된 상태여야만합니다!");
    }
    
    //stateManager.current_building_id = [floorInfo.building_id intValue];
    stateManager.current_floor_id = [floorInfo.floor_id intValue];
    
    stateManager.isZone = NO;
    
    NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/%@", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:stateManager.current_building_id type:@"B"], floorInfo.sbm_file]];
    NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/ResizeMaps",[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"],[CommonLib InternalIdValueToExternalIdValue:stateManager.current_building_id type:@"B"]]];
    
    [[DebugOutput instance] output:@"sbmPath : %@", sbmPath];
    [[DebugOutput instance] output:@"texturePath : %@", texturePath];
    
    [poiCanvas SelectedFloorChanged:stateManager.current_floor_id];
    [poiCanvas loadLodInfo];
    

    //WONIL - Add *2
//#warning 매니져에서 입력한 최대 거리가 모바일에서는 대체로 너무 가깝게 보이므로 2배 처리한다.(심원일)
//    NSLog(@"App Min-Max = %f - %f",CAMERA_MIN_DIST, poiCanvas.lodInfo.cameraMaxDistance);
    [self sbmLoadAtFloor:floorInfo withTexturePath:texturePath];
    [[self GLView] SetCameraMin:CAMERA_MIN_DIST MaxOffset:poiCanvas.lodInfo.cameraMaxDistance * 1.3f];

//#warning 보이게 될 층을 화면에 맞추고, 맞춘 상태가 min/max를 벗어나면 다시 조정한다.
    //WONIL
    [[self GLView] FitAABB2VisibleGroup:true];
    //아래는 혹시 위에 것이 맘에 안들 때 좀 더 심플하게 쓸 수 있는 메쏘드.
    //[[self GLView] SetCameraPostion2VisibleGroupCenter:poiCanvas.lodInfo.cameraMaxDistance];
    
    [self HideActivity];
}
// Map의 층을 변경 - Justin
-(void)changeFloor:(int)floor
{
    MAFloorInfo *floorInfo = [dbPool getFloor:floor];
    [self changeFloorById:floorInfo.floor_id];
    // 파일 로드
    /*
    [self ShowActivity];
    
    MAFloorInfo *floorInfo = [dbPool getFloor:floor];
    
    if (floorInfo.building_id == Nil) {
        NSAssert(NO, @"층이 로딩된 상태여야만합니다!");
    }
    
    //stateManager.current_building_id = [floorInfo.building_id intValue];
    stateManager.current_floor_id = [floorInfo.floor_id intValue];
    
    stateManager.isZone = NO;
    
   NSString* sbmPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/%@", [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"], [CommonLib InternalIdValueToExternalIdValue:stateManager.current_building_id type:@"B"], floorInfo.sbm_file]];
    NSString* texturePath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/building/%@/ResizeMaps",[CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"],[CommonLib InternalIdValueToExternalIdValue:stateManager.current_building_id type:@"B"]]];
    
    [[DebugOutput instance] output:@"sbmPath : %@", sbmPath];
    [[DebugOutput instance] output:@"texturePath : %@", texturePath];
    
    [poiCanvas SelectedFloorChanged:stateManager.current_floor_id];
    [poiCanvas loadLodInfo];
    [[self GLView] SetCameraMin:CAMERA_MIN_DIST MaxOffset:poiCanvas.lodInfo.cameraMaxDistance];
    
    [self sbmLoad:sbmPath texturePath:texturePath];
    
    [self HideActivity];
    */
    //return YES;
}

// 스크린좌표를 전달받아 Map 좌표를 반환
-(MAVector *)getMapPointX:(int)x andY:(int)y;
{
    MAVector *position = [[MAVector alloc] init];
    vec3 v = [[self GLView] GetGroundPositionOnScreenX:x Y:y];
    
    position.x = v.x;
    position.y = v.y;
    position.z = v.z;
    
    return position;
}

// 현재 Map 좌표에 해당하는 스크린 좌표를 반환
- (CGPoint)getScreenPoint:(MAVector *)pos
{
    return [poiCanvas getScreenPoint:pos];
}

// Map 이 몇도 회전하였는지를 리턴
-(void)setMapRotation:(float)rotation
{
    [[self GLView] SetCameraYaw:rotation];
}

-(float)getMapRotation
{
    return [[self GLView] GetCameraYaw];
}

- (void)setMapAzimuth:(float)azimuth
{
    [[self GLView] SetCameraYaw:(azimuth + stateManager.north_axis)];
}

- (float)getMapAzimuth
{
    return ([[self GLView] GetCameraYaw] - stateManager.north_axis);
}

// 현재 맵을 상하좌우로 설정된 픽셀만큼 이동
-(void)moveMapToX:(int)x andY:(int)y;
{
    vec3 v = [[self GLView] GetGroundPositionOnScreenX:x+self.center.x Y:y+self.center.y];
    [[self GLView] SetCameraToSee3DPointPx:v.x Py:v.y Pz:v.z];
}

// 카메라 상하 앵글값을 세팅
-(void)setCameraAngle:(float)angle
{
    [[self GLView] SetCameraPitch:(angle * -1.0f)];
    //[[self GLView] setVerticalScale:1.0];
}

-(float)getCameraAngle
{
    return ([[self GLView] GetCameraPitch] * -1.0f);
}

// 카메라 좌우 회전값을 세팅


// 카메라 Zoom In/Out
-(void)setZoom:(float)zoom
{
    [[self GLView] SetCameraOffset:zoom];
}

-(float)getZoom
{
    return [[self GLView] GetCameraOffset];
}

// 현재 맵의 리소스를 해제하고 초기상태로 되돌린다
-(void)clearMap
{
    [self uninit];
}


// MapView 리턴
//-(UIView*)getMapView
//{
//    return self;
//}


#pragma mark - POI

- (void) refresh
{
    [poiCanvas refresh];
}

// 전체 POI Show/Hide
-(void)showPoi:(BOOL)show
{
    [poiCanvas showPoi:show];
}

- (void)showPoiById:(NSArray *)Ids show:(BOOL)show
{
    [poiCanvas showPoiById:Ids show:show];
}

- (void)showPoiByInfo:(NSArray *)pois show:(BOOL)show
{
    [poiCanvas showPoiByInfo:pois show:show];
}

// 현재 사용중인 유형전체를 리턴
- (NSArray *)getPoiCategory
{
    return [poiCanvas getPoiCategory];
}

// 선택된 유형에 해당하는 POI만 가시화
-(void)setShowPoiType:(NSArray *)categoryList
{
    [poiCanvas SetCategories:categoryList];
}

// 현재 맵상의 특정 POI를 화면 중앙에 위치시킴
-(void)showSelectPoi:(int)poiId
{
    MAPoi *poi = [dbPool getPOIInfoFromId:poiId];
    [[self GLView] SetCameraToSee3DPointPx:[poi.x floatValue] Py:[poi.y floatValue] Pz:[poi.z floatValue] * -1.0];
}

// 동적 POI 리스트를 세팅
-(void)setDynamicPoi:(NSArray*)pois
{
    [poiCanvas setDynamicPoi:pois];
}

-(NSArray *)listDynamicPoi
{
    return [poiCanvas listDynamicPoi];
}

- (void) removeDynamicPoiAll
{
    [poiCanvas removeDynamicPoiAll];
}

-(void) removeDynamicPoiById:(int)Id
{
    [poiCanvas removeDynamicPoiById:Id];
}

-(void)removeDynamicPoiByArray:(NSArray *)pois
{
    [poiCanvas removeDynamicPoiByArray:pois];
}

- (void)setPoiTextSize:(int)size
{
    [poiCanvas setPoiTextSize:size];
}
- (void)setPoiTextColor:(UIColor *)color
{
    [poiCanvas setPoiTextColor:color];
}

#pragma mark - NAVIGATION
// 출발지, 경유지, 목적지 POI 정보를 세팅 (화면상에 POI 그리기 위함)
-(void)setPathInfo:(NSArray*)pois
{
    
}

// Guidance로부터 얻은 경로 및 출발지, 목적지, 경유지 POI, 그리고 층변경 아이콘을 화면에 그림 (이미 Guidance에 경로가 획득되어 있어야 함). 층변경 아이콘 누르면 다음 경로 연결 층 로딩
-(BOOL)drawTotalPath
{
    return YES;
}

// Guidance로부터 현위치를 받아 경로와 비교하여 지나간 경로는 다른색으로 표시
-(BOOL)drawPassPass
{
    return YES;
}

// 현재 그려진 경로 및 관련 아이콘들을 화면에서 제거
-(void)removePath
{
    [[self GLView] removePath];
}

// 경로 라인의 색상을 변경
-(void)setPathColor:(float)red green:(float)green blue:(float)blue
{
    
}

// 지나간 경로 라인의 색상을 변경
-(void)setPassPathColor:(float)red green:(float)green blue:(float)blue
{
    
}

// 지속적으로 내위치를 표시한다.
-(void)showMyPositionAtNavigation:(int)mode
{
    
}

// 현위치 아이콘 뷰를 받아 세팅
-(void)setMyPositionIconView:(UIView*)myView
{
    
}


#pragma mark - ETC

// 프로젝트에 포함된 전체 빌딩 정보 리턴
-(NSArray*)getBuildingInfo:(NSString*)zoneId
{
    return [dbPool getBuildingInfo:zoneId];
}

// 빌딩에 포함된 층정보 리턴
-(NSArray*)getFloorInfo:(NSString*)bldgId
{
    return [dbPool getFloorInfo:bldgId];
}

// 맵의 배경색을 변경한다
-(void)setBackgroundColorWithRed:(float)red green:(float)green blue:(float)blue
{
    RunTimeConfig config = [[self GLView] getRuntimeConfig];
    config.fBackgroundColor4f[0] = red / 255.0f;
    config.fBackgroundColor4f[1] = green / 255.0f;
    config.fBackgroundColor4f[2] = blue / 255.0f;
    config.fBackgroundColor4f[3] = 1.0f;
    
    [[self GLView] setRuntimeConfig:&config];
}

#warning WONIL - Animation으로 이동(이동 중 사용자 입력이 막힘)
- (void)mapMoveToPx:(float)x Py:(float)y Pz:(float)z
{
    [[self GLView] SetCameraToSee3DPointPx:x Py:y Pz:z];
}

#warning WONIL - Animation없이 바로 이동.
- (void)mapCenterAsPx:(float)x Py:(float)y Pz:(float)z
{
    [[self GLView] SetCameraCenterAsPx:x Py:y Pz:z];
}


- (NSString *)getDefaultZoneId
{
    if (stateManager.default_zoneId == nil) {
        stateManager.default_zoneId = [dbPool getDefaultZoneId];
    }
    return stateManager.default_zoneId;
}

- (NSString *)getDefaultBuildingId
{
    if (stateManager.default_bldgId == nil) {
        stateManager.default_bldgId = [dbPool getDefaultBuildingId];
    }
    return stateManager.default_bldgId;
}
- (NSString *)getDefaultFloorId
{
    if (stateManager.default_floorId == nil) {
        stateManager.default_floorId = [dbPool getDefaultFloorId];
    }
    return stateManager.default_floorId;

}

- (MAPoi *)getPoiInfoById:(int)poiId
{
    return [poiCanvas getPoiInfoById:poiId];
}

- (void)addPath:(NSArray *)posList withColor:(UIColor *)color withFloorId:(int)floorId
{
    float *pts = new float[[posList count] * 3];
    int count = 0;
    for (MAVector *v in posList) {
        pts[count * 3 + 0] = v.x;
        pts[count * 3 + 1] = v.y;
        pts[count * 3 + 2] = v.z;
        count ++;
    }
    
    float c[4];
    CGFloat cf[4];
    [color getRed:&cf[0] green:&cf[1] blue:&cf[2] alpha:&cf[3]];
    for (int i = 0; i < 4; i++) {
        c[i] = (float)cf[i];
    }
    
    [[self GLView] AddPathData3DPts:pts Num:[posList count] Color:(float *)&c[0] Width:100.f :true WithFloorID:floorId]; //모든 경로를 다 그리도록 수정
    
    delete []pts;

}

- (void)addPath:(NSArray *)posList withColor:(UIColor *)color
{
    float *pts = new float[[posList count] * 3];
    int count = 0;
    for (MAVector *v in posList) {
        pts[count * 3 + 0] = v.x;
        pts[count * 3 + 1] = v.y;
        pts[count * 3 + 2] = v.z;
        count ++;
    }
    
    float c[4];
    CGFloat cf[4];
    [color getRed:&cf[0] green:&cf[1] blue:&cf[2] alpha:&cf[3]];
    for (int i = 0; i < 4; i++) {
        c[i] = (float)cf[i];
    }
    
    [[self GLView] AddPathData3DPts:pts Num:[posList count] Color:(float *)&c[0] Width:100.f :true]; //모든 경로를 다 그리도록 수정
   
    delete []pts;
}



                        
                        
                        
- (BOOL)sbmLoadAll:(NSString *)dir sbmList:(NSArray *)fileList texturePath:(NSString *)texturePath
{
    VBMapErrorCode ret;
    [poiCanvas loadLodInfo];
    
    [[self GLView] clearData];
    for (NSString *f in fileList) {
        NSString *sbmPath = [dir stringByAppendingPathComponent:f];
        ret = [[self GLView] ImportMap:sbmPath withTexturePath:texturePath];
    }
    
    
 //   [[self GLView] SetCameraYaw:0.f PitchRatio:0.8f Position3DX:0.f Y:0.f Z:0.f Offset:poiCanvas.lodInfo.cameraMaxDistance];
        
    if (ret == VBMapError_NO_ERROR) {
        return YES;
    } else {
        return NO;
    }

}
                        
-(void)sbmLoadAtFloor:(MAFloorInfo *)floorInfo withTexturePath:(NSString *)texturePath
{
    [poiCanvas loadLodInfo];
    [[self GLView] SetVisibilityFlag:false WithGroupName:nil];//모든 그룹 숨기기
    for (NSString *sbm in [dbPool getSameFloor:floorInfo]) {
        [[self GLView] SetVisibilityFlag:true WithGroupName:sbm];//1층만 보이도록 설정.
    }
    
    [[self GLView] ApplyVisibilityFlagsToMesh];//Visibility flag의 적용.
    
//    [[self GLView] SetCameraYaw:0.f PitchRatio:0.8f Position3DX:0.f Y:0.f Z:0.f Offset:poiCanvas.lodInfo.cameraMaxDistance];
    /*
    if (ret == VBMapError_NO_ERROR) {
        return YES;
    } else {
        return NO;
    }
*/
    
}

- (void) showDynamicPoi:(BOOL)show x:(float)x y:(float)y z:(float)z
{
    [poiCanvas showDynamicPoi:show x:x y:y z:z];
}

-(BOOL)sbmLoad:(NSString*)sbmPath texturePath:(NSString*)texturePath
{
    [poiCanvas loadLodInfo];
    VBMapErrorCode ret = [[self GLView] LoadMap:sbmPath withTexturePath:texturePath];
    
    /*
     //TEST Code
     
    ret = [[self GLView] ImportMap:@"/Users/VB/P0482/building/B0470/Simincheong_del_청사 및 도서관 1층_2.sbm" withTexturePath:texturePath];
    
    NSString* group = [[self GLView] IsExistingGroupContaining:@"Simincheong_del_청사 및 도서관 1층_2.sbm"];
    if(group==nil)    NSLog(@"NO Group");
    else               NSLog(@"Found group = %@",group);
     
     //다 Hide시키기
    [[self GLView] SetVisibilityFlag:false WithGroupName:nil];//모든 그룹 숨기기
    [[self GLView] SetVisibilityFlag:true WithGroupName:@"Simincheong_del_청사 및 도서관 1층_2.sbm"];//1층만 보이도록 설정.
    [[self GLView] ApplyVisibilityFlagsToMesh];//Visibility flag의 적용.
    
    //*/
    

    //[[self GLView] SetGroundLevel:float(-660.f)]; //unrecognized selector...에러가 왜나지????
    
    
//    [[self GLView] SetCameraYaw:0.f PitchRatio:0.8f Position3DX:0.f Y:0.f Z:0.f Offset:poiCanvas.lodInfo.cameraMaxDistance];
    
    if (ret == VBMapError_NO_ERROR) {
        return YES;
    } else {
        return NO;
    }
}

- (void)debugEnable:(BOOL)enable
{
    [[self GLView] EnableDebugLog:enable];
}

- (NSArray *)getPoiInfoListByFloorId:(NSString *)floorId
{
    return [poiCanvas getPoiInfoListByFloorId:floorId];
}

- (NSArray *) getPoiInfoListByCategoryId:(int)categoryId
{
    return [poiCanvas getPoiInfoListByCategoryId:categoryId];
}

- (NSArray *)getPoiInfoListOfCurrentFloor
{
    return [poiCanvas getPoiInfoListByFloorId:[NSString stringWithFormat:@"%d",stateManager.current_floor_id]];
}

- (void)changePoiIcon:(int)poiId name:(NSString *)imageName
{
    [poiCanvas changePoiIcon:poiId name:imageName];
}

- (void)restorePoiIcon:(int)poiId
{
    [poiCanvas restorePoiIcon:poiId];
}

- (void)setCameraMode:(cameraMode)mode
{
    switch (mode) {
        case eTopView:
            [self setCameraAngle:kTopView];
            //[[self GLView] setVerticalScale:0.1];
            break;
        case eBirdView:
            [self setCameraAngle:kBirdView];
            //[[self GLView] setVerticalScale:1.0];
            break;
        case eWalkingView:
            [self setCameraAngle:kWalkingView];
            //[[self GLView] setVerticalScale:1.0];
            break;
        default:
            break;
    }
}

- (void)showPath:(BOOL)show
{
    [[self GLView] showPath:show];
}

- (BOOL)onPoiTouched:(int)poiId x:(int)x y:(int)y
{
    if ([_touchDelegate respondsToSelector:@selector(onPoiTouched:x:y:)]) {
        return [_touchDelegate onPoiTouched:poiId x:x y:y];
    }
    return NO;
}

- (BOOL)onBalloonTouched:(int)poiId
{
    if ([_touchDelegate respondsToSelector:@selector(onBalloonTouched:)]) {
        return [_touchDelegate onBalloonTouched:poiId];
    }
    return NO;
}

- (NSArray *)getCategoryList:(NSString *)prjId
{
    return [dbPool getCategoryList:prjId];
}

- (NSArray *)getPoiInfoList
{
    return [poiCanvas getPoiInfoList];
}

- (void) showMapAll:(BOOL)show
{
    [poiCanvas loadLodInfo];
    [[self GLView] SetVisibilityFlag:show WithGroupName:nil];//모든 그룹 숨기기
//    [[self GLView] SetVisibilityFlag:true WithGroupName:[dbPool getSbmGroupName:floorId]];//1층만 보이도록 설정.
    [[self GLView] ApplyVisibilityFlagsToMesh];//Visibility flag의 적용.
    
    //WONIL - Add *2
//#warning 매니져에서 입력한 최대 거리가 모바일에서는 대체로 너무 가깝게 보이므로, 전체층은 3배 처리한다.(심원일)
//    [[self GLView] SetCameraMin:CAMERA_MIN_DIST MaxOffset:poiCanvas.lodInfo.cameraMaxDistance];
    [[self GLView] SetCameraMin:CAMERA_MIN_DIST MaxOffset:[dbPool getMaxDistance]*1.5f];
    
   //[[self GLView] SetCameraYaw:0.f PitchRatio:0.8f Position3DX:0.f Y:0.f Z:0.f Offset:poiCanvas.lodInfo.cameraMaxDistance];
//#warning 화면 맞춰주는 메쏘드.
    //WONIL
    [[self GLView] FitAABB2VisibleGroup:true];
    //아래는 좀 더 심플한 방법(바운딩박스의 가운데를 보도록하는 ...
    //[[self GLView] SetCameraPostion2VisibleGroupCenter:poiCanvas.lodInfo.cameraMaxDistance];
}

- (void)setCameraYaw:(float)fYaw PitchRatio:(float)fPitchRatio Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    [[self GLView] SetCameraYaw:fYaw PitchRatio:fPitchRatio Position3DX:fX Y:fY Z:fZ Offset:offset];
}

//WONIL
- (void)setCameraYaw:(float)fYaw Pitch:(float)fPitc Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    [[self GLView] SetCameraYaw:fYaw Pitch:0.f Position3DX:fX Y:fY Z:fZ Offset:offset];
}


- (void) setVerticalScale:(float)scale
{
    [[self GLView] setVerticalScale:scale];
}

- (void) setPathVisibility:(BOOL)show withFloorId:(int)floorId exclusive:(BOOL)exclusive
{
    [[self GLView] SetPathVisibility:show WithFloorID:floorId :exclusive];
}

- (void) setGroundLevel:(float)groundLevel
{
    
    [[self GLView] SetGroundLevel:groundLevel];
    [poiCanvas setGroundLevel:groundLevel];
}

- (void) setHRotationAnimation:(BOOL)play step:(float)step
{
    [[self GLView] setHRotationAnimation:play WithYawStep:step];
}

- (void) brokeCurrentViewControlPan:(BOOL)panEnable zoom:(BOOL)zoomEnable rotate:(BOOL)rotateEnable
{
    [[self GLView] BrokeCurrentViewControlPan:panEnable Zoom:zoomEnable Rotate:rotateEnable];
}

- (void) updatePanBoundAsAABB
{
    [[self GLView] UpdatePanBoundAsAABB];
}

- (void) showMyPos:(BOOL)show x:(float)x y:(float)y z:(float)z red:(float)red green:(float)green blue:(float)blue alpha:(float)alpha rad:(float)rad
{
    [[self GLView] ShowMyPos:show At:x :y :z :red :green :blue :alpha :rad];
}

- (void) initPoiList:(NSArray *)list
{
    [poiCanvas initPoiList:list];
}

@end
