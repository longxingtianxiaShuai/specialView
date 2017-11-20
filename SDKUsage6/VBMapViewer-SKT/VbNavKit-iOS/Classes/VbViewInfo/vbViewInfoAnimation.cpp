#include "vbViewInfo.hpp"

#include <iostream>
#include <cmath>
#include <float.h>


//Animation

void vbViewInfo::UpdateAnimationFrame()
{
    switch(m_cam_navi_mode)
    {
            
        case NAVIGATION_WALK:
        {
            UpdateWalkingAnimation();
        }
        break;
            
        default:
            UpdateAnimatedCamera();
            break;
    }
    return;
}

bool vbViewInfo::UpdateWalkingAnimation()
{
    //입력 된 점의 유효성을 검사한다.
    if(!m_bNavigation || m_cam_navi_mode!=NAVIGATION_WALK)  return false;
    
    ivec2   scr_size = m_CameraPerspective.GetScreenSize();
    
    //////////////////////////  Rotate
    float halfH = (float)(scr_size.x/2);
    float ratioH = (float)(halfH - m_DownPos_prev.x)/halfH; 

    //마우스 위치로 속도를 정한다.
    float halfV = (float)(scr_size.y/2);
    float ratioV = (float)(halfV - m_DownPos_prev.y)/halfV;
    
    //방향과 위치 갱신
    m_CameraFPSControl.UpdateWalkingAnimation(ratioH, ratioV);
    
    //시선 방향 갱신
    UpdateOrbitCenterFromCamPosition();
    
    //메트릭스 갱신
    UpdateViewMatrix();
    
    return true;
    
}


void vbViewInfo::InitCameraMoveAni(vec3 pos_start, vec3 pos_end, Quaternion dir_start, Quaternion dir_end, CameraAniMode aniMode)
{
    m_CameraAnimationControl.InitAnimation(pos_start, pos_end, dir_start, dir_end);
    m_cam_ani_mode              = aniMode;
}

void  vbViewInfo::InitDeviceRotateAni(vbViewDirection eEndDirection)
{
    m_CameraAnimationControl.InitDeviceRotationAnimation(m_fDeviceAngle, eEndDirection);
    m_cam_ani_mode              = CAMERA_ANI_DEVICE_ROT;
}

