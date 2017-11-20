//
//  vbAnimationCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 12..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbAnimationCameraControl.hpp"
vbAnimationCameraControl::vbAnimationCameraControl()
{
    m_fYawStep = 0.f;
    
}

vbAnimationCameraControl::~vbAnimationCameraControl()
{
    
}

vec3 vbAnimationCameraControl::GetInterpolatedPosition(float ratio_from_start)
{
    vec3 cam_pos = m_Start_pos + (m_End_pos - m_Start_pos) * ratio_from_start;
    return cam_pos;
}

float vbAnimationCameraControl::GetInterpolatedDeviceAngle(float ratio_from_start)
{
    float fDeviceAngle  = m_fDeviceRot_Start + (m_fDeviceRot_Start - m_fDeviceRot_End) * ratio_from_start;
    return fDeviceAngle;
}

Quaternion  vbAnimationCameraControl::GetSlerpOrientation(float ratio_from_start)
{
    Quaternion quar = m_Start_orient.Slerp(ratio_from_start, m_End_orient); 
    return quar;
}

float vbAnimationCameraControl::IncreaseRatio(int nStep)
{
    m_fProgressRatio += m_fProgressStep * (float)nStep;
    return m_fProgressRatio;
}

void vbAnimationCameraControl::InitAnimation(vec3 start_pos, vec3 end_pos, Quaternion start_orient, Quaternion end_orient)
{
    //POSITION
    m_Start_pos = start_pos;
    m_End_pos   = end_pos;
    
    //ORIENTATION
    m_Start_orient  = start_orient;
    m_End_orient    = end_orient;
    
    //PROGRESS
    m_fProgressRatio = 0.f;
    
    //ANIMATION FLAG
    m_bOnAnimating = true;
}

void vbAnimationCameraControl::InitDeviceRotationAnimation(float fFromAngle, vbViewDirection eEndOrientation)
{
    //Start
    m_fDeviceRot_Start = fFromAngle;
    
    //End
    float fEndAngle = fFromAngle;
    switch (eEndOrientation)
    {
            //1-Device oriented vertically, home button on the bottom
        case vbViewOrientationPortrait:                 fEndAngle = 0.f;       break;
            //2-Device oriented vertically, home button on the top
        case vbViewOrientationPortraitUpsideDown:       fEndAngle = 180.f;     break;
            //3-Device oriented horizontally, home button on the right
        case vbViewOrientationLandscapeLeft:            fEndAngle = -90.f;     break;
            //4-Device oriented horizontally, home button on the left - Rotate
        case vbViewOrientationLandscapeRight:           fEndAngle = 90.f;      break;
        default:break;
    }
    m_fDeviceRot_End = fEndAngle;
    m_fProgressRatio = 0.f;
    m_bOnAnimating = true;
}

