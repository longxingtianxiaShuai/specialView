//
//  vbFPSCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 6..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbFPSCameraControl.hpp"
#include "vbCamera.hpp"
#include <assert.h>

vbFPSCameraControl::vbFPSCameraControl()
{
    
}

vbFPSCameraControl::~vbFPSCameraControl()
{
    
}

Quaternion  vbFPSCameraControl::GetOrientation()
{
    Quaternion cam_dir;
    
    return  cam_dir;
}

void vbFPSCameraControl::UpdateWalkingAnimation(float turn_ratio, float trans_ratio)
{
    assert(m_pCamera);
    
    //////////////////////////  Turn
    if(turn_ratio>0.1f)
    {
        float normalized_ratioH = (turn_ratio - 0.1f)/0.9f;
        Quaternion cam_orbit_rot = m_pCamera->GetCameraOrientation();
        cam_orbit_rot = cam_orbit_rot.Slerp(normalized_ratioH,m_qTurnSpeed * cam_orbit_rot);
        m_pCamera->SetCameraOrientation(cam_orbit_rot);
    }
    else if(turn_ratio<-0.1f)
    {
        float normalized_ratioH = (turn_ratio + 0.1f)/0.9f;
        Quaternion cam_orbit_rot = m_pCamera->GetCameraOrientation();
        cam_orbit_rot = cam_orbit_rot.Slerp(-normalized_ratioH,m_qTurnSpeed.Inverse() * cam_orbit_rot);
        m_pCamera->SetCameraOrientation(cam_orbit_rot);
    }
    
    //////////////////////////  Forward/Backward
    
    //마우스 위치로 속도를 정한다.
     
    //입력 된 점의 위치로 전후 속도를 결정한다.
    vec3 diff = m_pCamera->GetFront() * m_fMovingSpeed * trans_ratio;
    
    //Y방향으로 전후진
    m_pCamera->MoveCamera(diff);
}
