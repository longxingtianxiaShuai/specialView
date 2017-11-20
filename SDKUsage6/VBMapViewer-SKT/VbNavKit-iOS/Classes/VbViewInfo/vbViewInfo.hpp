#pragma once

#include "Quaternion.hpp"
#include "vbConfiguration.hpp"
#include "vbGeometry.hpp"

#include "vbCamera.hpp"
#include "vbTrackCameraControl.hpp"
#include "vbFPSCameraControl.hpp"
#include "vbMoveOrbitZoomCameraControl.hpp"
#include "vbMovingForwardCameraControl.hpp"
#include "vbHorizontalPanCameraControl.hpp"
#include "vbFreeCameraControl.hpp"
#include "vbAnimationCameraControl.hpp"


#include "vbPerspectiveProjection.hpp"
#include "vbOrthogonalProjection.hpp"

#include <vector>

using std::vector;

class vbViewInfo
{
public:
    vbViewInfo();
    ~vbViewInfo();
    
    enum    ProjectionMode
    {
        PROJECTION_PERSPECTIVE=0,
        PROJECTION_ORTHO,
        PROJECTION_MODE_NONE
    };
    
    enum    NavigationMode
    {
        NAVIGATION_3D=0,
        NAVIGATION_2D,
        NAVIGATION_WALK,
        NAVIGATION_ME,
        NAVIGATION_2D_FIXED,        //NAVIGATION_2D는 확대 축소 이동이 되지만 이것은 고정 뷰이다.
        NAVIGATION_ENVRN            //환경맵용 고정 카메라
    };
    
    enum    NavigationMode3D
    {
        NAVIGATION_3D_NONE=0,
        NAVIGATION_3D_ORBIT_V,
        NAVIGATION_3D_ORBIT_H,
        NAVIGATION_3D_PANZOOM,
        NAVIGATION_3D_ROTATEZOOM,
        NAVIGATION_3D_PAN,
        NAVIGATION_3D_ZOOM,
        NAVIGATION_3D_FIXED_V,  //고정 상태에서 Pitch 제어(Vertical)
        NAVIGATION_3D_FIXED_H,  //고정 상태에서 Yaw 제어
        NAVIGATION_3D_ELEVATION,
        NAVIGATION_3D_FORWARD,
        NAVIGATION_3D_ZOOMHROTATION
        
    };
    
    enum    CameraAniMode
    {
        CAMERA_ANI_NONE         = 0,
        CAMERA_ANI_JUMPPAN      = 1,
        CAMERA_ANI_JUMPZOOM     = 2,
        CAMERA_ANI_JUMPPANROT   = 3,
        CAMERA_ANI_3D2WALK      = 4,
        CAMERA_ANI_3DTO2D       = 5,
        CAMERA_ANI_2DTO3D       = 6,
        CAMERA_ANI_DEVICE_ROT   = 7,
        CAMERA_ANI_FIXEDFREE    = 8,
        CAMERA_ANI_YAW_ROTATE   = 9,
    };
    

    
    enum    vbViewControlSet
    {
        vbViewControlSetDefaultLBS  =0,             //PISA
        vbViewControlSetYawPitch    =1,             //IIAC
        vbViewControlSetStandardI   =2,             //STD1
        vbViewControlSetGongVueTypeA=3,             //GongVue Type A
        vbViewControlSetZoneView    =4,             //Zone view - Yaw only
        vbViewControlSetPalladiON   =5,             //PISA default에서 수직 제어 없애고 수평 연속 회전 되도록 함.
        vbViewControlSetPlanar3D    =6,             //2touch planar rotation
        vbViewControlSetFixedFPS        =7              //임의 좌표에서 임의 방향을 바라 볼 수 있도록 함(터치로는 제어 안됨)
    };
    
    struct vbScreenControlAreaParam
    {
    /*
     0 - LControl left px offset from Left
     1 - LControl right px offset from Left
     
     2 - RControl left px offset from Right
     3 - RControl right px offset from 
     
     4 - BControl top px offset from bottom
     5 - BControl bottom px offset from bottom
     
     6 - TControl top px offset from top
     7 - TControl bottom px offset from top
     */
        unsigned short   area_param[8];
    };
    
