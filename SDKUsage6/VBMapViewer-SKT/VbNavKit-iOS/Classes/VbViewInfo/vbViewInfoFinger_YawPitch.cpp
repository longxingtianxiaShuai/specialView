#include "vbViewInfo.hpp"

#include <iostream>

/////////////////////////////////////////// Yaw & Pitch //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void vbViewInfo::SingleFingerDown_YawPitch(ivec2 tPos, int tapCount)
{
    //회전 중 새로운 입력이 들어오면 무시한다.
    if(IsNavigation())   return;
    
    switch (m_cam_navi_mode)
    {
            
        case NAVIGATION_3D:
        {
            //화면 영역으로 회전인지 이동인지 식별
            ivec2   scr_size = m_CameraPerspective.GetScreenSize();
            if(tPos.y < ((float)scr_size.y) * (1.f - m_fRotateAreaRatio))
            {
                m_cam_navi_3d_mode = NAVIGATION_3D_PAN;
                StartHPanning(tPos);
            }
            else
            {
                m_cam_navi_3d_mode = NAVIGATION_3D_ORBIT_H;
                StartHRotation(tPos);
            }
            
        }
            break;
            
        case NAVIGATION_ENVRN://Yaw & Pitch
        {
            StartYawPitchRotation(tPos);
        }
            break;
            
            
        default:
        {
        }
            break;
    }
    
}

bool vbViewInfo::SingleFingerMove_YawPitch(ivec2 tPos, int tapCount)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D://Yaw & Pitch
        {
            switch (m_cam_navi_3d_mode)
            {
                case NAVIGATION_3D_PAN:
                {
                    return DoHPanning(tPos);
                }
                    break;
                    
                case NAVIGATION_3D_ORBIT_H:
                {
                    return DoYawRotation(tPos);
                }
                    break;
                    
                default://중간에 변경 된 경우
                {
                    /*
                    ivec2   scr_size = m_CameraPerspective.GetScreenSize();
                    
                    //화면 영역으로 회전인지 이동인지 식별
                    if(tPos.y < ((float)scr_size.y) * (1.f - m_fRotateAreaRatio))
                    {
                        m_cam_navi_3d_mode = NAVIGATION_3D_PAN;
                        StartHPanning(tPos);
                    }
                    else
                    {
                        m_cam_navi_3d_mode = NAVIGATION_3D_ORBIT_H;
                        StartHRotation(tPos);
                    }
                     */
                }
                    break;
            }
            
        }
            break;
            
        case NAVIGATION_ENVRN://Yaw & Pitch
        {
            return DoLandscapeYawPitchRotation(tPos);
        }
            break;
            
        default:
        {
        }
            break;
    }
    
    return false;    //No Update
    
}

bool vbViewInfo::SingleFingerUp_YawPitch(ivec2 tPos, int tapCount, vec3& pos3d)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            
            switch (m_cam_navi_3d_mode)
            {
                case NAVIGATION_3D_PAN:
                {
                    if(tapCount<2)  {   EndRotation(); EndHPanning();    }
                    else            JumpZooming(tPos);
                }
                    break;
                    
                case NAVIGATION_3D_ORBIT_H:
                {
                    EndRotation();
                }
                    break;
                    
                default:
                {
                    EndRotation();
                }
                    break;
            }
            
        }
            break;
            
        case NAVIGATION_ENVRN:
        {
            if(tapCount<2)  EndRotation();
            else            SphereJumpZooming(tPos);
        }
            break;
            
        default:
        {
            return false;
        }
            break;
            
    }
    
    if(tapCount>1)  return true;
    
    return false;
}

void vbViewInfo::DoubleFingerDown_YawPitch(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_ZOOM;
            StartZooming2Pt(pos1, pos2);
        }
            break;
        case NAVIGATION_ENVRN:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_ZOOM;
            StartZooming2PtEnvironment(pos1, pos2);
        }
            break;
            
        default:
        {
        }
            break;
    }
}

bool vbViewInfo::DoubleFingerMove_YawPitch(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            if(m_cam_navi_3d_mode != NAVIGATION_3D_ZOOM)
            {
                m_cam_navi_3d_mode = NAVIGATION_3D_ZOOM;
                StartZooming2Pt(pos1, pos2);
                return true;
            }
            
            return DoZooming2Pt(pos1, pos2);
        }
            break;
            
        case NAVIGATION_ENVRN:
        {
            return DoZooming2PtEnvironment(pos1, pos2);
        }
            break;
            
            
        default:
        {
        }
            break;
    }
    return true;
}


bool vbViewInfo::DoubleFingerUp_YawPitch(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        case NAVIGATION_ENVRN:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
            EndZooming();
            
        }
            break;
            
        default:
        {
        }
            break;
    }
    return true;
}
