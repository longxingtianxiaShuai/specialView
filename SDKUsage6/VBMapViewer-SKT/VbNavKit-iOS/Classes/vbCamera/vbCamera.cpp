//
//  vbCamera.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 5..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbCamera.hpp"
#include <iostream>
#include <cmath>

vbCamera::vbCamera()
{
    m_cam_pitch_degree  = 0.f;
    m_cam_yaw_degree    = 0.f;
}

vbCamera::~vbCamera()
{
    
}

void vbCamera::MoveCamera(vec3 pos_diff)
{
    m_cam_position += pos_diff;
    m_cam_lookat += pos_diff;
}


vec3 vbCamera::GetUp()
{
    return m_cam_orientation * VEC3_UNITY;
}

vec3 vbCamera::GetFront()
{
    return m_cam_orientation * (VEC3_UNITZ * (-1.f) ) ;
}

vec3 vbCamera::GetRight()
{
    return m_cam_orientation * VEC3_UNITX;
}

void vbCamera::SetYawDegree(vec3 dirYaw)
{
    //dirYaw는 XZ평면에 있으며, -Z로부터 시계 방향의 Degree 단위 각도를 의미한다.
    
    //XZ평면의 단위 벡터로 설정
    dirYaw.y = 0;
    dirYaw.Normalize();
    
    if(dirYaw.size()<0.0000001)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "Imporper dir" << std::endl;
#endif
        return;
    }
    
    //사이 각도는 내적을 이용함.
    vec3 nZ = vec3(0.f,0.f,-1.f);
    float theta_rad = acos(nZ.Dot(dirYaw));    //두 벡터는 각각 단위 벡터이므로 크기는 1이다. 따라서 나눠줄 필요가 없다.
    
    //acos의 리턴값은 0~PI이므로, 부호 판단을 해야 하는데, 이는 dirYaw의 x좌표로 하면 된다.
    m_cam_yaw_degree = RAD2DEG(theta_rad);
    if(dirYaw.x<0)  m_cam_yaw_degree = -m_cam_yaw_degree;
    
    UpdateCameraOrientation();
}

void  vbCamera::SetCameraOrientation(float fYaw, float fPitch)
{
    m_cam_yaw_degree = fYaw;
    m_cam_pitch_degree = fPitch;
    UpdateCameraOrientation();
}

void  vbCamera::UpdateCameraOrientation()
{
   m_cam_orientation = Quaternion::CreateFromNegativeZtoCWYawAndPitchDegree(m_cam_yaw_degree,m_cam_pitch_degree); 
}

void  vbCamera::SetYawDegree(float fYaw)
{
    m_cam_yaw_degree = fYaw;
    UpdateCameraOrientation();
}

void  vbCamera::SetPitchDegree(float fPitch)
{
    m_cam_pitch_degree = fPitch;
    UpdateCameraOrientation();
}
