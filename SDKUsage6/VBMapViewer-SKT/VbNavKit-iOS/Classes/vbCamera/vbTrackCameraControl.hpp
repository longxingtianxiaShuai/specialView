//
//  Header.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 6..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbTrackCameraControl_h
#define VbNavKit_iOS_vbTrackCameraControl_h

#include "vbRotateCameraControl.hpp"

class vbTrackCameraControl : public vbRotateCameraControl
{
public:
    vbTrackCameraControl();
    ~vbTrackCameraControl();
    
private:
    //Base property
    vbPlane m_ground_plane;         //카메라 중심이 위치 할 평면
    vec3    m_track_center;         //카메라가 지향 할 지점
    
//    float   m_fYawDegree;           //Y축을 회전축으로 하고, -Z방향으로부터 시계 방향으로의 회전 각도(-Y축 회전)
//    float   m_fPitchDegree;         //카메라 위치의 수평면으로부터 수직 각도(카메라가 위를 향하면 +)

    
    //Limitation property
    float   m_fPitchMinDegree;      //Pitch의 최소값
    float   m_fPitchMaxDegree;      //Pitch의 최대값
    
    
    

    
public:
    /////////////////////   Set/Get
    //Base property
    vec3    GetTrackCenter()                {   return m_track_center;      };
    void    SetTrackCenter(vec3 fCenter);
    /*
    float   GetYawDegree()                  {   return m_fYawDegree;        };
    void    SetYawDegree(float fYaw)        {   m_fYawDegree = fYaw;        };
    void    SetYawDegree(vec3 dirYaw);
    float   GetPitchDegree()                {   return m_fPitchDegree;      };
    void    SetPitchDegree(float fPitch)    {   m_fPitchDegree = fPitch;    };
     */
    
    float   GetPitchRatio();
    float   GetPitchRatio(float fCurrentPitch); //Pitch를 외부에서 관리하도록
   
    
    //Limitation property
    float   GetPitchMinDegree()                     {   return m_fPitchMinDegree;       };
    void    SetPitchMinDegree(float fMinPitch)      {   m_fPitchMinDegree = fMinPitch;  };
    float   GetPitchMaxDegree()                     {   return m_fPitchMaxDegree;       };
    void    SetPitchMaxDegree(float fMaxPitch)      {   m_fPitchMaxDegree = fMaxPitch;  };
    
    
    //Control method
    float   SetCameraPitchRatio(float fPitchRatio); //0~1사이의 값으로 Pitch min-max사이값으로 할당
    void        AddYawPitchDegree(float fDeltaYawDegree, float fDeltaPitchDegree);
    Quaternion  GetOrientation();                       //Yaw-Pitch로 쿼터니언 값을 구함.
      
    
    
    

};

#endif
