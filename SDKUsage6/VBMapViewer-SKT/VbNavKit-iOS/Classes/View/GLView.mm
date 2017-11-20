#import "GLView.h"

#import "vbPOIManager.h"
#import "Projection.h"
#import "GLViewDelegate2.h"

#ifndef GL_RENDERBUFFER
#define GL_RENDERBUFFER 0x8d41
#endif

#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

#ifndef GL_UNSIGNED_BYTE
#define GL_UNSIGNED_BYTE 0x1401
#endif

@implementation GLView
{
    CADisplayLink* displayLink;
}

//@synthesize delegate;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (BOOL) isMultipleTouchEnabled
{
    return YES;
}

- (void) pauseRender
{
    m_bHoldRender = true;
    [EAGLContext setCurrentContext:nil];
}

- (void) resumeRender
{
    m_bHoldRender = false;
    [EAGLContext setCurrentContext:m_context];
}

-(void)Prerelease
{
    [displayLink invalidate];
}

-(void)releaseAll  //데이터 삭제 및 모든 리소스 해제
{
    bool bClearRender=true;
    bool bClearMesh=true;
    bool bClearMaterial=true;
    bool bClearView=true;
    bool bClearPath=true;          //Path유지
    
    [EAGLContext setCurrentContext:m_context];
    
    m_applicationEngine->ClearData(bClearRender,  bClearMesh,  bClearMaterial,  bClearView,  bClearPath);
    m_renderingEngine->DeleteBaseFBOAndVBO(); //RTT, FBO(Render Buffer + Frame Buffer), VBO(Offscreen)
    
    [EAGLContext setCurrentContext:nil];
    
    if(m_applicationEngine)
    {
        delete m_applicationEngine;
        m_applicationEngine = NULL;
    }
    
    if(m_renderingEngine)
    {
        delete m_renderingEngine;
        m_renderingEngine=NULL;
    }
    
    if(m_resourceManager)
    {
        delete m_resourceManager;
        m_resourceManager=NULL;
    }
    
    [m_context release];
    m_context=nil;
    
    self.delegate2=nil;
    
    displayLink = nil;
   
}

-(void)dealloc
{
    [EAGLContext setCurrentContext:m_context];
    
    
    [self clearData];                       //Texture, VBO
    
    if (m_renderingEngine) {
        m_renderingEngine->DeleteBaseFBOAndVBO(); //RTT, FBO(Render Buffer + Frame Buffer), VBO(Offscreen)
    }
    
    [EAGLContext setCurrentContext:nil];
    
    if(m_applicationEngine)
    {
        delete m_applicationEngine;
        m_applicationEngine = NULL;
    }
    
    if(m_renderingEngine)
    {
        delete m_renderingEngine;
        m_renderingEngine=NULL;
    }
    
    if(m_resourceManager)
    {
        delete m_resourceManager;
        m_resourceManager=NULL;
    }
    
    [m_context release];
    m_context=nil;
    
    self.delegate2=nil;
    
    displayLink = nil;
    
	NSLog(@"GLView dealloc Called!!");
    
	[super dealloc];
}


// Hunjuly - 아래 코드들이 왜 들어갔는지 아무도 모른다.
// 일단 삭제하기는 했으나 인천공항 GUI에 영향을 줄 수 있다.
// 2014.01.01 까지 이 코드를 보관한다.
//- (void)setFrame:(CGRect)newRect
//{
//}
//
//- (void)willMoveToSuperview:(UIView *)newSuperview
//{
//    CGRect newFrame = newSuperview.frame;
//    [super setFrame:newFrame];
//}

-(void)SetDeviceDirection : (UIDeviceOrientation)direction WithAnimation:(bool)bAnimate
{
    if(!m_applicationEngine)
    {
        NSLog(@"[WATCH!!!]Application instance is destroyed.");
        return;
    }
    
    m_applicationEngine->SetDeviceDirection((DeviceDirection)direction, bAnimate);
}

