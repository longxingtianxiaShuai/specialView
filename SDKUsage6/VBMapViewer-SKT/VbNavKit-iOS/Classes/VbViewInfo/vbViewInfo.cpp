#include "vbViewInfo.hpp"
#include <iostream>
#include <cmath>

vbViewInfo::vbViewInfo()
{
    m_CameraFPSControl.SetCamera(&m_Camera);
    m_CameraZoomControl.SetCamera(&m_Camera);
    m_CameraHPanControl.SetCamera(&m_Camera);
    m_CameraTrackControl.SetCamera(&m_Camera);
    m_CameraFreeControl.SetCamera(&m_Camera);
    m_CameraAnimationControl.SetCamera(&m_Camera);
    m_CameraForwardControl.SetCamera(&m_Camera);
    
    //Camera
    m_Camera.SetYawDegree(0.f);
    m_Camera.SetPitchDegree(0.f);
    
    //이곳의 초기화는 임시로 하는 것이고, 실제 초기화는 Application Init에서 Configuration을 참고하여 수행한다.    
    //Track Control
    m_CameraTrackControl.SetTrackCenter(vec3(0.f,0.f,0.f));
    m_CameraTrackControl.SetPitchMinDegree(0.f);
    m_CameraTrackControl.SetPitchMaxDegree(0.f);
    
    //Zoom control
    m_CameraZoomControl.SetDistance(0.f);          //camera 방향으로 이동량
    
    //Animation control
    m_CameraAnimationControl.SetAnimating(false);
    
    
    m_ground_plane.SetPlane(vbPlane::PLN_P_ZX, 0.f);
    
    
    m_projection_mode   = PROJECTION_PERSPECTIVE;
    m_cam_navi_mode     = NAVIGATION_3D;//NAVIGATION_ORBIT;//NAVIGATION_PAN;  //NAVIGATION_ORBIT;
    
    m_pConf = NULL;
    
   
    m_eViewDirection = vbViewOrientationPortrait;
    
    m_eViewControlSet = vbViewControlSetYawPitch;//vbViewControlSetDefaultLBS;
    
   
    m_bViewUpdated = true;
    
    m_fDeviceAngle = 0.f;
    
    //각 영역에서 10~30px 사이의 영역을 사용함.
    unsigned short area_param[8] = { 0, 50,  0, 50,  0, 50,  0, 50 };
    memcpy(&m_area_param, area_param, sizeof(unsigned short)*8);
    
    m_bBrokeZoom = m_bBrokeRotate = m_bBrokePan = false;

}

//
void    vbViewInfo::BrokeControl(bool bBrokePan, bool bBrokeZoom, bool bBrokeRotate)
{
    m_bBrokeZoom = bBrokeZoom;
    m_bBrokeRotate = bBrokeRotate;
    m_bBrokePan = bBrokePan;
}

void vbViewInfo::ReloadConfiguration()
{
    if (m_pConf==NULL)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "Configuration class isn't pointed.\r\n";
#endif
        return;
    }
    //Projection
    float* per = (float*)m_pConf->fInitPerspective;
    SetPerspectiveParameters(per);
    UpdateProjectionMatrix();
    m_min_near_clip = m_pConf->fMinNear;
    
    m_CameraZoomControl.SetDistanceMin(m_pConf->fCamMinDistance);
    m_CameraZoomControl.SetDistanceMax(m_pConf->fCamMinDistance * 100.f); //모델이 로딩되면 다시 계산 된다.
    m_CameraZoomControl.SetDistance(0.f);
    
    
    m_cam_fMaxDistRatioBBSize = m_pConf->fCamMaxDistanceRatioToBBSize;
    m_cam_fMaxPanRatioToBSphereRad = m_pConf->fCamMaxPanRatioToBSphereRad;
    
    //Base Plane
    m_ground_plane = m_pConf->plnInitGround;
    
    //Pan Valid Ratio
    m_pan_valid_ratio = m_pConf->fPanPlaneExpandRatio;
    
    //방향을 정하고 Fitting을 하기 때문에 거리나 위치는 정하지 않음.
    
    //Camera direction
    m_Camera.SetCameraOrientation(m_pConf->qCameraQuaternion);
    
    //Camera Orbit center
    m_CameraTrackControl.SetTrackCenter(m_pConf->fvCameraOrbitCenter);
    
    //Eye height
    m_CameraFPSControl.SetEyeHeight(m_pConf->fEyeHeight);
        
    //Camera speed in FPS mode
    m_CameraFPSControl.SetMovingSpeed(m_pConf->fFPSSpeed);
    m_CameraFPSControl.SetTurnSpeed(m_pConf->qFPSRotation);
    
    //Camera angle for FPS End
    float fEndFPSDirection = DEFAULT_CONF_ENDFPS_ANGLE;
    if(m_pConf->fEndFPSDirection>=0)    fEndFPSDirection = DEFAULT_CONF_ENDFPS_ANGLE;
    else                                fEndFPSDirection = m_pConf->fEndFPSDirection;
    m_CameraFPSControl.SetEndAngle(fEndFPSDirection);    //Must be less than 0
    
    //Jump Zoom ratio
    m_cam_jumpzoom_ratio = m_pConf->fJumpZoomRatio;
    m_cam_jumpzoom_with_jumppan = m_pConf->bJumpPanOnJumpZoom;
    
    m_cam_ani_mode = CAMERA_ANI_NONE;
    m_CameraAnimationControl.UseAnimation(m_pConf->bUseCameraAnimation);
    m_CameraAnimationControl.SetProgressStep(m_pConf->fCameraAniStep);
   
    m_projection_mode   = PROJECTION_PERSPECTIVE;
    m_cam_navi_mode     = NAVIGATION_3D;//NAVIGATION_ORBIT;//NAVIGATION_PAN;  //NAVIGATION_ORBIT;
    
    //Rotation area ration
    m_fRotateAreaRatio = m_pConf->fCamRotateAreaRatio;
    
    //Yaw & Pitch Camera config
    m_CameraTrackControl.SetPitchMinDegree(m_pConf->fPitchLimitMin);
    m_CameraTrackControl.SetPitchMaxDegree(m_pConf->fPitchLimitMax);
    
    //Forward zoom height limits
    m_CameraForwardControl.SetTopAndBottomLimits(m_pConf->fForwardZoomHeightLimitTopY, m_pConf->fForwardZoomHeightLimitBottomY);
    
    //Env.
    m_cam_fEnvSphereRadius = m_pConf->fEnvSphereRadius;
    
    m_cam_bJumpZoomed = false;
    
    
    UpdateViewMatrix();    
}

void vbViewInfo::InitializeViewInfo(int width, int height, vbConfiguration* pConf)
{
    //Configuration
    m_pConf = pConf;
    
    //Screen Size
    SetScreenSize(ivec2(width,height));
    
    m_bBrokeZoom = m_bBrokeRotate = m_bBrokePan = false;
    
    //Trackball Size - 내접하도록 한다.
    if(width>height)     m_CameraFreeControl.SetVBallRadius(height/2.f);
    else                 m_CameraFreeControl.SetVBallRadius(width/2.f);
}

vbViewInfo::~vbViewInfo()
{
    m_pConf = NULL;
    
}


//Viewport
void    vbViewInfo::SetScreenSize(ivec2 screen_size)
{
    m_CameraPerspective.SetScreenSize(screen_size);
    m_CameraOrthogonal.SetScreenSize(screen_size);
}

ivec2   vbViewInfo::GetScreenSize()
{
    //뷰를 분리하기 전까지는 일단 Perspective...
    return m_CameraPerspective.GetScreenSize();
}



