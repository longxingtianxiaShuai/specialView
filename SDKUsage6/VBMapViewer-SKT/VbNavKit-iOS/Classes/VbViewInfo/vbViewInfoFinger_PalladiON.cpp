//
//  vbViewInfoFinger_PalladiON.cpp
//  VbNavKit-iOS
//
//  Created by Wonil Shim on 2014. 1. 28..
//  Copyright (c) 2014년 dev4. All rights reserved.
//
#include "vbViewInfo.hpp"

#include <iostream>


////////////////////////////////////////////////////// Default//////////////////////////////////////////////////////

void vbViewInfo::SingleFingerDown_PalladiON(ivec2 tPos, int tapCount)
{
    //회전 중 새로운 입력이 들어오면 무시한다.
    if(IsNavigation())   return;    //이후에는 각 컨트롤셋에서 처리할 것임.
    
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            StartYawPitchRotation(tPos);    //실제로 사용되는 회전은 수평 회전 뿐이다.
        }
        break;
            
        case NAVIGATION_WALK:       {   StartWalking(tPos);     }   break;
        case NAVIGATION_2D:
        default:                    break;
    }
    
}


bool vbViewInfo::SingleFingerMove_PalladiON(ivec2 tPos, int tapCount)
{
    //std::cout << "Mouse - " << tPos.x << "/" << m_screen_size.x << ", " << tPos.y << "/" << m_screen_size.y << "\r\n";
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            return DoContinuousYawRotation(tPos);
        }
        break;
            
            
        case NAVIGATION_WALK:
        {
            return DoWalking(tPos);
        }
        break;
            
        case NAVIGATION_2D:
            break;
            
        default:
            break;
            
    }
    
    return false;    //No Update
    
}

bool vbViewInfo::SingleFingerUp_PalladiON(ivec2 tPos, int tapCount, vec3& pos3d)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            if(tapCount<2)  EndRotation();
            else            JumpZooming(tPos);
        }
            break;
            
        case NAVIGATION_2D:
        {
            if(tapCount>=2)  JumpZooming(tPos);
        }
            break;
            
            
        case NAVIGATION_WALK:
        {
            EndWalking();
            return false;
        }
            break;
            
        case NAVIGATION_ME:
        {
            pos3d = GetRayIntersectionWithGround(tPos);
            return true; //Me를 내부에서 모르기 때문에 외부에서 처리 할 것은 3D좌표를 전달한다.
        }
            break;
            
        default:    return false;
            
    }
    if(tapCount>1)  return true;
    return false;
}


bool vbViewInfo::DoubleFingerUp_PalladiON(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
            EndPanZoom();
            //StartRotation(pos1);
        }
            break;
        case NAVIGATION_2D:
        {
            EndPanZoom();
        }
            break;
        case NAVIGATION_WALK:
        {
            return false;
        }
            break;
        default:
        {
        }
            break;
    }
    return true;
}

void vbViewInfo::DoubleFingerDown_PalladiON(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_PANZOOM;
            StartPanZoom(pos1, pos2);
        }
            break;
            
        case NAVIGATION_2D:
        {
            m_cam_navi_3d_mode = NAVIGATION_3D_PANZOOM;
            StartPanZoom(pos1, pos2);
        }
            break;
            
        case NAVIGATION_WALK:
        {
        }
            break;
        default:
        {
        }
            break;
    }
}

bool vbViewInfo::DoubleFingerMove_PalladiON(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        case NAVIGATION_2D:
        {
            //return DoHPanning((pos1+pos2)/2);
            return DoPanZoom(pos1, pos2);
        }
            break;
            
        case NAVIGATION_WALK:
        {
            return false;
        }
            break;
        default:
        {
        }
            break;
    }
    return true;
}