- (id) initWithFrame: (CGRect) frame withDelegate2:(id<GLViewDelegate2>)delegate2
{
    if ((self = [super initWithFrame:frame]) != nil)//비교가 아니고 대입이 맞다.
    {
        self.delegate2=delegate2;
        
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.drawableProperties = @{
                                         kEAGLDrawablePropertyRetainedBacking: [NSNumber numberWithBool:YES],
                                         kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8
                                         };

        eaglLayer.opaque = YES;
        
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
        float scale = [[UIScreen mainScreen] scale];
        //eaglLayer.contentsScale = [[UIScreen mainScreen] scale];
        
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES1;
		m_context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!m_context) {
            api = kEAGLRenderingAPIOpenGLES1;
            m_context = [[EAGLContext alloc] initWithAPI:api];
        }
        
        if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            [self release];
            return nil;
        }
        
        m_resourceManager = CreateResourceManager();
        
        if (api == kEAGLRenderingAPIOpenGLES1) {
            //Temp-NSLog(@"Using OpenGL ES 1.1");
            m_renderingEngine = ES1::CreateRenderingEngine(m_resourceManager);
        } else {
            //Temp-NSLog(@"Using OpenGL ES 2.0");
            m_renderingEngine = ES2::CreateRenderingEngine(m_resourceManager);
        }
        
        m_renderingEngine->SetScreenScale(scale);
        
        m_applicationEngine = CreateApplicationEngine(m_renderingEngine);
        
        mFrame = frame;
        mEaglLayer = eaglLayer;

        
        [self InitApplication];
        
        
        [EAGLContext setCurrentContext:nil];
        
        //[self SetViewControlSet:1];
        Boolean bTest = NO;
        if (bTest==NO)
        {
            // Gesture Setting
            [self SetViewControlSet:0];
        }
        else
        {
            NSLog(@"[[[GLView-TEST CONDITION]]] GLView is configured for TEST condition. If this version is officially released version, please ask wether GLView is ready to release or not.");
            
            [self SetViewControlSet:3];
        }
        
    }
    
    return self;
}

/*
 -(void)didRotate : (NSNotification*)notification
 {
 UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
 
 [self SetDeviceDirection:orientation WithAnimation:false];
 }
 */
- (void)GetOrbitCenter:(float*)pCenter
{
    vec3 pos =  m_applicationEngine->GetOrbitCenter();
    pCenter[0] = pos.x;
    pCenter[1] = -pos.z;
    
}

//터치 이벤트 처리 중인 것을 모두 종료 시킨다.(롱탭 등 처리)
- (void)InvalidateManipulation
{
    m_applicationEngine->InvalidateManipulation();
}

-(vec3)GetGroundPositionOnScreenX : (int)screenX Y : (int)screenY
{
    return m_applicationEngine->GetGroundPositionOnScreenXY(ivec2(screenX,screenY));
}

-(void)FitAABB: (bool)bUpdateAABB
{
    m_applicationEngine->FitAABBtoScreen(bUpdateAABB);
}

-(void)FitAABB2VisibleGroup: (bool)bBoundMinMaxDistance
{
    m_applicationEngine->FitAABB2VisibleGroup(bBoundMinMaxDistance);
}

-(void)SetCameraPostion2VisibleGroupCenter:(float)fCameraDistance
{
    m_applicationEngine->SetCameraPostion2VisibleGroupCenter(fCameraDistance);
}

//XY평면 기준의 좌표는  XZ기준으로 변환되어야 한다.
-(void)SetCameraToSee3DPointPx : (float)fPosX Py : (float)fPosY Pz:(float)fPosZ
{
    vec3 new_cntr = vec3(fPosX,fPosY,fPosZ);
    m_applicationEngine->SetCameraToSee3DPoint(new_cntr.x,new_cntr.y,new_cntr.z);
}

//XY평면 기준의 좌표는  XZ기준으로 변환되어야 한다. - 애니매이션 없이 감.
-(void)SetCameraCenterAsPx : (float)fPosX Py : (float)fPosY Pz:(float)fPosZ
{
    float camera_param[6];
    m_applicationEngine->GetCemeraParam(camera_param);//Yaw-Pitch-cx-cy-cz-offset
    //float cx, float cy, float cz, float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,float fDist
    m_applicationEngine->SetCamera(fPosX, fPosY, fPosZ, camera_param[0], camera_param[1], camera_param[5]);
}

//XY평면 기준의 좌표는  XZ기준으로 변환되어야 한다.
-(void)SetCameraToSee2DPointPx : (float)fPosX Py : (float)fPosY
{
    vec3 new_cntr = vec3(fPosX,0.f,-fPosY);
    m_applicationEngine->SetCameraToSee3DPoint(new_cntr.x,new_cntr.y,new_cntr.z);
}

-(void)setCameraPitchRatio : (float)pitch_ratio
{
    m_applicationEngine->SetCameraPitchRatio(pitch_ratio);
}

-(float)GetCameraPitchRatio {
    return m_applicationEngine->GetPitchRatio();
}

//Path
-(void)removePath
{
    m_applicationEngine->SetPathData(NULL,0,-1,0,false);//다 삭제하는 조건
}

//개별 경로를 추가함
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth :(bool)bArrowTip/*Will be deprecated*/
{
    m_applicationEngine->AddPathData(pts, ptNum, fRGBA, fPathWidth, bArrowTip, 0, 0);//Solid Path
}
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth :(bool)bArrowTip WithGroupName:(NSString*)grp_name
{
    const char* str_group_name = [grp_name UTF8String];
    std::string group_name = std::string(str_group_name);
    m_applicationEngine->AddPathDataWithGroupName(group_name,pts, ptNum, fRGBA, fPathWidth, bArrowTip, 0, 0);//Solid Path
}
-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth :(bool)bArrowTip WithFloorID:(int)floorID
{
    m_applicationEngine->AddPathDataWithFloorID(floorID,pts, ptNum, fRGBA, fPathWidth, bArrowTip, 0, 0);//Solid Path
    
}