    enum    vbViewScreenControlArea
    {
        vbViewScreenControlAreaMid      = 0,
        vbViewScreenControlAreaTop      = 1,
        vbViewScreenControlAreaLeft     = 2,
        vbViewScreenControlAreaRight    = 3,
        vbViewScreenControlAreaBottom   = 4
    };
    

    
    
protected:
    //vbConfiguration
    vbConfiguration*            m_pConf;
    vbScreenControlAreaParam    m_area_param;
    
    //GroundPlane
    vbPlane             m_ground_plane;
    
    //Pan Valid range
    float               m_pan_valid_ratio;
    
    //View Matrix
    mat4    m_view_matrix;
    
    //Projection Matrix
    mat4    m_prj_matrix;
    
    //view matrix X projection matrix - for Window/World conversion
    mat4    m_view_prj_matrix;
    bool    m_bViewUpdated;
    
    //Projection mode
    ProjectionMode  m_projection_mode;  //Projection mode
    
     
    float   m_min_near_clip;    
    float   m_rad_bounding_sphere;
    
    vec3    m_bounding_center;
    vec3    m_bound_min;
    vec3    m_bound_max;
    
    
 
 
    //Camera
    vbCamera                m_Camera;
    
    //Move
    vbFPSCameraControl              m_CameraFPSControl;
    vbMoveOrbitZoomCameraControl    m_CameraZoomControl;
    vbHorizontalPanCameraControl    m_CameraHPanControl;
    vbMovingForwardCameraControl    m_CameraForwardControl;
    
    //Rotation
    vbTrackCameraControl    m_CameraTrackControl;
    vbFreeCameraControl     m_CameraFreeControl;
    
    //Animation
    vbAnimationCameraControl    m_CameraAnimationControl;
    
    //Projection
    vbPerspectiveProjection m_CameraPerspective;
    vbOrthogonalProjection  m_CameraOrthogonal;
      
    
    float       m_cam_fMaxDistRatioBBSize;
    float       m_cam_fMaxPanRatioToBSphereRad;
  
    bool        m_cam_bJumpZoomed;
    
    
    //Environment sphere
    float       m_cam_fEnvSphereRadius;
    
   
    
    //Camera animation
    CameraAniMode m_cam_ani_mode;
   
    float       m_cam_jumpzoom_ratio;
    bool        m_cam_jumpzoom_with_jumppan;
    
    //
    bool        m_bBrokePan;
    bool        m_bBrokeZoom;
    bool        m_bBrokeRotate;
   
    //Navigation
    NavigationMode      m_cam_navi_mode;            //camera navigation mode
    NavigationMode      m_cam_navi_mode_backup;     //camera navigation mode
    NavigationMode3D    m_cam_navi_3d_mode;        //
    
    bool                m_bNavigation;
    ivec2               m_DownPos_prev;
    
    vec3                m_cam_reference3D_first;     //Reference 3d position 1
    vec3                m_cam_reference3D_second;    //Reference 3d position 2
    
    
    //3D View camera backup
    Quaternion          m_cam_3D_backup_rotation;
    vec3                m_cam_3D_backup_orbit_cntr;
    float               m_cam_3D_backup_distance;
    
    //Turn angle
    float               m_fDeviceAngle;
    vbViewDirection     m_eViewDirection;
    
    //View mode
    vbViewControlSet    m_eViewControlSet;
    float               m_fRotateAreaRatio; //화면의 아래쪽에서 몇 %까지 수평 회전 영역에 쓸 것인지 (0~1)

    
public:
    CameraAniMode       GetAniMode()        {   return m_cam_ani_mode;  };
    
    vbMoveOrbitZoomCameraControl*   GetCameraControl_OrbitZoom()    {   return &m_CameraZoomControl;    }
    vbTrackCameraControl*           GetCameraTrackControl()         {   return &m_CameraTrackControl;   }
    
    //Turn
    vbViewDirection     GetViewTurnDirection()                      {   return m_eViewDirection;    };
    void                SetViewTurnDirection(vbViewDirection dir, bool bAnimating=false);
    float               GetDeviceAngle()                            {   return m_fDeviceAngle;      };
    void                UpdateDeviceAngleDeg();
    ivec2               ConvertToRotatedDeviceCoord(ivec2 tPos);
    
