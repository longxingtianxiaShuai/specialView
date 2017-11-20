//
//  vbCamera.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 5..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbCamera_h
#define VbNavKit_iOS_vbCamera_h

#include "Quaternion.hpp"
#include "vbConfiguration.hpp"
#include "vbGeometry.hpp"

#include <vector>

using std::vector;

class vbCamera
{
public:
    vbCamera();
    ~vbCamera();
    
private:
    //서로 서로 얽혀있는 관계라서 하나를 넣어주면 다른 것도 업데이트 해줘야 함.
    vec3        m_cam_position;
    vec3        m_cam_lookat;
    Quaternion  m_cam_orientation;
    
    float       m_cam_yaw_degree;
    float       m_cam_pitch_degree;
    
    
public:
    
    ///////////////////////// Set/Get
    
    vec3        GetCameraPosition()                 {   return  m_cam_position;     };
    void        SetCameraPosition(vec3 pos)         {   m_cam_position = pos;       };
    vec3        GetCameraLookAt()                   {   return  m_cam_lookat;       };
    void        SetCameraLookAt(vec3 pos)           {   m_cam_lookat = pos;         };

    Quaternion  GetCameraOrientation()              {   return m_cam_orientation;   };
    void        SetCameraOrientation(Quaternion orient) {   m_cam_orientation = orient; };
    void        SetCameraOrientation(float fYaw, float fPitch);
    void        UpdateCameraOrientation();
    
    float       GetYawDegree()                  {   return m_cam_yaw_degree;        };
    float       GetPitchDegree()                {   return m_cam_pitch_degree;      };
    void        SetYawDegree(float fYaw);
    void        SetYawDegree(vec3 dirYaw);
    void        SetPitchDegree(float fPitch);
    
    /////////////////////////// Orientation
    vec3        GetUp();
    vec3        GetFront();
    vec3        GetRight();
    
    //////////////////////////// ABSTRACT
    //Projection
    //virtual mat4 GetProjectionMatrix() = 0;
    //virtual mat4 GetModelViewMatrix() = 0;
    
    ///////////////////////////  FUNCTION
    void        MoveCamera(vec3 pos_diff);

    
};
#endif
