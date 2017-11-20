//
//  vbMoveCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbMoveCameraControl__
#define __VbNavKit_iOS__vbMoveCameraControl__

#include "vbCameraControl.hpp"

class vbMoveCameraControl : public vbCameraControl
{
    
public:
    vbMoveCameraControl();
    ~vbMoveCameraControl();
    
    
public:
    virtual vec3  GetPosition() = 0;
    
};
#endif /* defined(__VbNavKit_iOS__vbMoveCameraControl__) */
