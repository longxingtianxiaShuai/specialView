#include "vbViewInfo.hpp"

#include <iostream>

/////////////////////////////////////////// GongVueA //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////
void    vbViewInfo::SetScreenAreaParam(const vbScreenControlAreaParam* pParam)
{
    memcpy(&m_area_param, pParam, sizeof(vbScreenControlAreaParam));
}

vbViewInfo::vbViewScreenControlArea vbViewInfo::IsScreenControlArea(ivec2 tPos)
{
    ivec2   scr_size = m_CameraPerspective.GetScreenSize();
    
    //좌우 영역을 먼저 확인하고, 그 다음 위아래 영역을 검사한다. - 좌우하상 
    //Left
    if (tPos.x >= m_area_param.area_param[0] && tPos.x <= m_area_param.area_param[1])
    {
        return vbViewScreenControlAreaLeft;
    }
    //Right
    else if (tPos.x >= (scr_size.x - m_area_param.area_param[3]) && tPos.x <= (scr_size.x - m_area_param.area_param[2]))
    {
        return vbViewScreenControlAreaRight;
    }
    //Bottom
    else if (tPos.y >= (scr_size.y - m_area_param.area_param[5]) && tPos.y <= (scr_size.y -m_area_param.area_param[4]))
    {
        return vbViewScreenControlAreaBottom;
    }
    //Top
    else if (tPos.y >= m_area_param.area_param[6] && tPos.y <= m_area_param.area_param[7])
    {
        return vbViewScreenControlAreaTop;
    }
    else
        return vbViewScreenControlAreaMid;
}


void vbViewInfo::SingleFingerDown_GongVueA(ivec2 tPos, int tapCount)
{
    //회전 중 새로운 입력이 들어오면 무시한다.
    if(IsNavigation())   return;
    if(m_cam_navi_mode!=NAVIGATION_3D)  return;
    
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
            {
                m_cam_navi_3d_mode = NAVIGATION_3D_PAN;
                StartHPanning(tPos);
            }
            break;
            default:
            break;
    }
}

bool vbViewInfo::SingleFingerMove_GongVueA(ivec2 tPos, int tapCount)
{
    //제어 상태가 아니면 아무것도 처리 하지 않는다.
    if(!IsNavigation())                 return false;
    if(m_cam_navi_mode!=NAVIGATION_3D)  return false;
  
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
            //SingleFingerDown_GongVueA(tPos,1);
        }
        break;
    }
       
    return false;    //No Update
    
}

bool vbViewInfo::SingleFingerUp_GongVueA(ivec2 tPos, int tapCount, vec3& pos3d)
{
    //제어 상태가 아니면 아무것도 처리 하지 않는다.
    if(!IsNavigation())                 return false;
    if(m_cam_navi_mode!=NAVIGATION_3D)  return false;
 
    m_bNavigation = false;
    m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
 
    return false;
}

void vbViewInfo::DoubleFingerDown_GongVueA(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
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
}

bool vbViewInfo::DoubleFingerMove_GongVueA(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
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
}


bool vbViewInfo::DoubleFingerUp_GongVueA(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2)
{
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
}
