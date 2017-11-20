#import "Interfaces.hpp"
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@protocol GLViewDelegate2;

@interface GLView : UIView {
    
@private
    IApplicationEngine* m_applicationEngine;
    IRenderingEngine* m_renderingEngine;
    IResourceManager* m_resourceManager;
    EAGLContext* m_context;
    float m_timestamp;
    float m_last_timestamp;
    unsigned short  m_framecount_from_lastStamp;
    
    CGRect mFrame ;
    CAEAGLLayer* mEaglLayer;
    
    bool m_bHoldRender;
    bool m_bUseDirectTouch;
}

@property (nonatomic, assign) id <GLViewDelegate2> delegate2;

////////////////////////////// PROJECTION
//Coordinates conversion
- (mat4*)GetViewProjectionMatrix;


/////////////////////////////// Camera
- (float)GetCameraHeight;
- (void)SetCameraYaw:(float)fYaw;

- (void)SetCameraYaw:(float)fYaw Pitch:(float)fPitch Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (vec3)GetCameraPosition;
- (float)GetCameraYaw;
- (float)GetCameraPitch;
- (void)SetCameraPitch:(float)fPitch;

- (float)GetCameraOffset;
- (void)SetCameraOffset:(float)fOffset;

- (void)GetOrbitCenter:(float*)pCenter;


- (void)SetCameraYaw:(float)fYaw PitchRatio:(float)fPitchRatio Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (void)SetOrthoCamera:(VBOrthoCameraDirection)mode WithPosX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset;
- (bool)SetCameraOnClosestPathFromX:(float)fX
                                  Y:(float)fY
                            AtFloor:(int)nFloor              //몇 층의 경로인지(현재는 한 츠엥 하나의 경로만 존재
                             FromVt:(int)nIndexOfFromVt      //입력 된 경로 중 몇번째 index의 vt로부터 진행 된 위치인지
                           Progress:(float)fProgress         //해당 Vt에서 다음 Vt까지 어느 정도 진행했는지
                     WithPitchRatio:(float)fPitchRatio       //카메라 수직 방향은 몇 %로 하는지
                        AndDistance:(float)fDistance         //카메라 거리는 얼마로 하는지
                       FitDirection:(bool)bFitDir;           //진행 방향으로 맞출 것인지 여부

- (bool)SetCameraOnPathFromX:(float)fX
                           Y:(float)fY
                     AtFloor:(int)nFloor              //몇 층의 경로인지(현재는 한 츠엥 하나의 경로만 존재
                FitDirection:(bool)bFitDir;           //진행 방향으로 맞출 것인지 여부


-(void)SetCameraToSee3DPointPx : (float)fPosX Py : (float)fPosY Pz:(float)fPosZ;
-(void)SetCameraToSee2DPointPx : (float)fPosX Py : (float)fPosY;

-(vec3)GetGroundPositionOnScreenX : (int)screenX Y : (int)screenY;
-(void)FitAABB: (bool)bUpdateAABB;
-(void)FitAABB2VisibleGroup: (bool)bBoundMinMaxDistance;
-(void)SetCameraPostion2VisibleGroupCenter:(float)fCameraDistance;
    
-(void)SetCameraMin:(float)fMinOffset MaxOffset:(float)fMaxOffset;

-(void)SetFixedFPSCamera:(float)fX2D :(float)fY2D :(float)fEyeHeight :(float)fYaw :(bool)bAnimate;

//Temp
/*
-(void)didRotate : (NSNotification*)notification;
*/

/*
 vbViewControlSetDefaultLBS  =0,             //PISA
 vbViewControlSetYawPitch    =1,             //IIAC
 vbViewControlSetStandardI   =2,             //STD1
 vbViewControlSetGongVueTypeA=3,             //GongVue Type A
 vbViewControlSetZoneView    =4,             //Zone view - Yaw only
 vbViewControlSetPalladiON   =5,             //PISA default에서 수직 제어 없애고 수평 연속 회전 되도록 함.
 vbViewControlSetPlanar3D    =6,             //2touch planar rotation
 vbViewControlSetFixedFPS    =7              //임의 좌표에서 임의 방향을 바라 볼 수 있도록 함(터치로는 제어 안됨)
*/

-(void)SetViewControlSet : (unsigned char)view_control_set;

//제스쳐
//Device direction
-(void)SetDeviceDirection : (UIDeviceOrientation)direction WithAnimation:(bool)bAnimate;

//Touch
- (void)UseDirectTouch : (bool)bUseDirectTouch;

- (void)exTouchesMoved: (NSSet*) touches withEvent: (UIEvent*) event;
- (void)exTouchesEnded: (NSSet*) touches withEvent: (UIEvent*) event;
- (void)exTouchesBegan: (NSSet*) touches withEvent: (UIEvent*) event;

//터치 이벤트 처리 중인 것을 모두 종료 시킨다.(롱탭 등 처리)
- (void)InvalidateManipulation;

//초기화
-(id) initWithFrame: (CGRect) frame withDelegate2:(id<GLViewDelegate2>)delegate2;
-(void)InitApplication;
-(void)clearData;  //주요 데이터 삭제
-(void)releaseAll;  //데이터 삭제 및 모든 리소스 해제

