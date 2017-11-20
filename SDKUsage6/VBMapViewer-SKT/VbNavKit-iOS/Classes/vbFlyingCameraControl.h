//
//  vbFreeCameraControl.h
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 8. 5..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbFreeCameraControl__
#define __VbNavKit_iOS__vbFreeCameraControl__

#include <iostream>
#include "vbMoveCameraControl.hpp"

class vbFlyingCameraControl : public vbMoveCameraControl
{
public:
    vbFlyingCameraControl();
    ~vbFlyingCameraControl();
    
private:
    //vbCamera*       m_pCamera;                        //from vbCameraControl
        /*
         vec3        m_cam_position;
         vec3        m_cam_lookat;
         Quaternion  m_cam_orientation;
         
         float       m_cam_yaw_degree;
         float       m_cam_pitch_degree;
         */
    
public:
    ////////////////// Set/Get
    //void        SetCamera(vbCamera* pCamera);         //from vbCameraControl
    

    //Position
    vec3    GetPosition();                              //from vbMoveCameraControl
    
    void    SetPose(vec3 new_pos, float fYaw, float fPitch);
    void    SetPose(vec3 new_pos, Quaternion qOrient);
    
    
};

#endif /* defined(__VbNavKit_iOS__vbFreeCameraControl__) */
