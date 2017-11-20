//
//  vbOrthogonalProjection.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 13..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include <iostream>
#include "vbOrthogonalProjection.hpp"

vbOrthogonalProjection::vbOrthogonalProjection()
{
    memset(m_orthogonal_param, 0, sizeof(float)*6);
}

vbOrthogonalProjection::~vbOrthogonalProjection()
{
    
}

void vbOrthogonalProjection::SetOrthogonalParam(const float* orthogonal_param)
{
    memcpy(m_orthogonal_param, orthogonal_param, sizeof(float)*6);
}

void vbOrthogonalProjection::SetOrthogonalParam(float left, float right, float bottom, float top, float near, float far)
{
    m_orthogonal_param[0] = left;
    m_orthogonal_param[1] = right;
    m_orthogonal_param[2] = bottom;
    m_orthogonal_param[3] = top;
    m_orthogonal_param[4] = near;
    m_orthogonal_param[5] = far;
}

const float*  vbOrthogonalProjection::GetOrthogonalParam()
{
    return (const float*)m_orthogonal_param;
}


///////////// Projection
mat4  vbOrthogonalProjection::GetProjectionMatrix()
{
    return mat4::Frustum(m_orthogonal_param[0], m_orthogonal_param[1], m_orthogonal_param[2], m_orthogonal_param[3], m_orthogonal_param[4], m_orthogonal_param[5]);
}