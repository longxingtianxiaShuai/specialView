//
//  MAMapView.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 2..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "MAOpenGlView.h"
#import "PoiCanvas.h"
#import "MAManager.h"




@class MAPoi;
@class MAVector;

@interface MAMapView : MAOpenGlView <PoiCanvasDelegate>

@property(nonatomic,assign) id touchDelegate;
@property(nonatomic,assign) id mapDelegate;

#pragma mark - SETTING
// 서버주소 설정
//-(void)setServerUrl:(NSString*)url;

// 저장경로설정 - iOS는 사용안함

// Prject ID 설정
//-(void)setProjectId:(NSString*)prjId;


/*
#pragma mark - DOWNLOAD
// Zone 다운로드/업데이트 여부 체크
-(BOOL)needZoneUpdate:(NSString*)projectId;

// Building 다운로드/업데이트 여부 체크
-(BOOL)needBuildingUpdate:(NSString*)buildingId;

// Zone 다운로드 시작
-(void)startZoneUpdate:(NSString*)zoneId;

// Building 다운로드 시작
-(void)startBuildingUpdate:(NSString*)buildingId;

// 다운로드 취소
-(void)cancelUpdate;

// Zone 하위 데이터 삭제
-(void)deletaData:(NSString*)zoneId;
*/

#pragma mark - MAP
// Zone 뷰를 로딩한다
-(void)loadZone;

// Map 뷰를 로딩한다
-(void)loadMap;

// Map 뷰를 로딩한다
-(void)loadMap:(int)buildingId floorId:(NSString *)floorId;

-(BOOL)sbmLoad:(NSString*)sbmPath texturePath:(NSString*)texturePath;

// Map의 Texture Show/Hide
-(void)showTexture:(BOOL)show;

// Map의 층을 변경
-(void)changeFloor:(int)floor;


// 스크린좌표를 전달받아 Map 좌표를 반환
-(MAVector *)getMapPointX:(int)x andY:(int)y;

// 현재 Map 좌표에 해당하는 스크린 좌표를 반환
- (CGPoint)getScreenPoint:(MAVector *)pos;

// Map 이 몇도 회전하였는지를 리턴
-(float)getMapRotation;

-(void)setMapAzimuth:(float)azimuth;
-(float)getMapAzimuth;

// 현재 맵을 상하좌우로 설정된 픽셀만큼 이동
-(void)moveMapToX:(int)x andY:(int)y;

// 카메라 상하 앵글값을 세팅
-(void)setCameraAngle:(float)angle;
-(float)getCameraAngle;

// 카메라 좌우 회전값을 세팅
-(void)setMapRotation:(float)rotation;
- (void) refresh;
// 카메라 Zoom In/Out
-(void)setZoom:(float)zoom;
-(float)getZoom;

// 현재 맵의 리소스를 해제하고 초기상태로 되돌린다
-(void)clearMap;

// MapView 리턴
//-(UIView*)getMapView;


#pragma mark - POI

// 전체 POI Show/Hide
-(void)showPoi:(BOOL)show;

- (void)showPoiById:(NSArray *)Ids show:(BOOL)show;
- (void)showPoiByInfo:(NSArray *)pois show:(BOOL)show;

// 현재 사용중인 유형전체를 리턴
-(NSArray*)getPoiCategory;

// 선택된 유형에 해당하는 POI만 가시화
-(void)setShowPoiType:(NSArray *)categoryList;

// 현재 맵상의 특정 POI를 화면 중앙에 위치시킴
-(void)showSelectPoi:(int)poiId;

// 동적 POI 리스트를 세팅
-(void)setDynamicPoi:(NSArray*)pois;
-(NSArray *)listDynamicPoi;
-(void) removeDynamicPoiAll;
-(void) removeDynamicPoiById:(int)Id;
-(void) removeDynamicPoiByArray:(NSArray *)pois;
#pragma mark - NAVIGATION
// 출발지, 경유지, 목적지 POI 정보를 세팅 (화면상에 POI 그리기 위함)
-(void)setPathInfo:(NSArray*)pois;