-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth Mode:(unsigned short)nMode Param:(const float*)param WithArrowTip:(bool)bArrowTip
{

    if (nMode>1) {
        NSLog(@"Bad Path type = %d. [0=Solid, 1=Triangles]",nMode);
        return;
    }

    m_applicationEngine->AddPathData(pts, ptNum, fRGBA, fPathWidth, bArrowTip, nMode, param);

}

-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth Mode:(unsigned short)nMode/*0=Solid,1=Triangls*/ Param:(const float*)param WithArrowTip:(bool)bArrowTip WithGroupName:(NSString*)grp_name
{
    if (nMode>1) {
        NSLog(@"Bad Path type = %d. [0=Solid, 1=Triangles]",nMode);
        return;
    }
    const char* str_group_name = [grp_name UTF8String];
    std::string group_name = std::string(str_group_name);
   
    m_applicationEngine->AddPathDataWithGroupName(group_name, pts, ptNum, fRGBA, fPathWidth, bArrowTip, nMode, param);
   
}

-(void)AddPathData3DPts :(float*)pts Num:(unsigned short)ptNum Color:(float*)fRGBA Width:(float)fPathWidth Mode:(unsigned short)nMode/*0=Solid,1=Triangls*/ Param:(const float*)param WithArrowTip:(bool)bArrowTip WithFloorID:(int)floorID
{
    m_applicationEngine->AddPathDataWithFloorID(floorID,pts, ptNum, fRGBA, fPathWidth, bArrowTip, nMode, param);
}




-(void)SetPathDataPts :(float*)pts Num:(unsigned short)ptNum Dist:(float)total_dist :(bool)bArrowTip
{
    m_applicationEngine->SetPathData(pts, ptNum, total_dist,0,bArrowTip);
}

-(void)SetPathData2DPts :(float*)pts2D Floor:(int*)floors Num:(unsigned short)ptNum :(bool)bArrowTip
{
    m_applicationEngine->SetPathData2D(pts2D, floors, ptNum, bArrowTip);
}

-(void)SetVisiblePathAtFloor : (int)nFloor/*deprecated. Use SetPathVisibility.....*/
{
    m_applicationEngine->showPathAtFloor(nFloor);
}

//경로의 Visibility 설정
-(void)SetPathVisibility:(bool)bShow WithGroupName:(NSString*)group_name :(bool)bExclusive
{
    const char* grp_name = [group_name UTF8String];
    std::string target_group = std::string(grp_name);
    m_applicationEngine->SetPathVisibilityWithGroupName(grp_name, bShow, bExclusive);
}

-(void)SetPathVisibility:(bool)bShow WithFloorID:(int)floorID :(bool)bExclusive
{
    m_applicationEngine->SetPathVisibilityWithFloorID(floorID, bShow, bExclusive);
}


//-(void)showPath : (int) pathID
-(void)showPath : (bool)bShow
{
    m_applicationEngine->showPath(bShow);
}

-(void)UpdateVisiblePathAtX :(float)px2D Y:(float)py2D AtFloor:(int)nFloor
{
    m_applicationEngine->UpdateVisiblePath(px2D, py2D, nFloor);
}

