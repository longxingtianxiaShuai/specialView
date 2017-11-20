//
//  vbAnimationCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 12..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbAnimationCameraControl__
#define __VbNavKit_iOS__vbAnimationCameraControl__

#include "Quaternion.hpp"
#include "vbCameraControl.hpp"

enum    vbViewDirection
{
    vbViewOrientationUnknown=0,
    vbViewOrientationPortrait,            // Device oriented vertically, home button on the bottom
    vbViewOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
    vbViewOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
    vbViewOrientationLandscapeRight,      // Device oriented horizontally, home button on the left - Rotate angle(90)
    vbViewOrientationFaceUp,              // Device oriented flat, face up
    vbViewOrientationFaceDown             // Device oriented flat, face down
};


class vbAnimationCameraControl : public vbCameraControl
{
    
public:
    vbAnimationCameraControl();
    ~vbAnimationCameraControl();
    
    
private:
    //POSITION & ORIENTATION
    vec3        m_Start_pos;
    vec3        m_End_pos;
    Quaternion  m_Start_orient;
    Quaternion  m_End_orient;
    
    //DEVICE ROTATION
    float       m_fDeviceRot_Start;
    float       m_fDeviceRot_End;
    
    //YAW
    float       m_fYawStep;
   
    //INTERPOLATION
    float       m_fProgressRatio;
    float       m_fProgressStep;
    
    //STATE
    bool        m_bUseAnimation;
    bool        m_bOnAnimating;

 
    
public:
    //////////// SET/GET
    //POSITION
    vec3        GetStartPosition()                  {   return m_Start_pos;         };
    void        SetStartPosition(vec3 pos_start)    {   m_Start_pos = pos_start;    };
    vec3        GetEndPosition()                    {   return m_End_pos;           };
    void        SetEndPosition(vec3 pos_end)        {   m_End_pos = pos_end;        };

    //YAW
    float       GetYawStep()                        {   return m_fYawStep;          };
    void        SetYawStep(float fYawStep)          {   m_fYawStep = fYawStep;      };
    
    
    //ORIENTATION
    Quaternion  GetStartOrientation()                           {   return m_Start_orient;          };
    void        SetStartOrientation(Quaternion orient_start)    {   m_Start_orient = orient_start;  };
    Quaternion  GetEndOrientation()                             {   return m_End_orient;            };
    void        SetEndOrientation(Quaternion orient_end)        {   m_End_orient = orient_end;      };
 
    
    //DEVICE ROTATION
    float       GetDeviceRotationStartAngle()                   {   return m_fDeviceRot_Start;  };
    void        SetDeviceRotationStartAngle(float fStartAngle)  {   m_fDeviceRot_Start = fStartAngle;   };
    float       GetDeviceRotationEndAngle()                     {   return m_fDeviceRot_End;  };
    void        SetDeviceRotationEndAngle(float fEndAngle)      {   m_fDeviceRot_End = fEndAngle;   };
    
    
    
    
    //INTERPOLATION
    float       GetProgressRatio()              {   return m_fProgressRatio;    };
    void        SetProgressRatio(float fRatio)  {   m_fProgressRatio = fRatio;  };
    float       GetProgressStep()               {   return m_fProgressStep;     };
    void        SetProgressStep(float fStep)    {   m_fProgressStep = fStep;    };
   
    //STATE
    bool        IsAnimationUsed()           {   return m_bUseAnimation;     };
    void        UseAnimation(bool bUse)     {   m_bUseAnimation = bUse;     };
    bool        IsOnAnimating()             {   return m_bOnAnimating;      };
    void        SetAnimating(bool bAnimate) {   m_bOnAnimating = bAnimate;  };
    
    
    
    
    ///////////// FUNCTION
    //INITIALIZE
    void        InitAnimation(vec3 start_pos, vec3 end_pos, Quaternion start_orient, Quaternion end_orient);
    void        InitDeviceRotationAnimation(float fFromAngle, vbViewDirection eEndOrientation);
    
    //Interpolation
    vec3        GetInterpolatedPosition(float ratio_from_start);
    float       GetInterpolatedDeviceAngle(float ratio_from_start);
    Quaternion  GetSlerpOrientation(float ratio_from_start);
    
    //PROGRESS
    float       IncreaseRatio(int nStep=1);
    
};

#endif /* defined(__VbNavKit_iOS__vbAnimationCameraControl__) */
