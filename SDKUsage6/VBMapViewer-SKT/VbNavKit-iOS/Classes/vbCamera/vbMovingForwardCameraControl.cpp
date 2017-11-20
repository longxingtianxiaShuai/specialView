//
//  vbHorizontalPanCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include <iostream>
#include "vbCamera.hpp"
#include "vbMovingForwardCameraControl.hpp"

vbMovingForwardCameraControl::vbMovingForwardCameraControl()
{
    m_base_value=m_forward_increment=0.f;
    m_forward_increment = 100.f;
    
    m_fForwardZoomHeightLimitBottomY = 150.f;
    m_fForwardZoomHeightLimitTopY    = 10000.f;
}

vbMovingForwardCameraControl::~vbMovingForwardCameraControl()
{
    
}

vec3  vbMovingForwardCameraControl::GetPosition()
{
    vec3 new_pos;
    
    return new_pos;
}

vec3    vbMovingForwardCameraControl::MoveForward(float fCurrValue)
{
    //현재는 두 터치 사이의 거리로 한다.
    float ratio = fCurrValue / m_base_value;
    
    //전진
    if (ratio>=1.f)
    {
        m_curr_pos = m_start_pos + m_forward_dir * m_forward_increment * (ratio - 1.f);
    }
    //후진
    else
    {
        m_curr_pos = m_start_pos - m_forward_dir * m_forward_increment * (1.f - ratio);
    }
    
    //Check height limits
    if(m_curr_pos.y > m_fForwardZoomHeightLimitTopY)
    {
       // std::cout << "[FORWARD CAMERA LIMIT] POS is over the upper limit as " << m_curr_pos.y << ", it is limited as " << m_fForwardZoomHeightLimitTopY << "\n";
        m_curr_pos.y = m_fForwardZoomHeightLimitTopY;
    }
    else if(m_curr_pos.y < m_fForwardZoomHeightLimitBottomY)
    {
       // std::cout << "[FORWARD CAMERA LIMIT] POS is under the lower limit as " << m_curr_pos.y << ", it is limited as " << m_fForwardZoomHeightLimitBottomY << "\n";
        m_curr_pos.y = m_fForwardZoomHeightLimitBottomY;
        
    }
    
    if(m_pCamera)
    {
        m_pCamera->SetCameraPosition(m_curr_pos);
    }
    else
    {
#ifdef VB_CORE_DEBUG
        std::cout << "[** CRITICAL ERROR **] Camera pointer isn't assigned to FORWARD CAMERA CONTROL.\n";
#endif
    }

    return m_curr_pos;
}


vec3    vbMovingForwardCameraControl::MoveVertical(float fCurrValue)
{
    //기존 좌표 대 현재 좌표를 비교
    float ratio = fCurrValue - m_base_value;
    
    m_curr_pos.y = m_start_pos.y + m_forward_increment * ratio;
    
    
    //Check height limits
    if(m_curr_pos.y > m_fForwardZoomHeightLimitTopY)
    {
     //   std::cout << "[FORWARD CAMERA LIMIT] POS is over the upper limit as " << m_curr_pos.y << ", it is limited as " << m_fForwardZoomHeightLimitTopY << "\n";
        m_curr_pos.y = m_fForwardZoomHeightLimitTopY;
    }
    else if(m_curr_pos.y < m_fForwardZoomHeightLimitBottomY)
    {
     //   std::cout << "[FORWARD CAMERA LIMIT] POS is under the lower limit as " << m_curr_pos.y << ", it is limited as " << m_fForwardZoomHeightLimitBottomY << "\n";
        m_curr_pos.y = m_fForwardZoomHeightLimitBottomY;
        
    }
    
    if(m_pCamera)
    {
        m_pCamera->SetCameraPosition(m_curr_pos);
    }
    else
    {
#ifdef VB_CORE_DEBUG
        std::cout << "[** CRITICAL ERROR **] Camera pointer isn't assigned to FORWARD CAMERA CONTROL.\n";
#endif
    }
    
    return m_curr_pos;
}
