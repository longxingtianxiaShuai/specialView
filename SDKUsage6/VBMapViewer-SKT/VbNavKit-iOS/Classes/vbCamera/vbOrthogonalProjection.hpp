//
//  vbOrthogonalProjection.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 13..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbOrthogonalProjection__
#define __VbNavKit_iOS__vbOrthogonalProjection__

#include    "vbICameraProjection.hpp"

class vbOrthogonalProjection : public vbICameraProjection
{
public:
    vbOrthogonalProjection();
    ~vbOrthogonalProjection();
    
private:
    //Orthogonal
    float   m_orthogonal_param[6];      //[0] - left,   [1] - right,    [2] - bottom
                                        //[3] - top,    [4] - near,     [5] - far
    
public:
    ///////////// Set/Get
    //Perspective param
    void            SetOrthogonalParam(const float* orthogonal_param);
    void            SetOrthogonalParam(float left, float right, float bottom, float top, float near, float far);
    const float*    GetOrthogonalParam();
    
    
    ///////////// Projection
    mat4            GetProjectionMatrix();
};

#endif /* defined(__VbNavKit_iOS__vbOrthogonalProjection__) */
