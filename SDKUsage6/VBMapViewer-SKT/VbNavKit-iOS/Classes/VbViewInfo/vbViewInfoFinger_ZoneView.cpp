#include "vbViewInfo.hpp"

#include <iostream>

/////////////////////////////////////////// ZoneView //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////


void vbViewInfo::SingleFingerDown_ZoneView(ivec2 tPos, int tapCount)
{
    //회전 중 새로운 입력이 들어오면 무시한다.
    if(IsNavigation())   return;
    if(m_cam_navi_mode!=NAVIGATION_3D)  return;
    
    //ZoneView Mode에서는 무조건 수평 회전(Yaw)만 한다.
    m_cam_navi_3d_mode = NAVIGATION_3D_ORBIT_H;
    StartHRotation(tPos);
    
    /* 나중에 제스쳐 수정 될 수 있으니 일단 참조 코드 놔둠.
    switch (IsScreenControlArea(tPos))
    {
        case vbViewScreenControlAreaLeft://Pitch
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_FIXED_V;
            StartFixedYawPitchRotate(tPos);
        }
            break;
            
        case vbViewScreenControlAreaRight://Height
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_ELEVATION;
            StartElevation(tPos);
        }
            break;
            
        case vbViewScreenControlAreaTop://Yaw
        case vbViewScreenControlAreaBottom:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_FIXED_H;
            StartFixedYawPitchRotate(tPos);
        }
            break;
            
        case vbViewScreenControlAreaMid://Pan
        default:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_PAN;
            StartHPanning(tPos);
        }
            break;
    }
     */
}

bool vbViewInfo::SingleFingerMove_ZoneView(ivec2 tPos, int tapCount)
{
    //제어 상태가 아니면 아무것도 처리 하지 않는다.
    if(!IsNavigation())                 return false;
    if(m_cam_navi_mode!=NAVIGATION_3D)  return false;
    
    //ZoneView Mode에서는 무조건 수평 회전(Yaw)만 한다.
    return DoYawRotation(tPos);
    
    
    /*
    switch (m_cam_navi_3d_mode)
    {
        case NAVIGATION_3D_FIXED_V:
        {
            return DoFixedPitchRotate(tPos);
        }
            break;
            
        case NAVIGATION_3D_ELEVATION:
        {
            return DoElevation(tPos);
        }
            break;
            
        case NAVIGATION_3D_FIXED_H:
        {
            return DoFixedYawRotate(tPos);
        }
            break;
            
        case NAVIGATION_3D_PAN:
        {
            return DoHPanning(tPos);
        }
            break;
            
        default://중간에 변경 된 경우
        {
            SingleFingerDown_ZoneView(tPos,1);
        }
            break;
    }
     */
    
    return false;    //No Update
    
}

bool vbViewInfo::SingleFingerUp_ZoneView(ivec2 tPos, int tapCount, vec3& pos3d)
{
    //제어 상태가 아니면 아무것도 처리 하지 않는다.
    if(!IsNavigation())                 return false;
    if(m_cam_navi_mode!=NAVIGATION_3D)  return false;
    
    m_bNavigation = false;
    m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
    
    return false;
}

void vbViewInfo::DoubleFingerDown_ZoneView(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    return;
    
    /*
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_FORWARD;
            StartForwardZoom(pos1, pos2);
        }
            break;
            
            
        default:
        {
        }
            break;
    }
     */
}

bool vbViewInfo::DoubleFingerMove_ZoneView(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    return true;
    
    /*
    //Env는 아직은 없으니까....
    switch (m_cam_navi_3d_mode)
    {
        case NAVIGATION_3D_FORWARD:
        {
            return DoForwardZoom(pos1, pos2);
        }
            break;
            
        default:
        {
        }
            break;
    }
    return true;
     */
}


bool vbViewInfo::DoubleFingerUp_ZoneView(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2)
{
    return true;
    
    /*
    switch (m_cam_navi_3d_mode)
    {
        case NAVIGATION_3D_FORWARD:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
            EndForwardZoom();
        }
            break;
            
        default:
        {
        }
            break;
    }
    return true;
     */
}
