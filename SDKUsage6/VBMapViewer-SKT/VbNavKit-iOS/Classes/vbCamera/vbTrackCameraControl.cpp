//
//  vbTrackCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 6..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbTrackCameraControl.hpp"
#include <iostream>

#include "vbCamera.hpp"

vbTrackCameraControl::vbTrackCameraControl()
{
    m_track_center  = vec3(0.f,0.f,0.f); 
    m_ground_plane.SetPlane(vbPlane::PLN_P_ZX, 0.f);
    
    m_fPitchMinDegree   = -80.f;
    m_fPitchMaxDegree   = -10.f;
}

vbTrackCameraControl::~vbTrackCameraControl()
{
    
}

void    vbTrackCameraControl::SetTrackCenter(vec3 fCenter)
{
    m_track_center = fCenter;
    
    if (m_pCamera)
    {
        m_pCamera->SetCameraLookAt(m_track_center);
    }
}

//0~1사이의 값으로 Pitch min-max사이값으로 할당
float  vbTrackCameraControl::SetCameraPitchRatio(float fPitchRatio)
{
    if(fPitchRatio > 1.f)          fPitchRatio = 0.999f;
    else if(fPitchRatio < 0.f)     fPitchRatio = 0.001f;
    
    float fPitchDegree = m_fPitchMinDegree + (m_fPitchMaxDegree - m_fPitchMinDegree) * fPitchRatio;
    
    m_pCamera->SetPitchDegree(fPitchDegree);
    return fPitchDegree;
}

float vbTrackCameraControl::GetPitchRatio()
{
    float fPitchDegree = m_pCamera->GetPitchDegree();
    return (fPitchDegree - m_fPitchMinDegree) / (m_fPitchMaxDegree - m_fPitchMinDegree);
}

//Pitch를 외부에서 관리하는 경우
float vbTrackCameraControl::GetPitchRatio(float fCurrentPitch)
{
    return (fCurrentPitch - m_fPitchMinDegree) / (m_fPitchMaxDegree - m_fPitchMinDegree);
}


//Yaw-Pitch로 쿼터니언 값을 구함.
Quaternion vbTrackCameraControl::GetOrientation()
{
    m_pCamera->UpdateCameraOrientation();
    return m_pCamera->GetCameraOrientation();
}

void vbTrackCameraControl::AddYawPitchDegree(float fDeltaYawDegree, float fDeltaPitchDegree)
{
    float fYaw_degree = m_pCamera->GetYawDegree() + fDeltaYawDegree;
    if(fYaw_degree<-180.f)      fYaw_degree = 360.f - fYaw_degree;
    else if(fYaw_degree>180.f)  fYaw_degree = -360.f + fYaw_degree;
    
    float fPitchDegree= m_pCamera->GetPitchDegree() + fDeltaPitchDegree;
        
    if(fPitchDegree > m_fPitchMaxDegree)          fPitchDegree = m_fPitchMaxDegree;
    else if(fPitchDegree < m_fPitchMinDegree)     fPitchDegree = m_fPitchMinDegree;
    
    //Set orientation
    if (m_pCamera)
    {
        m_pCamera->SetCameraOrientation(fYaw_degree, fPitchDegree);
    }
}