-(void)SetMapPath : (NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash;
{
    if (m_applicationEngine==NULL)
    {
        NSLog(@"[WATCH!!]Initailization wasn't improper or m_application is removed.");
        return;
    }
    
    //SBM path
    //NSString to char*
    const char* full_file_path = [strFullFilePath UTF8String];
//    int length_KOR = (int)[strFullFilePath lengthOfBytesUsingEncoding:-2147481280] ;//EUC-KR
    int length_KOR = (int)strlen(full_file_path);
    

    m_applicationEngine->SetMeshPath(length_KOR+1, full_file_path); //+1 is for \n
    
    //Texture path
    //    const char* full_tex_path = [[strTextureDirectoryPathWithSlash stringByAppendingString:@"/"] UTF8String];
    const char* full_tex_path = [strTextureDirectoryPathWithoutSlash UTF8String];
//    length_KOR = (int)[strTextureDirectoryPathWithoutSlash lengthOfBytesUsingEncoding:-2147481280];//EUC-KR
    length_KOR = (int)strlen(full_tex_path);
   
    m_applicationEngine->SetTexturePath(length_KOR+2, full_tex_path); //+2 for slash+null
}

-(void)clearData
{
    //인자가 없으면 기본값으로 모두 다 초기화 하도록 하고 있다.
    //void ClearData(bool bClearRender=true, bool bClearMesh=true, bool bClearMaterial=true, bool bClearView=true, bool bClearPath=true) = 0;
    bool bClearRender=true;
    bool bClearMesh=true;
    bool bClearMaterial=true;
    bool bClearView=true;
    bool bClearPath=true;          //Path유지
    
    [EAGLContext setCurrentContext:m_context];
    if  (m_applicationEngine) m_applicationEngine->ClearData(bClearRender,  bClearMesh,  bClearMaterial,  bClearView,  bClearPath);
    if (m_renderingEngine)    m_renderingEngine->ClearData();
    
    [EAGLContext setCurrentContext:nil];
}

-(void)InitApplication {
    if(!m_applicationEngine->IsInitialted() && m_context) {
        
        m_bHoldRender = false;
        m_bUseDirectTouch = true;
        m_bBrokeTouch = false;
        
        [m_context
         renderbufferStorage:GL_RENDERBUFFER
         fromDrawable: mEaglLayer];
        
        int width = CGRectGetWidth(mFrame);
        int height = CGRectGetHeight(mFrame);
        
        m_applicationEngine->Initialize(width, height); //경로는 미리 설정되어 있어야 한다.
        
        [self drawView: nil];
        m_last_timestamp = m_timestamp = CACurrentMediaTime();
        m_framecount_from_lastStamp = 0;
        
        displayLink = [CADisplayLink displayLinkWithTarget:self
                                                  selector:@selector(drawView:)];
        
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
}

//경로가 사전에 설정되어 있어야 한다.
-(VBMapErrorCode)LoadMeshData:(bool)bAsStatic
{
    [EAGLContext setCurrentContext:m_context];
    
    if([EAGLContext currentContext]==nil)
    {
        NSLog(@"+++++++++++++++++++++++++++++++++OPENGL ISN'T READY YET.");
        return VBMapError_INVALID_CONTEXT;
    }
    VBMapErrorCode result = m_applicationEngine->LoadMesh(bAsStatic);
    
    [EAGLContext setCurrentContext:nil];
    //두 부분으로 나눈 것은 데이터 로딩 후 뷰로 전환하기 위해서임.
    
    return result;
}


//Instance
-(VBMapErrorCode)ImportObjectInstance:(NSString *)strFullFilePath
{
    const char* full_file_path = [strFullFilePath UTF8String];
    VBMapErrorCode result = m_applicationEngine->ImportInstance(full_file_path);
    
    return result;
}


//Instance
-(VBMapErrorCode)AddMeshInstanceofSBM:(NSString*)sbm_name_without_extension
                                 Mesh:(unsigned short)mesh_ID
                                   At:(const float*)transform_mat
{
    const char* strSBMName = [sbm_name_without_extension UTF8String];
    
    VBMapErrorCode result = VBMapError_NO_ERROR;
    int add_result = m_applicationEngine->AddInstance(strSBMName, mesh_ID, transform_mat);

    if (add_result<0) result = VBMapError_BAD_INPUT_PARAMETER;

    return result;
}


- (float) getPassedTimeFromLastTimeStamp:(unsigned short&)passedframecount;
{
    //경과시간
    float elapsed_time = m_timestamp - m_last_timestamp;
    passedframecount = m_framecount_from_lastStamp;
    
    //마지막 확인 시간 갱신
    m_last_timestamp = m_timestamp;
    m_framecount_from_lastStamp = 0;
    
    //경과시간 반환
    return elapsed_time;
}

- (void) drawView: (CADisplayLink*) aDisplayLink
{
    if(m_bHoldRender || m_context==nil)   return;
    
    [EAGLContext setCurrentContext:m_context];
    
    if (aDisplayLink != nil) {
        float elapsedSeconds = aDisplayLink.timestamp - m_timestamp;
        m_timestamp = aDisplayLink.timestamp;
        
        m_framecount_from_lastStamp++;
        if(m_framecount_from_lastStamp>=65535)
            m_framecount_from_lastStamp = 0;
        
        m_applicationEngine->UpdateAnimation(elapsedSeconds);
    }
    
    if( m_applicationEngine->IsViewUpdated() == true )
    {
        Projection* projection=[[Projection alloc]initMatrix:m_applicationEngine->GetViewProjectionMatrix()
                                            withCameraHeight:[self GetCameraHeight]
                                ];
        
        [self.delegate2 onGLViewUpdated:projection cameraYaw:[self GetCameraYaw] cameraPitch:[self GetCameraPitchRatio] cameraHeight:[self GetCameraHeight] cameraOffset:[self GetCameraOffset]];
        
        [projection release];
        m_applicationEngine->SetViewUpdated(false);
        
    }
    
    m_applicationEngine->Render();
    
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
    
    [EAGLContext setCurrentContext:nil];
}

-(void)SetAntiAliasingMode : (AntiAliasingMode)eAAMode
{
    m_renderingEngine->SetAntiAliasingMode(eAAMode);
}

- (void) setSolidRendering:(bool)bOn
{
    m_renderingEngine->SetSolidRendering(bOn);
}

- (void) setRenderMode:(vbGLRenderMode)eRenderMode
{
    m_renderingEngine->SetRenderingMode(eRenderMode);
}

- (void) setVerticalScale:(float)fVScale
{
    m_renderingEngine->SetVerticalScale(fVScale);
}

-(VBMapErrorCode)ImportMap:(NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash
{
    [self pauseRender];
    NSDate *date1 = [NSDate date];
    
    [self SetMapPath:strFullFilePath withTexturePath:strTextureDirectoryPathWithoutSlash];
    VBMapErrorCode result = [self LoadMeshData:true];
    
    NSDate *date2 = [NSDate date];
    NSLog(@"======================");
    NSLog(@"ImportMap Interval : %f %@", [date2 timeIntervalSinceDate:date1], strFullFilePath);
    
    [self resumeRender];
    
    return result;
}


-(VBMapErrorCode)ImportObjectMesh:(NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash
{
    [self pauseRender];
    
    [self SetMapPath:strFullFilePath withTexturePath:strTextureDirectoryPathWithoutSlash];
    VBMapErrorCode result = [self LoadMeshData:false];
    
    [self resumeRender];
    return result;
}


-(VBMapErrorCode)LoadMap:(NSString *)strFullFilePath withTexturePath:(NSString *)strTextureDirectoryPathWithoutSlash
{
    [self pauseRender];
    [self clearData];   //Path는 유지 됨.
    
    VBMapErrorCode result = [self ImportMap:strFullFilePath withTexturePath:strTextureDirectoryPathWithoutSlash];
    
    [self resumeRender];
    
    return result;
}

-(void)SetViewControlSet : (unsigned char)view_control_set
{
    m_applicationEngine->SetViewControlSet((vbViewInfo::vbViewControlSet)view_control_set);
}
//Touch
- (void)UseDirectTouch : (bool)bUseDirectTouch
{
    m_bUseDirectTouch = bUseDirectTouch;
}

//Touch - vbViewControlSetPlanar3D에 대해서만 일단 구현 되어 있음.
- (void)BrokeCurrentViewControlPan:(bool)bBrokePan Zoom:(bool)bBrokeZoom Rotate:(bool)bBrokeRotate
{
    m_applicationEngine->BrokeControl(bBrokePan, bBrokeZoom, bBrokeRotate);
}

-(bool)IsOnHRotatingAnimation
{
    return m_applicationEngine->IsOnHRotatingAnimation();
}

- (void)exTouchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
    if (m_bBrokeTouch)  return;
    
 //   if(m_applicationEngine->IsPickingMode())    return;
    
    //get all touches
    NSSet* allTouches = [event allTouches];
    
    switch ([allTouches count])
    {
        case 1://Single Touch
        {
            UITouch* touch = [touches anyObject];
            CGPoint current = [touch locationInView: self];
            m_applicationEngine->OnSingleFingerMove(current.x, current.y,(int)[touch tapCount]);
        }
            break;
        case 2:
        {
            UITouch* touch1 = [[allTouches allObjects] objectAtIndex:0];    //1st touch
            CGPoint pos1  = [touch1 locationInView: self];
            int tapCount1 = (int)[touch1 tapCount];
            
            UITouch* touch2 = [[allTouches allObjects] objectAtIndex:1];    //2nd touch
            CGPoint pos2  = [touch2 locationInView: self];
            int tapCount2 = (int)[touch2 tapCount];
            
            m_applicationEngine->OnDoubleFingerMove(pos1.x,pos1.y, tapCount1, pos2.x,pos2.y, tapCount2);
            
        }
            break;
    }
}
- (void)exTouchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
    if (m_bBrokeTouch)  return;
    
    //get all touches
    NSSet* allTouches = [event allTouches];
    
    switch ([allTouches count])
    {
        case 1://Single Touch
        {
            
            UITouch* touch = [touches anyObject];
            CGPoint location  = [touch locationInView: self];
            
            if(m_applicationEngine->IsPickingMode())
            {
                int meshID = [self getPickedMeshID:location.x :location.y];
                
                if(meshID>=0)
                {
                    [self.delegate2 onMeshPicked:(unsigned short)meshID];
                    return;
                }
            }

            m_applicationEngine->OnSingleFingerUp(location.x, location.y,(int)[touch tapCount]);
        }
        break;
            
        case 2:
        {
             UITouch* touch1 = [[allTouches allObjects] objectAtIndex:0];    //1st touch
            CGPoint pos1  = [touch1 locationInView: self];
            int tapCount1 = (int)[touch1 tapCount];
            UITouchPhase phase1 = [touch1 phase];
            
            UITouch* touch2 = [[allTouches allObjects] objectAtIndex:1];    //2nd touch
            CGPoint pos2  = [touch2 locationInView: self];
            int tapCount2 = (int)[touch2 tapCount];
            
            if(phase1==UITouchPhaseEnded)//터치 상태에 있는 점을 앞에 넘긴다.
                m_applicationEngine->OnDoubleFingerUp(pos1.x,pos1.y, tapCount1, pos2.x,pos2.y, tapCount2);
            else
                m_applicationEngine->OnDoubleFingerUp(pos2.x,pos2.y, tapCount2, pos1.x,pos1.y, tapCount1);
        }
            break;
    }
    
}


//##
-(void)SetFixedFPSCamera:(float)fX2D :(float)fY2D :(float)fEyeHeight :(float)fYaw :(bool)bAnimate
{
    //카메라 모드 설정
    m_applicationEngine->SetViewControlSet(vbViewInfo::vbViewControlSetFixedFPS);   //API로만 제어 됨.
    
    //초기 카메라 위치
    m_applicationEngine->SetCamera(fX2D, fEyeHeight, -fY2D, fYaw, 0.f, 0.1f);
    
}

- (void)exTouchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
//    if(m_applicationEngine->IsPickingMode())    return;
    if (m_bBrokeTouch)  return;
    
    //get all touches
    NSSet* allTouches = [event allTouches];
    
    switch ([allTouches count])
    {
        case 1://Single Touch
        {
            UITouch* touch = [touches anyObject];
            CGPoint location  = [touch locationInView: self];
            
            m_applicationEngine->OnSingleFingerDown(location.x, location.y,(int)[touch tapCount]);
            //[self SetFixedFPSCamera:-968.574f :-798.786f :180.f :-90.f :false];
//            -(void)SetFixedFPSCamera:(float)fX2D :(float)fY2D :(float)fEyeHeight :(float)fYaw :(bool)bAnimate
        }
            break;
            
        case 2:
        {
            UITouch* touch1 = [[allTouches allObjects] objectAtIndex:0];    //1st touch
            CGPoint pos1  = [touch1 locationInView: self];
            int tapCount1 = (int)[touch1 tapCount];
            
            UITouch* touch2 = [[allTouches allObjects] objectAtIndex:1];    //2nd touch
            CGPoint pos2  = [touch2 locationInView: self];
            int tapCount2 = (int)[touch2 tapCount];
            
            m_applicationEngine->OnDoubleFingerDown(pos1.x,pos1.y, tapCount1, pos2.x,pos2.y, tapCount2);
        }
            break;
            
        default:
            break;
    }
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
    if(!m_bUseDirectTouch)  return;
    [self exTouchesBegan:touches withEvent:event];
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
    if(!m_bUseDirectTouch)  return;
    [self exTouchesEnded:touches withEvent:event];
    
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
    if(!m_bUseDirectTouch)  return;
    [self exTouchesMoved:touches withEvent:event];
    
}

//Coordinates conversion
- (mat4*)GetViewProjectionMatrix
{
    return m_applicationEngine->GetViewProjectionMatrix();
}

- (ivec2)GetScreenSize
{
    return m_applicationEngine->GetScreenSize();
}

//Camera position
- (vec3)GetCameraPosition
{
    return m_applicationEngine->GetCameraPosition();
}

- (float)GetCameraHeight
{
    return m_applicationEngine->GetCameraPosition().y;
}

- (void)SetCameraYaw:(float)fYaw Pitch:(float)fPitch Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    m_applicationEngine->SetCamera(fX, fY, fZ, fYaw, fPitch, offset);
}

