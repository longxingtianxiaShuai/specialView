//
//  vbRotateCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbRotateCameraControl_h
#define VbNavKit_iOS_vbRotateCameraControl_h

#include "vbCameraControl.hpp"

class vbRotateCameraControl : public vbCameraControl
{

public:
    vbRotateCameraControl();
    ~vbRotateCameraControl();
    
    
public:
    virtual Quaternion  GetOrientation() = 0;

};
#endif