    //Init
    void    InitializeViewInfo(int width, int height, vbConfiguration* pConf);
    void    ReloadConfiguration();

    //Viewport
    void    SetScreenSize(ivec2 screen_size);
    ivec2   GetScreenSize() ;
    
    
    ////////////////////////////////////// Projection  - vbViewInfoProjection.cpp
    //View matrix
    mat4*   GetViewMatrix()                                 {   return &m_view_matrix;           }
    float const*  GetViewMatrixPointer()                    {   return m_view_matrix.Pointer();  }
    void    SetViewMatrix(mat4  view_mat)                   {   m_view_matrix = view_mat;        }
    void    UpdateViewMatrix();
    
    //Projection matrix
    mat4*   GetProjectionMatrix()                           {   return &m_prj_matrix;            }
    float const*  GetProjectionMatrixPointer()              {   return m_prj_matrix.Pointer();   }
    void    SetProjectionMatrix(mat4  view_mat)             {   m_prj_matrix = view_mat;         }
    void    UpdateProjectionMatrix();
    ivec2   WorldToWindow(vec3  world_pt);
    void    WindowToWorld(ivec2 win_pos, vec3& near_pt, vec3& far_pt);
    mat4*  GetViewProjectionMatrix()                        {   return &m_view_prj_matrix;      }
    bool    IsViewUpdated()                                 {   return m_bViewUpdated;          }
    void    SetViewUpdated(bool bUpdated)                   {   m_bViewUpdated = bUpdated;      }
    
    //Projection mode
    ProjectionMode  GetProjectionMode()                     {   return m_projection_mode;        }
    void            SetProjectionMode(ProjectionMode mode)  {   m_projection_mode = mode;        }
    
    //Perspective
    void    SetPerspectiveParameters(float* perspective_param);
    void    SetPerspectiveParameters(float fovy, float aspect, float near, float far);
    const float*  GetPerspectiveParameters();

    void    UpdateNearFarClip();

public:    
    //Orthogonal
    void    SetOrthogonalParameters(float left, float right, float bottom, float top, float near, float far);
    const float*  GetOrthogonalParameters();
    ////////////////////////////////////// Projection  - vbViewInfoProjection.cpp
   
    
    ////////////////////////////////////// Navigation  - vbViewInfoNavi.cpp
    
    
    //Fit
    void    FitAABBtoScreen(bool bBoundMinMaxDist=false);
    void    FitAABBtoVertices(std::vector<vec3>&	target_vtx, bool bBoundMinMaxDist=false);
    
    
    //Ground
    void    SetGroundPlane(vbPlane ground_plane)        {   m_ground_plane = ground_plane;  }
    vbPlane GetGroundPlane()                            {   return m_ground_plane;          }
    
    float   GetPanValidRatio()              {   return  m_pan_valid_ratio;  }
    void    SetPanValidRatio(float pRatio)  {   m_pan_valid_ratio = pRatio; }
    void    SetPanValidRange(vec3 min, vec3 max, float pRatio);
    
    //Orbit
    float   GetTrackBallRatio();
    void    SetTrackBallRatio(float trRatio);
    
    void    SetOrbitCenter(vec3 orbit_center);
    vec3    GetOrbitCenter();
    
    
    void    StartFreeRotation(ivec2 finger_pos);
    bool    DoFreeRotation(ivec2 touchpoint);
   
    void    StartRotation(ivec2 finger_pos);
    void    StartHRotation(ivec2 finger_pos);
    void    StartVRotation(ivec2 finger_pos);
    void    StartYawPitchRotation(ivec2 finger_pos);
    
