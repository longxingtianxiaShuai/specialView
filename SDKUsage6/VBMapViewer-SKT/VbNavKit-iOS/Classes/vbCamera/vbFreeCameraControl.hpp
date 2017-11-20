//
//  vbExaminCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbExaminCameraControl__
#define __VbNavKit_iOS__vbExaminCameraControl__

#include "vbRotateCameraControl.hpp"

class vbFreeCameraControl : public vbRotateCameraControl
{
public:
    vbFreeCameraControl();
    ~vbFreeCameraControl();
    
private:
    float       m_fVBallRadius;
    Quaternion  m_Orientation;
    vec3        m_trackball_start;
        
public:
    Quaternion  GetOrientation();
    
    void        SetOrientation(Quaternion orientation)  {   m_Orientation = orientation;    };
    void        AddOrientation(vec3 cur_trackball_pos);
    
    vec3        GetTrackballStart()                 {   return m_trackball_start;       };
    void        SetTrackballStart(vec3 start_pt)    {   m_trackball_start = start_pt;   };
    
    
    float       GetVBallRadius()                    {   return m_fVBallRadius;      };
    void        SetVBallRadius(float fVBallRad)     {   m_fVBallRadius = fVBallRad; };
    

};

#endif /* defined(__VbNavKit_iOS__vbExaminCameraControl__) */