- (void)SetCameraYaw:(float)fYaw
{
    float camera_param[6];
    m_applicationEngine->GetCemeraParam(camera_param);//Yaw-Pitch-cx-cy-cz-offset
    //float cx, float cy, float cz, float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,float fDist
    m_applicationEngine->SetCamera(camera_param[2], camera_param[3], camera_param[4], fYaw, camera_param[1], camera_param[5]);
}

-(void)setHRotationAnimation:(bool)bStart WithYawStep:(float)fYawStep
{
    m_applicationEngine->SetHRotationAnimation(bStart, fYawStep);
}

- (void)SetCameraPitch:(float)fPitch
{
    float camera_param[6];
    m_applicationEngine->GetCemeraParam(camera_param);//Yaw-Pitch-cx-cy-cz-offset
    //float cx, float cy, float cz, float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,float fDist
    m_applicationEngine->SetCamera(camera_param[2], camera_param[3], camera_param[4], camera_param[0], fPitch,  camera_param[5]);
}

- (float)GetCameraYaw
{
    return m_applicationEngine->GetYawDegree();
}

- (float)GetCameraPitch
{
    return m_applicationEngine->GetPitchDegree();
}

- (float)GetCameraOffset
{
    float camera_param[6];
    m_applicationEngine->GetCemeraParam(camera_param);//Yaw-Pitch-cx-cy-cz-offset
    //float cx, float cy, float cz, float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,float fDist
    return camera_param[5];
}
- (void)SetCameraOffset:(float)fOffset
{
    float camera_param[6];
    m_applicationEngine->GetCemeraParam(camera_param);//Yaw-Pitch-cx-cy-cz-offset
    //float cx(2), float cy(3), float cz(4), float yaw_deg_from_nZ(0), float pitch_deg_from_xz_plane(1),float fDist(5)
    m_applicationEngine->SetCamera(camera_param[2], camera_param[3], camera_param[4], camera_param[0], camera_param[1], fOffset);
}

