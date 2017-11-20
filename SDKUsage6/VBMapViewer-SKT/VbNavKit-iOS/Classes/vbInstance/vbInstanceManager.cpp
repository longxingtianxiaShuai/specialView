//
//  vbInstanceManager.cpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 25..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#include "vbInstanceManager.h"
#include "vbMeshInstance.h"

vbInstanceManager::vbInstanceManager()
{
    m_pInstances.resize(0);
}

vbInstanceManager::~vbInstanceManager()
{
    Clear();
}

vbMeshInstance* vbInstanceManager::CreateMeshInstance()
{
    vbMeshInstance* pNewInstance = new vbMeshInstance;
    pNewInstance->SetID((unsigned short)m_pInstances.size());
    
    m_pInstances.push_back(pNewInstance);
    return pNewInstance;
}

void vbInstanceManager::Clear()
{
    for (unsigned int i=0; i<m_pInstances.size(); i++)
    {
        delete m_pInstances.at(i);
        m_pInstances[i] = 0;
    }
    m_pInstances.resize(0);
}