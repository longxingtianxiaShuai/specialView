//
//  vbZoomCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbMoveOrbitZoomCameraControl.hpp"
#include <float.h>
#include <iostream>

vbMoveOrbitZoomCameraControl::vbMoveOrbitZoomCameraControl()
{
    
    m_fDistance     = 0.f;
    m_fDistanceMin = 100.f;
    m_fDistanceMax = FLT_MAX;
   
}

vbMoveOrbitZoomCameraControl::~vbMoveOrbitZoomCameraControl()
{
    
}

vec3  vbMoveOrbitZoomCameraControl::GetPosition()
{
    vec3 new_pos;
    
    return new_pos;
}

void    vbMoveOrbitZoomCameraControl::StartZoomWithRatio(float fInitRatio)
{
    //현재 카메라 거리를 구한다.
    m_fBaseDistance = m_fDistance;
    
    //Zoom 시작 비율을 저장한다.
    m_fBaseRatio = fInitRatio;
}


void    vbMoveOrbitZoomCameraControl::UpdateDistanceWithBaseDistanceRatio(float fNewRatio)
{
    float cam_distance = m_fBaseDistance * m_fBaseRatio / fNewRatio;
    
    //Zoom in
    if( (m_fBaseRatio<fNewRatio) && cam_distance < m_fDistanceMin)
    {
#ifdef VB_CORE_DEBUG
        printf("[MinDistBroking]Base ratio = %f, New ratio = %f, MinDist = %f, MaxDist = %f, new camera distance = %f\r\n", m_fBaseRatio, fNewRatio, m_fDistanceMin, m_fDistanceMax, cam_distance);
#endif
        cam_distance = m_fDistanceMin;

        
        //현재 카메라 거리를 구한다.
        m_fBaseDistance = m_fDistance;
        //Zoom 시작 비율을 저장한다.
        m_fBaseRatio = fNewRatio;
    }
    //Zoom out
    else if( (m_fBaseRatio>fNewRatio) && cam_distance>m_fDistanceMax)
    {
#ifdef VB_CORE_DEBUG
        printf("[MaxDistBroking]Base ratio = %f, New ratio = %f, MinDist = %f, MaxDist = %f, new camera distance = %f\r\n", m_fBaseRatio, fNewRatio, m_fDistanceMin, m_fDistanceMax, cam_distance);
#endif
        cam_distance = m_fDistanceMax;
        
        //현재 카메라 거리를 구한다.
        m_fBaseDistance = m_fDistance;
        //Zoom 시작 비율을 저장한다.
        m_fBaseRatio = fNewRatio;
    }
    
    m_fDistance = cam_distance;
}

void    vbMoveOrbitZoomCameraControl::UpdateDistanceFromBaseDistPlusBaseDistRatio(float fNewRatio)
{
    
    float ratio = fNewRatio/ m_fBaseRatio;
    
    float cam_distance = m_fBaseDistance + (m_fDistanceMax - m_fDistanceMin) * (ratio - 1.f);

    if(cam_distance < m_fDistanceMin)           cam_distance = m_fDistanceMin;
    else if(cam_distance>m_fDistanceMax)        cam_distance = m_fDistanceMax;
    
    m_fDistance = cam_distance;
    
    
}

void    vbMoveOrbitZoomCameraControl::ToggleDistanceLimit()
{
    if(m_fDistance > m_fDistanceMax*0.9f)
    {
        //zoom out --> zoom in
        m_fDistance = m_fDistanceMin;
    }
    else
    {
        //zoom in --> zoom out
        m_fDistance = m_fDistanceMax;
    }
}