- (void)SetCameraYaw:(float)fYaw PitchRatio:(float)fPitchRatio Position3DX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    
    m_applicationEngine->SetCamera(fX, fY, fZ, fYaw, 45.f, offset);
    m_applicationEngine->SetCameraPitchRatio(fPitchRatio);
}

- (void)SetOrthoCamera:(VBOrthoCameraDirection)mode WithPosX:(float)fX Y:(float)fY Z:(float)fZ Offset:(float)offset
{
    m_applicationEngine->SetOrthoCameraWithPosOffset(mode, vec3(fX,fY,fZ), offset);
}

- (bool)SetCameraOnClosestPathFromX:(float)fX Y:(float)fY AtFloor:(int)nFloor FromVt:(int)nIndexOfFromVt Progress:(float)fProgress WithPitchRatio:(float)fPitchRatio AndDistance:(float)fDistance FitDirection:(bool)bFitDir
{
    return m_applicationEngine->SetCameraOnPath(fX,fY,nFloor,nIndexOfFromVt, fProgress, fPitchRatio, fDistance,bFitDir);
}

- (bool)SetCameraOnPathFromX:(float)fX
                           Y:(float)fY
                     AtFloor:(int)nFloor              //몇 층의 경로인지(현재는 한 츠엥 하나의 경로만 존재
                FitDirection:(bool)bFitDir           //진행 방향으로 맞출 것인지 여부