    bool    DoOrbit(ivec2 touchpoint);                      //싱글터치, 초기 이동 방향에 따라 수직/수평 회전
    bool    DoYawRotation(ivec2 touchpoint);                //수평 왕복 회전
    bool    DoContinuousYawRotation(ivec2 touchpoint);      //수평 연속 회전
    bool    DoPitchRotation(ivec2 touchpoint);              //수직 회전
    bool    DoYawPitchRotation(ivec2 touchpoint);           //수직/수평 회전
    bool    DoLandscapeYawPitchRotation(ivec2 touchpoint);  //VR용 수직,수평 회전
    
    
    void    EndRotation();
    void    JumpOrbit(ivec2 finger_pos);
    
    void    SetHRotationRatio(float fVertialRatioFromBottom)  { m_fRotateAreaRatio=fVertialRatioFromBottom;}
    float   GetHRotationRatio()         {   return m_fRotateAreaRatio;  }
    
    bool    StartHRotationAnimation(float fStepYaw);
    bool    EndHRotationAnimation();
    
    //Panning
    void    StartHPanning(ivec2  finger_pos);
    bool    DoHPanning(ivec2 touchpoint);
    void    EndHPanning();
    
    void    JumpPanning(ivec2 finger_pos);
    void    JumpPanning(vec3 new_orbit_cntr);
    void    JumpPanRotation(vec3 new_orbit_cntr, Quaternion new_orientation);
    

    void    UpdateValidRange(vec3 bbMin, vec3 bbMax);
    
    //Zoom
    void    StartZooming(ivec2 finger_pos);
    bool    DoZooming(ivec2 touchpoint);
    void    EndZooming();
    void    StartZooming2Pt(ivec2 finger_pos1,ivec2 finger_pos2);
    void    StartZooming2PtEnvironment(ivec2 finger_pos1,ivec2 finger_pos2);
    
    bool    DoZooming2Pt(ivec2 finger_pos1,ivec2 finger_pos2);
    bool    DoZooming2PtEnvironment(ivec2 finger_pos1,ivec2 finger_pos2);
    
    void    JumpZooming(ivec2 finger_pos);
    void    SphereJumpZooming(ivec2 finger_pos);
    
    float   GetPreviousZoomDistance();
    void    SetPreviousZoomDistance(float prevDist);
    
    //Pan & Zoom
    void    StartPanZoom(ivec2 pos1, ivec2 pos2);
    bool    DoPanZoom(ivec2 pos1, ivec2 pos2);
    void    EndPanZoom();
    
    //HRotate & Zoom
    void    StartHRotateZoom(ivec2 pos1, ivec2 pos2);
    bool    DoHRotateZoom(ivec2 pos1, ivec2 pos2);
    void    EndHRotateZoom();
    
    //Fixed Yaw & Pitch control
    void    StartFixedYawPitchRotate(ivec2 pos1);
    void    StartForwardZoom(ivec2 pos1, ivec2 pos2);
    void    StartElevation(ivec2 pos1);
    
    bool    DoFixedYawRotate(ivec2 pos1);
    bool    DoFixedPitchRotate(ivec2 pos1);
    bool    DoFixedYawPitchRotate(ivec2 pos1);
    bool    DoForwardZoom(ivec2 pos1, ivec2 pos2);
    bool    DoElevation(ivec2 pos1);
    
    void    EndFixedYawPitchRotate();
    void    EndForwardZoom();
    
    //
    void    BrokeControl(bool bBrokePan, bool bBrokeZoom, bool bBrokeRotate);

    //Navi
    void    InvalidateManipulation();
    
    //2D-3D Camera 
    void    SetTopView();
    void    ReturnTo3DView();
    
    //Walk
    float   GetEyeHeight();
    void    SetEyeHeight(float eye_h);
    
    void    InitWalk();//현재 회전 중심 위치로 내려가 걸어다닌다.
    void    StartWalking(ivec2 finger_pos);
    bool    DoWalking(ivec2 finger_pos);
    void    EndWalking();
    void    SetWalkingCamera(vec3 pos, Quaternion dir);
    
    //FPS-Manipulation
    void    StartFPS();
    void    EndFPS();
    
    //Navigation mode
    bool            IsNavigation()                          {   return m_bNavigation;       }
    void            SetNavigation(bool bNavi)               {   m_bNavigation = bNavi;      }
    NavigationMode  GetNavigationMode()                     {   return m_cam_navi_mode;     }
    bool            SetNavigationMode(NavigationMode nMode);
    void            RecoverNavigationMode();
    