//메쉬
-(void)SetMapPath : (NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithSlash;
-(VBMapErrorCode)LoadMeshData:(bool)bAsStatic;  //Static = Instance에 사용되지 않고 항상 보이는 메쉬

-(VBMapErrorCode)LoadMap:(NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash;
-(VBMapErrorCode)ImportMap:(NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash;

-(VBMapErrorCode)ImportObjectMesh:(NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash;

//Instance
-(VBMapErrorCode)ImportObjectInstance:(NSString *)strFullFilePath;
-(VBMapErrorCode)AddMeshInstanceofSBM:(NSString*)sbm_name_without_extension
                                 Mesh:(unsigned short)mesh_ID
                                   At:(const float*)transform_mat;


//Group
//group_name이 포함 된 Group이 있는지 확인한다.
/*
 input
    group_name : 확인 하고자 하는 그룹 이름
 output
    group_name이 포함 된 첫번째 그룹의 이름
    없으면 nil
 */
- (NSString*)IsExistingGroupContaining:(NSString *)group_name;
//각 그룹의 Show/Hide 설정(단, 메쉬까지 적용을 하는 것을 별도임.) 그룹 이름이  nil이면 전체.
- (void)SetVisibilityFlag:(bool)bVisible WithGroupName:(NSString*)group_name;
//Visibility flag를 각 메쉬에 적용한다.
- (void)ApplyVisibilityFlagsToMesh;


//Runtime configuration
//DO NOT MODIFY bUseVBO on RENDERING. IF YOU WANT TO CHANGE THIS, USE AS 'CLEAR-CHANGE-LOAD'
-(void)setRuntimeConfig :(RunTimeConfig*)pConfig;
-(RunTimeConfig)getRuntimeConfig;

-(void)EnableDebugLog:(bool)bEnableDebugLog;



//내외부 연결
//Path
-(void)removePath; // 전체 경로를 삭제함.
//개별 경로를 추가함
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth :(bool)bArrowTip;///*Will be deprecated*/
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth :(bool)bArrowTip WithGroupName:(NSString*)grp_name;
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth :(bool)bArrowTip WithFloorID:(int)floorID;

-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth Mode:(unsigned short)nMode/*0=Solid,1=Triangls*/ Param:(const float*)param WithArrowTip:(bool)bArrowTip;
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth Mode:(unsigned short)nMode/*0=Solid,1=Triangls*/ Param:(const float*)param WithArrowTip:(bool)bArrowTip WithGroupName:(NSString*)grp_name;
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth Mode:(unsigned short)nMode/*0=Solid,1=Triangls*/ Param:(const float*)param WithArrowTip:(bool)bArrowTip WithFloorID:(int)floorID;

//경로의 Visibility 설정
-(void)SetPathVisibility:(bool)bShow WithGroupName:(NSString*)group_name :(bool)bExclusive;
-(void)SetPathVisibility:(bool)bShow WithFloorID:(int)floorID :(bool)bExclusive;

//수직 Offset 값을 결정한다
-(void)SetGroundLevel:(float)fGroundLevel;




//기존 데이터를 다 삭제하고, 건물 전체의 경로를 입력하면 층별로 분리해서 여러개의 경로를 만들어줌.
//WILL BE DEPRECATED
-(void)SetPathDataPts :(float*)pts Num:(unsigned short)ptNum Dist:(float)total_dist :(bool)bArrowTip;
-(void)SetPathData2DPts :(float*)pts2D Floor:(int*)floors Num:(unsigned short)ptNum :(bool)bArrowTip;
//
-(void)SetVisiblePathAtFloor : (int)nFloor;/*deprecated. Use SetPathVisibility.....*/
-(void)UpdateVisiblePathAtX :(float)px2D Y:(float)py2D AtFloor:(int)nFloor;
-(void)SetMyPos2DOriginalX:(float)oX Y:(float)oY AndCorrectedX:(float)cX Y:(float)cY;
-(void)ShowMyPos:(bool)bShow;

//-(void)showPath : (int) pathID;
-(void)showPath :(bool)bShow;
-(void)setCameraPitchRatio : (float)pitch_ratio;
-(void)setHRotationRange : (float)fVerticalRatioFromBottom;

-(void)setHRotationAnimation:(bool)bStart WithYawStep:(float)fYawStep;

//-(void)setCurrentPos : (CGPoint) myPosition2D;

//랜더링
- (void) drawView: (CADisplayLink*) displayLink;
- (float) getPassedTimeFromLastTimeStamp:(unsigned short&)passedframecount;
//- (void) holdRender;
- (void) pauseRender;
- (void) resumeRender;
-(void)SetAntiAliasingMode : (AntiAliasingMode)eAAMode;
- (void) setSolidRendering:(bool)bOn;/*랜더링 모드가 늘어났으므로 가급적 아래 API를 사용할 것*/
- (void) setRenderMode : (vbGLRenderMode)eRenderMode;
- (void) setVerticalScale:(float)fVScale;

//Picking
- (int) getPickedMeshID:(int) tx :(int)ty ;
- (bool) IsPickingMode;
- (void) setPickingMode:(bool)bPickingOn;

//2D
//Polygon의 이름, 색상은 16진수로, 외곽선 폴리곤의 2차원 좌표 배열
- (void)AddPolygon2D:(NSString*)strName  :(unsigned char*)rgba255 :(NSArray*)pts;
//마지막에 추가 한 폴리곤에 구멍을 내기 위한 폴리곤 추가
- (VBMapErrorCode)AddHoleToLastPolygon:(NSArray*)pts;
//추가 된 폴리곤들을 Mesh로 변환하고 폴리곤 정보 삭제
- (void)ConvertPolygonToMesh;

-(void)Prerelease;
-(UIImage*)GetScreenShot;
@end




