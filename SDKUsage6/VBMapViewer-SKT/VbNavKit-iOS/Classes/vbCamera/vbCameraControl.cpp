//
//  vbTrackCamera.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 6..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbCameraControl.hpp"
#include "vbCamera.hpp"
#include <assert.h>

vbCameraControl::vbCameraControl()
{
    m_pCamera = 0;
    
}
vbCameraControl::~vbCameraControl()
{
    m_pCamera = 0;  //외부 인스턴스이므로 여기서 삭제해서는 안된다.
}

void vbCameraControl::SetCamera(vbCamera* pCamera)
{
    assert(pCamera!=0);
    
    m_pCamera = pCamera;
}
