//
//  vbCameraProjection.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 5..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbCameraProjection_h
#define VbNavKit_iOS_vbCameraProjection_h

#include "Quaternion.hpp"

class vbICameraProjection
{
    
public:
    vbICameraProjection() {};
    ~vbICameraProjection() {};
    
protected:
    //Viewport
    ivec2     m_screen_size;            //[0] - width, [1] - height

public:
    //Virtual
    virtual mat4            GetProjectionMatrix() = 0;
    
    ///////////////// SET/GET
    ivec2   GetScreenSize()                 {   return m_screen_size;       };
    void    SetScreenSize(ivec2 scr_size)   {   m_screen_size = scr_size;   };

};


#endif