    //Calculation
    vec3    MapToSphere(ivec2 touchpoint);
    vec3    GetRayFromCamera(ivec2 win_pos, vec3& fromPt);
    vec3    GetRayIntersectionWithGround(ivec2 ray_win_pos);   //Ground = XY Plane

    ////////////////////////////////////// Navigation  - vbViewInfoNavi.cpp

    
    ////////////////////////////////////// Camera  - vbViewInfoCamera.cpp
    //camera position
    
    void    SetCameraPosition(vec3 cam_pos);
    vec3    GetCameraPosition();
     
    
    void    UpdateCameraPositionFromOrbit();//Orbit center에서 지정된 distance에 지정 된 방향으로 위치
    void    UpdateCameraTargetPosition();
    void    UpdateOrbitCenterFromCamPosition();
    void    InitOrientation(bool bUpdateViewMat=true);
    
    void    SetCameraPitchRatio(float pitch_ratio);
    float   GetPitchRatio();
    float   GetPitchDegree();
    float   GetYawDegree();
    void    SetYawDegree(float fYaw);
    void    SetYawDegree(vec3 dirYaw);
    
    void    SetCamera(float cx, float cy, float cz,
                                  float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,
                                  float fDist);
    //camera distance
    float   GetCameraDistance();
    void    SetCameraDistance(float dist, bool bBoundMinMaxDistance=false);
    
    void    SetCameraMaxDistanceRatioBBSize(float ratio)    {   m_cam_fMaxDistRatioBBSize = ratio;  }
    void    SetCameraMaxPanRatioBSphereRad(float ratio)     {   m_cam_fMaxPanRatioToBSphereRad = ratio; }
    void    SetCameraMinMaxDistance(float fMinDistance, float fMaxDistance, bool bBoundMinMaxDist=false);
        
    //Zoom In/Out (Close to/ Far fromt Rot.Center)
    void    MoveToRotationCenter(bool bToCenter);
    
    //Environmental map
    void    SetEnvironmentalCamera();
    
    //camera orientation
    void    SetOrientation(Quaternion qtn);
    Quaternion  GetOrientation();       //회전 쿼터니언을 반환한다.
    vec3    GetDirection();             //카메라의 방향 벡터
    vec3    GetRight();                 
    vec3    GetUp();
    
    //Environment map
    float   GetEnvironmentSphereRadius()    {   return m_cam_fEnvSphereRadius;  }
    
    //camera fitting
    void    FullExtendVertexVector(std::vector<vec3>&	target_vtx, bool bBoundMinMaxDist=false);
    bool    GetPerspectiveFittingCameraPosition(vec3				inputCameraDirection,/*대상을 바라보고 있을 카메라의 방향. 보통 현재 카메라의 방향*/
                                                vec3				inputCameraUp,/*대상을 바라보고 있을 카메라의 Upvector.*/
                                                float				inputCameraFOV_Y_radian,/*카메라의 세로 방향 시야각*/
                                                float				inputCameraFOV_X_radian,/*카메라의 가로 방향 시야각*/
                                                std::vector<vec3>&	inputVts,/*Fitting 대상이 되는 점들*/
                                                float				inputFit_ratio,/* 1 : 화면에 꼭 맞게, 0.5 : 화면의 절반 크기에 맞게, 0.01 : 최소값*/
                                                vec3&               outputCameraPosition,/*카메라의 위치*/
                                                vec3&               outputOrbitCenter,//New orbit center
                                                bool&               outVerticalFit,/*세로방향으로 맞췄는지 여부*/   
                                                float&              outDistance,
                                                bool                bBoundMinMaxDist=false);//

