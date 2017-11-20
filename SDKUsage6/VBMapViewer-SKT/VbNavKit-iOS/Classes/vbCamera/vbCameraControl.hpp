//
//  vbTrackCamera.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 6..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbCameraControl_h
#define VbNavKit_iOS_vbCameraControl_h

#include "Quaternion.hpp"
#include "vbGeometry.hpp"

class vbCamera;

class vbCameraControl
{
public:
    vbCameraControl();
    ~vbCameraControl();
    
protected:
    vbCamera*       m_pCamera;
    
    
public:
    ////////////////// Set/Get
    void        SetCamera(vbCamera* pCamera);
    
};

#endif
