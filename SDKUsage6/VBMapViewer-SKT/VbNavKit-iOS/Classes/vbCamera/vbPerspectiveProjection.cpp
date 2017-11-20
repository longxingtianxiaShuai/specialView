//
//  vbPerspectiveCamera.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 5..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbPerspectiveProjection.hpp"
#include <assert.h>
#include <iostream> //for memcpy

vbPerspectiveProjection::vbPerspectiveProjection()
{
    
}


vbPerspectiveProjection::~vbPerspectiveProjection()
{
    
}


///////////// Set/Get
//Perspective param
void vbPerspectiveProjection::SetPerspectiveParam(const float* perspective_param)
{
    assert(perspective_param!=NULL);
    
    memcpy(m_perspective_param,perspective_param,sizeof(float)*4);
}

void  vbPerspectiveProjection::SetPerspectiveParam(float fov, float aspect_ratio, float near, float far)
{
    m_perspective_param[0] = fov;
    m_perspective_param[1] = aspect_ratio;
    m_perspective_param[2] = near;
    m_perspective_param[3] = far;
}

const float* vbPerspectiveProjection::GetPerspectiveParam()
{
    return (const float*)m_perspective_param;
}

mat4 vbPerspectiveProjection::GetProjectionMatrix()
{
    return mat4::Perspective(m_perspective_param[0], m_perspective_param[1], m_perspective_param[2], m_perspective_param[3]);
}