- (void)addPath:(NSArray *)posList withColor:(UIColor *)color;
- (void)addPath:(NSArray *)posList withColor:(UIColor *)color withFloorId:(int)floorId;

// Guidance로부터 얻은 경로 및 출발지, 목적지, 경유지 POI, 그리고 층변경 아이콘을 화면에 그림 (이미 Guidance에 경로가 획득되어 있어야 함). 층변경 아이콘 누르면 다음 경로 연결 층 로딩
-(BOOL)drawTotalPath;

// Guidance로부터 현위치를 받아 경로와 비교하여 지나간 경로는 다른색으로 표시
-(BOOL)drawPassPass;

// 현재 그려진 경로 및 관련 아이콘들을 화면에서 제거
-(void)removePath;

// 경로 라인의 색상을 변경
-(void)setPathColor:(float)red green:(float)green blue:(float)blue;

// 지나간 경로 라인의 색상을 변경
-(void)setPassPathColor:(float)red green:(float)green blue:(float)blue;

// 지속적으로 내위치를 표시한다.
-(void)showMyPositionAtNavigation:(int)mode;

// 현위치 아이콘 뷰를 받아 세팅
-(void)setMyPositionIconView:(UIView*)myView;


#pragma mark - ETC

// 프로젝트에 포함된 전체 빌딩 정보 리턴
-(NSArray*)getBuildingInfo:(NSString*)zoneId;

// 빌딩에 포함된 층정보 리턴
-(NSArray*)getFloorInfo:(NSString*)bldgId;

// 맵의 배경색을 변경한다
-(void)setBackgroundColorWithRed:(float)red green:(float)green blue:(float)blue;

#warning WONIL - Animation으로 이동(이동 중 사용자 입력이 막힘)
- (void)mapMoveToPx:(float)x Py:(float)y Pz:(float)z;
#warning WONIL - Animation없이 바로 이동.
- (void)mapCenterAsPx:(float)x Py:(float)y Pz:(float)z;

- (BOOL)onPoiTouched:(int)poiId x:(int)x y:(int)y;
- (BOOL)onBalloonTouched:(int)poiId;

- (NSString *)getDefaultZoneId;
- (NSString *)getDefaultBuildingId;
- (NSString *)getDefaultFloorId;
- (MAPoi *)getPoiInfoById:(int)poiId;

- (void)debugEnable:(BOOL)enable;
- (void)setPoiTextSize:(int)size;
- (void)setPoiTextColor:(UIColor *)color;
- (NSArray *)getPoiInfoListByFloorId:(NSString *)floorId;
- (NSArray *)getPoiInfoListOfCurrentFloor;
- (void)changePoiIcon:(int)poiId name:(NSString *)imageName;
- (void)restorePoiIcon:(int)poiId;
- (void)setCameraMode:(cameraMode)mode;
- (void)showPath:(BOOL)show;
- (NSArray *)getCategoryList:(NSString *)prjId;
- (NSArray *)getPoiInfoList;
- (void)changeFloorById:(NSString *)floorId;
- (NSArray *) getPoiInfoListByCategoryId:(int)categoryId;
- (void) loadMapAll:(int)buildingId;
- (void) showMapAll:(BOOL)show;
- (void) setCameraYaw:(float)fYaw PitchRatio:(float)fPitchRatio Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (void) setCameraYaw:(float)fYaw Pitch:(float)fPitch Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (void) setVerticalScale:(float)scale;
- (void) setPathVisibility:(BOOL)show withFloorId:(int)floorId exclusive:(BOOL)exclusive;
- (void) setGroundLevel:(float)groundLevel;
- (void) setHRotationAnimation:(BOOL)play step:(float)step;
- (void) brokeCurrentViewControlPan:(BOOL)panEnable zoom:(BOOL)zoomEnable rotate:(BOOL)rotateEnable;
- (void) updatePanBoundAsAABB;
//
- (void) showMyPos:(BOOL)show x:(float)x y:(float)y z:(float)z red:(float)red green:(float)green blue:(float)blue alpha:(float)alpha rad:(float)rad;
- (void) showDynamicPoi:(BOOL)show x:(float)x y:(float)y z:(float)z;
- (void) initPoiList:(NSArray *)list;

@end
