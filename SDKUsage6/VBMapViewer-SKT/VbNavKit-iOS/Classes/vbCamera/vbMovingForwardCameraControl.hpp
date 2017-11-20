//
//  vbHorizontalPanCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbMovingForwardCameraControl__
#define __VbNavKit_iOS__vbMovingForwardCameraControl__


#include "vbMoveCameraControl.hpp"

class vbMovingForwardCameraControl : public vbMoveCameraControl
{
    
public:
    vbMovingForwardCameraControl();
    ~vbMovingForwardCameraControl();
    
private:
    vec3    m_start_pos;
    vec3    m_curr_pos;
    vec3    m_forward_dir;
    float   m_base_value;
    float   m_forward_increment;
    
    float   m_fForwardZoomHeightLimitBottomY;
    float   m_fForwardZoomHeightLimitTopY;

    
public:
    //virtual func
    vec3    GetPosition();
    
    //Set/Get
    vec3    GetStartPosition()                      {   return m_start_pos;         }
    void    SetStartPosition(vec3 start_pos)        {   m_start_pos = start_pos;    }
    vec3    GetCurrentPosition()                    {   return m_curr_pos;          }
    void    SetCurrentPosition(vec3 cur_pos)        {   m_curr_pos = cur_pos;       }
    
    vec3    GetForwardDir()                         {   return m_forward_dir;       }
    void    SetForwardDir(vec3 normalized_dir)      {   m_forward_dir = normalized_dir; }
    
    float   GetBaseValue()                          {   return m_base_value;        }
    void    SetBaseValue(float fBaseValue)          {   m_base_value = fBaseValue;  }
    float   GetIncrement()                          {   return m_forward_increment; }
    void    SetIncrement(float fIncrement)          {   m_forward_increment = fIncrement; }
    
    float   GetBottomLimit()                        {   return m_fForwardZoomHeightLimitBottomY;}
    float   GetTopLimit()                           {   return m_fForwardZoomHeightLimitTopY;   }
    void    SetTopAndBottomLimits(float fTop, float fBottom)
    {
        m_fForwardZoomHeightLimitBottomY = fBottom;
        m_fForwardZoomHeightLimitTopY    = fTop;
    }
    
    
    vec3    MoveForward(float fCurrValue);
    vec3    MoveVertical(float fCurrValue);
 
};
#endif /* defined(__VbNavKit_iOS__vbMovingForwardCameraControl__) */
