//
//  vbFPSCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 6..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbFPSCameraControl_h
#define VbNavKit_iOS_vbFPSCameraControl_h

#include "vbCameraControl.hpp"

class vbFPSCameraControl : public vbCameraControl
{
    
public:
    vbFPSCameraControl();
    ~vbFPSCameraControl();
    
private:
    float       m_fEyeHeight;   //Eye height from ground
    float       m_fMovingSpeed; //Speed on FPS
    Quaternion  m_qTurnSpeed;   //Angular speed on FPS
    float       m_fEndAngle;
    
public:
    //////////////// SET/GET
    float       GetEyeHeight()                      {   return m_fEyeHeight;            };
    void        SetEyeHeight(float fEyeHeight)      {   m_fEyeHeight = fEyeHeight;      };
    float       GetMovingSpeed()                    {   return m_fMovingSpeed;          };
    void        SetMovingSpeed(float fMovingSpeed)  {   m_fMovingSpeed = fMovingSpeed;  };
    Quaternion  GetTurnSpeed()                      {   return m_qTurnSpeed;            };
    void        SetTurnSpeed(Quaternion qTurnSpeed) {   m_qTurnSpeed = qTurnSpeed;      };
    float       GetEndAngle()                       {   return m_fEndAngle;             };
    void        SetEndAngle(float fEndAngle)        {   m_fEndAngle = fEndAngle;        };
    
    //////////////// FUNCTION
    //ANIMATION
    void        UpdateWalkingAnimation(float turn_ratio, float trans_ratio);
    
public:
    Quaternion  GetOrientation();
   
};

#endif
