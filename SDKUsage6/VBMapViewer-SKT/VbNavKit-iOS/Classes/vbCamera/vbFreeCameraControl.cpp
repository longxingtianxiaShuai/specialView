//
//  vbExaminCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbFreeCameraControl.hpp"
#include "vbCamera.hpp"

vbFreeCameraControl::vbFreeCameraControl()
{
    
}

vbFreeCameraControl::~vbFreeCameraControl()
{
    
}

Quaternion  vbFreeCameraControl::GetOrientation()
{
    return m_Orientation;
}

void  vbFreeCameraControl::AddOrientation(vec3 cur_trackball_pos)
{
    Quaternion delta = Quaternion::CreateFromVectors(cur_trackball_pos,m_trackball_start);
    m_Orientation.Rotate(delta);
    m_trackball_start = cur_trackball_pos;
    
    //Set camera orientation
    m_pCamera->SetCameraOrientation(m_Orientation);
}
