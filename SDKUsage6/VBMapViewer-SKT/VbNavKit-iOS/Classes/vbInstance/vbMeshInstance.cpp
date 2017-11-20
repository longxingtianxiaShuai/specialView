//
//  vbMeshInstance.cpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 25..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#include "vbMeshInstance.h"
#include "vbMesh.hpp"

vbMeshInstance::vbMeshInstance()
{
    m_pMesh = NULL;
}

vbMeshInstance::~vbMeshInstance()
{
    m_pMesh = NULL;
}

void vbMeshInstance::SetMat(const float* pMat)
{
    memcpy(m_Mat, pMat, sizeof(float)*16);
}