void vbViewInfo::UpdateAnimatedCamera()
{
    
     //Progress
    float progress_ratio = m_CameraAnimationControl.IncreaseRatio();
    
    if(progress_ratio<-0.001f || progress_ratio>1.f)
    {
        //종료 처리는 외부에서 모드에 맞게 처리한다.
        m_CameraAnimationControl.SetAnimating(false);
        
        switch (m_cam_ani_mode)
        {
            case CAMERA_ANI_JUMPPAN:
            {
                m_Camera.SetCameraPosition(m_CameraAnimationControl.GetEndPosition());
                UpdateOrbitCenterFromCamPosition();
                
                m_bNavigation = false;
                
                float cam_distance = (m_CameraTrackControl.GetTrackCenter() - m_Camera.GetCameraPosition()).size();
                m_CameraZoomControl.SetDistance(cam_distance);
  
            }
            break;
                
            case CAMERA_ANI_JUMPZOOM:
            {
                
                if(m_cam_navi_mode!=NAVIGATION_ENVRN)
                {
                    m_CameraTrackControl.SetTrackCenter(m_CameraAnimationControl.GetEndPosition());
                    float cam_distance;
                    if(!m_cam_bJumpZoomed)
                        cam_distance = m_CameraZoomControl.GetBaseDistance() * m_cam_jumpzoom_ratio;
                    else
                        cam_distance = m_CameraZoomControl.GetBaseDistance() / m_cam_jumpzoom_ratio;
                    m_CameraZoomControl.SetDistance(cam_distance);
                    
                    //Check Zoom limt(Camera distance)
                    if(cam_distance<m_CameraZoomControl.GetDistanceMin())
                    {
                        cam_distance = m_CameraZoomControl.GetDistanceMin();
                        m_CameraAnimationControl.SetProgressRatio(1.f);
                    }
                    else if(cam_distance>m_CameraZoomControl.GetDistanceMax())
                    {
                        cam_distance = m_CameraZoomControl.GetDistanceMax();
                        m_CameraAnimationControl.SetProgressRatio(1.f);
                    }
                    
                    m_CameraZoomControl.SetDistance(cam_distance);
                    
                    //Orbit center를 기준으로 카메라 위치를 갱신한다.
                    UpdateCameraPositionFromOrbit();
                    m_cam_bJumpZoomed = !m_cam_bJumpZoomed;
                }
                else
                {
                    m_CameraZoomControl.SetDistance(m_CameraAnimationControl.GetEndPosition().z);
                }
                m_bNavigation = false;
                

            }
            break;
                
            case CAMERA_ANI_JUMPPANROT:
            {
                m_CameraTrackControl.SetTrackCenter(m_CameraAnimationControl.GetEndPosition());
                m_Camera.SetCameraOrientation(m_CameraAnimationControl.GetEndOrientation());
                UpdateCameraPositionFromOrbit();
                m_bNavigation = false;
                
            }
            break;
                
            case CAMERA_ANI_3DTO2D:
            {
                m_Camera.SetCameraOrientation(m_CameraAnimationControl.GetEndOrientation());
                UpdateCameraPositionFromOrbit();
                m_bNavigation = false;
                
            }
            break;
                
            case CAMERA_ANI_DEVICE_ROT:
            {
                UpdateDeviceAngleDeg();    
            }
                break;
                
            case CAMERA_ANI_YAW_ROTATE:
            {
                //끝내지 않는다.
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        //2차 선형 보간
        float progress_ration = m_CameraAnimationControl.GetProgressRatio();
        float ratio = progress_ration * progress_ration;
        
        switch (m_cam_ani_mode)
        {
            case CAMERA_ANI_JUMPPAN:
            {
                vec3 cam_position = m_CameraAnimationControl.GetInterpolatedPosition(ratio);
                
                m_Camera.SetCameraPosition(cam_position);
                
                UpdateOrbitCenterFromCamPosition();
 
            }
            break;
                
            case CAMERA_ANI_JUMPZOOM:
            {
                 if(m_cam_navi_mode!=NAVIGATION_ENVRN)
                {
                    
                    vec3 cam_orbit_cntr = m_CameraAnimationControl.GetInterpolatedPosition(ratio);
                    m_CameraTrackControl.SetTrackCenter(cam_orbit_cntr);
                    
                    float cam_distance = 0.f;
                    if(!m_cam_bJumpZoomed)
                        cam_distance = m_CameraZoomControl.GetBaseDistance() * (1.f - ratio) +  m_CameraZoomControl.GetBaseDistance() * m_cam_jumpzoom_ratio * ratio;
                    else
                        cam_distance = m_CameraZoomControl.GetBaseDistance() * (1.f - ratio) +  m_CameraZoomControl.GetBaseDistance() / m_cam_jumpzoom_ratio * ratio;
                    
                    //Check Zoom limt(Camera distance)
                    if(cam_distance<m_CameraZoomControl.GetDistanceMin())
                    {
                        cam_distance = m_CameraZoomControl.GetDistanceMin();
                        m_CameraAnimationControl.SetProgressRatio(1.f);
                    }
                    else if(cam_distance>m_CameraZoomControl.GetDistanceMax())
                    {
                        cam_distance = m_CameraZoomControl.GetDistanceMax();
                        m_CameraAnimationControl.SetProgressRatio(1.f);
                    }
                    
                    m_CameraZoomControl.SetDistance(cam_distance);
                    
                    
                    //Orbit center를 기준으로 카메라 위치를 갱신한다.
                    UpdateCameraPositionFromOrbit();
                }
                else
                {
                    //Environment
                    vec3    interpolated_pos = m_CameraAnimationControl.GetInterpolatedPosition(ratio);
                    m_CameraZoomControl.SetDistance(interpolated_pos.z);
                    
                }

            }
            break;
                
            case CAMERA_ANI_JUMPPANROT:
            {
                //Pan
                vec3 cam_orbit_cntr = m_CameraAnimationControl.GetInterpolatedPosition(ratio);
                m_CameraTrackControl.SetTrackCenter(cam_orbit_cntr);
                
                //Rot
                Quaternion cam_orbit_rot = m_CameraAnimationControl.GetSlerpOrientation(ratio);
                m_Camera.SetCameraOrientation(cam_orbit_rot);
                UpdateCameraPositionFromOrbit();
            }
                break;
            case CAMERA_ANI_3DTO2D:
            {
                Quaternion cam_orbit_rot = m_CameraAnimationControl.GetSlerpOrientation(ratio);
                m_Camera.SetCameraOrientation(cam_orbit_rot);
                UpdateCameraPositionFromOrbit();    
            }
            break;
                
            case CAMERA_ANI_2DTO3D:
            {
                Quaternion cam_orbit_rot = m_CameraAnimationControl.GetSlerpOrientation(ratio);
                m_Camera.SetCameraOrientation(cam_orbit_rot);
                UpdateCameraPositionFromOrbit();    
            }
            break;
                
            case CAMERA_ANI_DEVICE_ROT:
            {
                m_fDeviceAngle  = m_CameraAnimationControl.GetInterpolatedDeviceAngle(ratio);
            }
            break;
                
            case CAMERA_ANI_YAW_ROTATE:
            {
                m_CameraAnimationControl.SetProgressRatio(0.f);
                float new_yaw = m_Camera.GetYawDegree()+m_CameraAnimationControl.GetYawStep();
                
                if (new_yaw< (-180.f)) {
                    while (new_yaw < (-180.f)) {
                        new_yaw += 360.f;
                    }
                }
                else if(new_yaw > 180.f) {
                    while (new_yaw > 180.f) {
                        new_yaw -= 360.f;
                    }
                }
                m_Camera.SetCameraOrientation(new_yaw, m_Camera.GetPitchDegree());
                UpdateCameraPositionFromOrbit();
            }
                break;
                
                
            default:
                break;
        }        
    }
    

    
    //선형 보간을 한다.

    
    UpdateViewMatrix();
    UpdateNearFarClip();
    
    
}

void    vbViewInfo::UseCameraAnimation(bool bUseCameraAnimation)
{
    m_CameraAnimationControl.UseAnimation(bUseCameraAnimation);
}

bool    vbViewInfo::IsCameraAnimationAvailable()
{
    return  m_CameraAnimationControl.IsAnimationUsed();
}

bool    vbViewInfo::IsOnAnimating()
{
    return m_CameraAnimationControl.IsOnAnimating();
}
