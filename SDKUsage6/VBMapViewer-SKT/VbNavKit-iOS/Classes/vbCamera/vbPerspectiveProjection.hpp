//
//  vbPerspectiveCamera.hpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 5..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbPerspectiveCamera_hpp
#define VbNavKit_iOS_vbPerspectiveCamera_hpp

#include    "vbICameraProjection.hpp"

class vbPerspectiveProjection : public vbICameraProjection
{
public:
    vbPerspectiveProjection();
    ~vbPerspectiveProjection();
    
private:
    float       m_perspective_param[4];
   
public:
    ///////////// Set/Get
    //Perspective param
    void            SetPerspectiveParam(const float* perspective_param);
    void            SetPerspectiveParam(float fov, float aspect_ratio, float near, float far);
    const float*    GetPerspectiveParam();
    
    
    ///////////// Projection
    mat4            GetProjectionMatrix();
};

#endif