{
    return m_applicationEngine->SetCameraOnPath(fX, fY, nFloor, bFitDir);
}


-(void)SetCameraMin:(float)fMinOffset MaxOffset:(float)fMaxOffset
{
    m_applicationEngine->SetCameraMinMaxOffset(fMinOffset, fMaxOffset);
}

-(void)SetMyPos2DOriginalX:(float)oX Y:(float)oY AndCorrectedX:(float)cX Y:(float)cY
{
    m_applicationEngine->SetMyPosition(oX, 0.f, -oY, cX, 0.f, -cY);
}
-(void)ShowMyPos:(bool)bShow
{
    m_applicationEngine->ShowMyPosition(bShow);
}

-(void)ShowMyPos:(bool)bShow At:(float)px3D :(float)py3D :(float)pz3D :(float)red :(float)green :(float)blue :(float)alpha :(float)rad
{
    const float rgba[4] = { red, green, blue, alpha };
    const float pos[3] = { px3D, py3D, pz3D };
    
    m_applicationEngine->SetMyPosDiscState(bShow, rgba, pos, rad);
    
}

-(void)setHRotationRange : (float)fVerticalRatioFromBottom
{
    m_applicationEngine->SetHRotationTouchRange(fVerticalRatioFromBottom);
}

//Runtime configuration
-(void)setRuntimeConfig :(RunTimeConfig*)pConfig
{
    m_applicationEngine->SetRuntimeConfig(pConfig);
}

-(RunTimeConfig)getRuntimeConfig
{
    RunTimeConfig  config;
    m_applicationEngine->GetRuntimeConfig(&config);
    return config;
}

-(void )UpdatePanBoundAsAABB
{
    m_applicationEngine->UpdatePanBoundAsAABB();
}

-(void)UpdatePanBound:(float)minX3D :(float)minZ3D :(float)maxX3D :(float)maxZ3D
{
    m_applicationEngine->UpdatePanBound(vec3(minX3D,0.f,minZ3D), vec3(maxX3D,0.f,maxZ3D));
}


-(void)EnableDebugLog:(bool)bEnableDebugLog
{
    m_applicationEngine->EnableDebug(bEnableDebugLog);
}

-(UIImage*)GetScreenShot
{
    [EAGLContext setCurrentContext:m_context];
    
    UIImage* image=[self ScreenShot];
    [EAGLContext setCurrentContext:nil];
    
    return image;
}

