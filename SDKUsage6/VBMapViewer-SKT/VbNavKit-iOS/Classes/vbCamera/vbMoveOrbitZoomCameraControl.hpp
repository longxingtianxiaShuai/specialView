//
//  vbZoomCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbMoveOrbitZoomCameraControl_h
#define VbNavKit_iOS_vbMoveOrbitZoomCameraControl_h

#include "vbMoveCameraControl.hpp"

class vbMoveOrbitZoomCameraControl : public vbMoveCameraControl
{
    
public:
    vbMoveOrbitZoomCameraControl();
    ~vbMoveOrbitZoomCameraControl();
 
private:
    //Base property
    float   m_fDistance;            //카메라에서 카메라 방향으로 카메라 중심까지 거리(음수면 회전 중심을 뒤에 둔 것임)
    float   m_fBaseDistance;        //확대/축소의 기준이 되는 거리(확대/축소 시작 시점의 거리)
    float   m_fBaseRatio;           //확대/축소의 기준이 되는 비(ex. 두 터치 좌표 사이의 거리)
    
    //Limitation property
    float   m_fDistanceMin;         //Distance의 최소값
    float   m_fDistanceMax;         //Distance의 최대값

    
public:
    vec3    GetPosition();
    
    /////////////////////   Set/Get
    //Base property
    float   GetDistance()                           {   return m_fDistance;                 };
    void    SetDistance(float fDistance)            {   m_fDistance = fDistance;            };
    float   GetBaseDistance()                       {   return m_fBaseDistance;             };
    void    SetBaseDistance(float fBaseDistance)    {   m_fBaseDistance = fBaseDistance;    };
    float   GetBaseRatio()                          {   return m_fBaseRatio;                };
    void    SetBaseRatio(float fBaseRatio)          {   m_fBaseRatio = fBaseRatio;          };
    
    //Limitation property
    float   GetDistanceMin()                        {   return m_fDistanceMin;          };
    void    SetDistanceMin(float fDistanceMin)      {   m_fDistanceMin = fDistanceMin;  };
    float   GetDistanceMax()                        {   return m_fDistanceMax;          };
    void    SetDistanceMax(float fDistanceMax)      {   m_fDistanceMax = fDistanceMax;  };
    
    //Control function
    void    StartZoomWithRatio(float fInitRatio);
    void    UpdateDistanceWithBaseDistanceRatio(float fNewRatio);
    void    UpdateDistanceFromBaseDistPlusBaseDistRatio(float fNewRatio);
    
    void    SetDistanceAsMAX()          {   m_fDistance = m_fDistanceMax;   };
    void    SetDistanceAsMIN()          {   m_fDistance = m_fDistanceMin;   };
    void    ToggleDistanceLimit();      //Max * 0.9보다 멀면 Min으로, 그렇지 않으면 MAX로 변경
    
};

#endif