    //Orthogonal mode에서 Fitting
    bool     GetOrthogonalFittingCameraPosition(vec3                inputCameraDirection/*대상을 바라보고 있을 카메라의 방향. 보통 현재 카메라의 방향*/,
                                                vec3				inputCameraUp/*대상을 바라보고 있을 카메라의 Upvector.*/,
                                                std::vector<vec3>&	inputVts/*Fitting 대상이 되는 점들*/,
                                                float               inputFit_ratio/* 1 : 화면에 꼭 맞게, 0.5 : 화면의 절반 크기에 맞게, 0.01 : 최소값*/,
                                                float               inputDistance_for_ortho/*직교투영은 원근감이 없으므로, 거리에 상관없이 동일한 화면이 나오기 때문에 거리를 지정해야 한다*/,
                                                float               inputViewAspectRatio/* Width/height*/,
                                                vec3&               outputCameraPosition/*카메라의 위치*/,
                                                float&              outputCamWidth/*카메라의 Orthogonal Width*/,
                                                float&              outputCamHeight/*카메라의 Orthogonal Height*/,
                                                bool&               outVerticalFit);/*세로방향으로 맞췄는지 여부*/
    
    void    UpdateCameraDistWithScreenCenter(bool bBoundMinMaxDist=false);
                                        
    
    
    
    
    //
    ////////////////////////////////////// Camera  - vbViewInfoCamera.cpp
    
    
    //////////////////// RANGE PARAMETER
    const vbScreenControlAreaParam*  GetScreenAreaParam()    {   return (const vbScreenControlAreaParam*)&m_area_param;   }
    void    SetScreenAreaParam(const vbScreenControlAreaParam* pParam);
    vbViewScreenControlArea IsScreenControlArea(ivec2 tPos);
 

    ////////////////////////////////////// Interaction  - vbViewInfoFinger.cpp
        //Touch interaction
    bool    SingleFingerUp(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown(ivec2 tPos, int tapCount);
    bool    SingleFingerMove(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    
    vbViewControlSet    GetViewControlSet()                                 {   return m_eViewControlSet;       }
    void                SetViewControlSet(vbViewControlSet viewControlSet)  {   m_eViewControlSet = viewControlSet;   }

    /////////////// ViewControlSet별 이벤트 처리 함수
    
    //////////// Default
    bool    SingleFingerUp_Default(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_Default(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_Default(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_Default(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_Default(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_Default(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    //////////// Yaw & Pitch
    bool    SingleFingerUp_YawPitch(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_YawPitch(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_YawPitch(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_YawPitch(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_YawPitch(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_YawPitch(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    //////////// STD1
    bool    SingleFingerUp_STD1(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_STD1(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_STD1(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_STD1(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_STD1(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_STD1(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    //////////// GongVueA
    bool    SingleFingerUp_GongVueA(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_GongVueA(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_GongVueA(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_GongVueA(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_GongVueA(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_GongVueA(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    //////////// ZoneView
    bool    SingleFingerUp_ZoneView(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_ZoneView(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_ZoneView(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_ZoneView(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_ZoneView(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_ZoneView(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    //////////// PalladiON
    bool    SingleFingerUp_PalladiON(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_PalladiON(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_PalladiON(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_PalladiON(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_PalladiON(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_PalladiON(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
    
    //////////// Planar3D
    bool    SingleFingerUp_Planar3D(ivec2 tPos, int tapCount, vec3& pos3d);
    void    SingleFingerDown_Planar3D(ivec2 tPos, int tapCount);
    bool    SingleFingerMove_Planar3D(ivec2 tPos, int tapCount);
    
    bool    DoubleFingerUp_Planar3D(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2);
    void    DoubleFingerDown_Planar3D(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    bool    DoubleFingerMove_Planar3D(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2);
    
     ////////////////////////////////////// Interaction  - vbViewInfoFinger.cpp
    
    
    /////////////////////////////////////// Animation - vbViewInfoAnimation.cpp
    void    UseCameraAnimation(bool bUseCameraAnimation);
    bool    IsCameraAnimationAvailable();
    void    UpdateAnimationFrame();
    bool    UpdateWalkingAnimation();
    
    void    InitCameraMoveAni(vec3 pos_start, vec3 pos_end, Quaternion dir_start, Quaternion dir_end, CameraAniMode aniMode);
    void    InitDeviceRotateAni(vbViewDirection eEndDirection);
    void    UpdateAnimatedCamera();
    bool    IsOnAnimating();
    
    /////////////////////////////////////// Animation - vbViewInfoAnimation.cpp
    
    
    /////////
    
};