-(UIImage*)ScreenShot
{
    CGSize size=self.bounds.size;
    CGFloat temp=size.height;
    size.height=size.width;
    size.width=temp;
    
    GLuint bufferLength = size.width * size.height * 4;
    GLubyte* buffer = (GLubyte*)malloc(bufferLength);
    
    //Read Pixels from OpenGL
    //glReadPixels(0, 0, size.height, size.width, GL_RGBA, GL_UNSIGNED_BYTE, buffer);  //ES1.0 ONLY
    //Make data provider with data.
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, buffer, bufferLength, NULL);
    
    //Configure image
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow = 4 * size.height;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef iref = CGImageCreate(size.height, size.width, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
    
    uint32_t* pixels = (uint32_t*)malloc(bufferLength);
    CGContextRef context = CGBitmapContextCreate(pixels, size.height, size.width, 8, size.height * 4, CGImageGetColorSpace(iref), kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    CGContextTranslateCTM(context, 0, size.width);
    CGContextScaleCTM(context, 1.0f, -1.0f);
    
    CGContextDrawImage(context, CGRectMake(0.0f, 0.0f, size.height, size.width), iref);
    
    CGImageRef myCGImage = CGBitmapContextCreateImage(context);
    UIImage *outputImage = [UIImage imageWithCGImage:myCGImage];
    
    UIImageWriteToSavedPhotosAlbum(outputImage, nil, nil, nil);
    
    CGImageRelease(myCGImage);
    
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);
    CGImageRelease(iref);
    CGContextRelease(context);
    free(buffer);
    free(pixels);
    
    return outputImage;
}

//Picking
- (int) getPickedMeshID:(int) tx :(int)ty
{
 
    //Flip y
    ty = self.bounds.size.height - ty;
    
    unsigned short usPickedMesh = -1;
    
    [EAGLContext setCurrentContext:m_context];
    if (m_applicationEngine->GetPickedMeshID(tx, ty, &usPickedMesh) == false) return -1;
    [EAGLContext setCurrentContext:nil];
    
    return usPickedMesh;
}

- (bool) IsPickingMode
{
    return m_applicationEngine->IsPickingMode();
}

- (void) setPickingMode:(bool)bPickingOn
{
    m_applicationEngine->SetPickingMode(bPickingOn);
}

//2D
//Polygon의 이름, 색상은 16진수로, 외곽선 폴리곤의 2차원 좌표 배열
- (void)AddPolygon2D:(NSString*)strName :(unsigned char*)rgba255  :(NSArray*)pts
{
    //Name
    std::string name = [strName UTF8String];
    
    //Outer ring - pts로부터 정보를 추출하여 완성
    vbRing* pNewOuter = new vbRing; //제거는 Application에서 함.
    pNewOuter->nPtCount = (int)pts.count;
    pNewOuter->pts2D = new float[pNewOuter->nPtCount * 2];
    for(int i=0; i<pNewOuter->nPtCount; i++)
    {
        //pNewOuter->pts2D[i] = (float)[pts objectAtIndex:i];
    }
    
    m_applicationEngine->AddPolygon(name, rgba255, pNewOuter);
    
    pNewOuter = 0;
    
}

//마지막에 추가 한 폴리곤에 구멍을 내기 위한 폴리곤 추가
- (VBMapErrorCode)AddHoleToLastPolygon:(NSArray*)pts
{
    //Inner ring - pts로부터 정보를 추출하여 완성
    vbRing* pNewInner = new vbRing; //제거는 Application에서 함.
    pNewInner->nPtCount = (int)pts.count;
    pNewInner->pts2D = new float[pNewInner->nPtCount * 2];
    for(int i=0; i<pNewInner->nPtCount; i++)
    {
        //pNewInner->pts2D[i] = (float)[pts objectAtIndex:i];
    }
    
    return m_applicationEngine->AddHoleToLastPolygon(pNewInner);
}

//추가 된 폴리곤들을 Mesh로 변환하고 폴리곤 정보 삭제
- (void)ConvertPolygonToMesh
{
    m_applicationEngine->ConvertPolygonToMesh();
}

/////////////////////////////// Group ////////////////////////////
- (NSString*)IsExistingGroupContaining:(NSString *)group_name
{
    const char* utf_group_name = [group_name UTF8String];
    
    std::string found_group_name = m_applicationEngine->IsExistingGroupContaining(std::string(utf_group_name));
    
    if (found_group_name==std::string("")) {
        return nil;
    }
    
    NSString* found_ns_name = [[NSString alloc] initWithUTF8String:found_group_name.c_str()];

    return found_ns_name;
}

//각 그룹의 Show/Hide 설정(단, 메쉬까지 적용을 하는 것을 별도임.) 그룹 이름이  nil이면 전체.
- (void)SetVisibilityFlag:(bool)bVisible WithGroupName:(NSString*)group_name
{
    if(group_name==nil)
        m_applicationEngine->SetVisibilityFlagWithGroupName(bVisible, std::string(""));
    else
    {
        const char* utf_group_name = [group_name UTF8String];
        m_applicationEngine->SetVisibilityFlagWithGroupName(bVisible, std::string(utf_group_name));
    }
    /*
    NSLog(@"Group name = %@", group_name);
    
    
    std::cout << "Converted name = " << utf_group_name << std::endl;
    */
}

//Visibility flag를 각 메쉬에 적용한다.
- (void)ApplyVisibilityFlagsToMesh
{
    m_applicationEngine->ApplyVisibilityFlagsToMesh();
}

- (void) SetGroundLevel:(float)fGroundLevel
{
    m_applicationEngine->SetGroundLevel(fGroundLevel);
}

@end



