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

void vbViewInfo::SingleFingerDown_Planar3D(ivec2 tPos, int tapCount)
{
    
    //std::cout << "SingleFingerUp_Planar3D - Single touch - Down\n";
    
    //회전 중 새로운 입력이 들어오면 무시한다.
    if(IsNavigation())   return;    //이후에는 각 컨트롤셋에서 처리할 것임.
    
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            if(m_bBrokePan==false)  StartHPanning(tPos);
        }
        break;
            
        default:
            break;
    }
    
}

bool vbViewInfo::SingleFingerMove_Planar3D(ivec2 tPos, int tapCount)
{
//    std::cout << "SingleFingerUp_Planar3D - Single touch - move\n";
    
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            if(m_bBrokePan)     return false;
            
            return DoHPanning(tPos);
        }
        break;

        default:
            break;
            
    }
    
    return false;    //No Update
    
}

bool vbViewInfo::SingleFingerUp_Planar3D(ivec2 tPos, int tapCount, vec3& pos3d)
{
    //std::cout << "SingleFingerUp_Planar3D - Single touch - up\n";
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
            if(tapCount<2)  EndHPanning();
            else
            {
                //하나라도 막혀있으면 JumpZoom을 하지 않음.
                if (m_bBrokePan || m_bBrokeZoom)    return false;
                
                //Zoom과 Pan 둘 다 가능할 때만 실행 됨.
                JumpZooming(tPos);
            }
        }
        break;
            
        default:    return false;
            
    }
    if(tapCount>1)  return true;
    return false;
}


bool vbViewInfo::DoubleFingerUp_Planar3D(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
           // std::cout << "SingleFingerUp_Planar3D - Double touch - Up\n";
           m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
            EndHRotateZoom();
            
        }
        break;

        default:
        {
        }
            break;
    }
    return true;
}

void vbViewInfo::DoubleFingerDown_Planar3D(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
#ifdef VB_CORE_DEBUG
     //       printf("DoubleFingerDown_Planar3D(x,y,count) - P1[%d,%d - %d], P2[%d,%d - %d]\r\n",pos1.x,pos1.y,tapCount1,pos2.x,pos2.y,tapCount2);
#endif
            m_cam_navi_3d_mode = NAVIGATION_3D_ROTATEZOOM;
            
            //둘 다 막혀 있으면 시작을 하지 않는다.
            if (m_bBrokeZoom && m_bBrokeRotate) return;
            
            //하나라도 허용되어 있으면 내부적으로 막는다.
            StartHRotateZoom(pos1, pos2);
            
        }
        break;
            
        default:
        {
        }
        break;
    }
}



bool vbViewInfo::DoubleFingerMove_Planar3D(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2)
{
    switch (m_cam_navi_mode)
    {
        case NAVIGATION_3D:
        {
#ifdef VB_CORE_DEBUG
      //      printf("DoubleFingerMove_Planar3D(x,y,count) - P1[%d,%d - %d], P2[%d,%d - %d]\r\n",pos1.x,pos1.y,tapCount1,pos2.x,pos2.y,tapCount2);
#endif
            return DoHRotateZoom(pos1, pos2);
        }
            break;

        default:
        {
        }
            break;
    }
    return true;
}

