//
//  vbFreeCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 8. 5..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#include "vbFlyingCameraControl.h"
#include "vbCamera.hpp"
#include <assert.h>

vbFlyingCameraControl::vbFlyingCameraControl()
{
    m_pCamera = NULL;
}

vbFlyingCameraControl::~vbFlyingCameraControl()
{
    m_pCamera = NULL;
}

vec3    vbFlyingCameraControl::GetPosition()//from vbMoveCameraControl
{
    assert(m_pCamera);
    
    return m_pCamera->GetCameraPosition();
}

//1인칭뷰를 위해 Path Guider를 사용한다면, 위치와 Yaw값이 전달된다. 
void    vbFlyingCameraControl::SetPose(vec3 new_pos, float fYaw, float fPitch)
{
    assert(m_pCamera);

    m_pCamera->SetCameraPosition(new_pos);
    m_pCamera->SetCameraOrientation(fYaw, fPitch);
}

void    vbFlyingCameraControl::SetPose(vec3 new_pos, Quaternion qOrient)
{
    assert(m_pCamera);
    
    m_pCamera->SetCameraPosition(new_pos);
    m_pCamera->SetCameraOrientation(qOrient);
}