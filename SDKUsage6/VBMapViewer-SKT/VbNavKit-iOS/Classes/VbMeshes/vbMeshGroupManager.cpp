//
//  vbMeshGroupManager.cpp
//  VbNavKit-iOS
//
//  Created by fieldbim on 2015. 2. 17..
//  Copyright (c) 2015년 dev4. All rights reserved.
//

#include "vbMeshGroupManager.h"
#include <float.h>
#include "vbMeshGroup.h"
#include <iostream>

vbMeshGroupManager::vbMeshGroupManager()
{

    Clear();
}

vbMeshGroupManager::~vbMeshGroupManager()
{
    Clear();
}

//Clear
void    vbMeshGroupManager::Clear()
{
    for (int i=0; i<m_Groups.size(); i++)
    {
        vbMeshGroup* pGroup = m_Groups.at(i);
        delete pGroup;
        m_Groups[i] = NULL;
    }
    
    m_Groups.resize(0);
    m_Groups.reserve(20);
    
    m_minBB = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_maxBB = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    m_minVisibleBB = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_maxVisibleBB = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
}

//Add Group
void    vbMeshGroupManager::AddGroup(std::string  group_name, vec3 minBB, vec3 maxBB, bool bVisibility)
{
    vbMeshGroup*    pNewGroup = new vbMeshGroup;
    pNewGroup->SetGroupName(group_name);
    pNewGroup->SetBB(minBB, maxBB);
    pNewGroup->SetVisibility(bVisibility);
    
    m_Groups.push_back(pNewGroup);
}


void    vbMeshGroupManager::UpdateBB()
{
    m_minBB = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_maxBB = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    m_minVisibleBB = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_maxVisibleBB = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    
  
    vec3 min,max;
    for (int i=0; i<m_Groups.size(); i++)
    {
        vbMeshGroup* pGroup = m_Groups.at(i);
        
        min = pGroup->GetMinBB();
        max = pGroup->GetMaxBB();
        
        if (min.x < m_minBB.x)  m_minBB.x = min.x;
        if (min.y < m_minBB.y)  m_minBB.y = min.y;
        if (min.z < m_minBB.z)  m_minBB.z = min.z;
        
        if (max.x > m_maxBB.x)  m_maxBB.x = max.x;
        if (max.y > m_maxBB.y)  m_maxBB.y = max.y;
        if (max.z > m_maxBB.z)  m_maxBB.z = max.z;
        
        if(pGroup->IsVisible())
        {
            if (min.x < m_minVisibleBB.x)  m_minVisibleBB.x = min.x;
            if (min.y < m_minVisibleBB.y)  m_minVisibleBB.y = min.y;
            if (min.z < m_minVisibleBB.z)  m_minVisibleBB.z = min.z;
            
            if (max.x > m_maxVisibleBB.x)  m_maxVisibleBB.x = max.x;
            if (max.y > m_maxVisibleBB.y)  m_maxVisibleBB.y = max.y;
            if (max.z > m_maxVisibleBB.z)  m_maxVisibleBB.z = max.z;
        }
     }
}

vbMeshGroup*    vbMeshGroupManager::GetNamedGroupContaining(std::string group_name)
{
    for (int i=0; i<m_Groups.size(); i++)
    {
        vbMeshGroup* pGroup = m_Groups.at(i);
        //포함하고 있다면..
        if(pGroup->GetGroupName().find(group_name)!=std::string::npos)      return pGroup;
    }
    return NULL;
}

//Show info
void    vbMeshGroupManager::ShowInfo()
{
#ifdef VB_CORE_DEBUG
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Group Count = " << m_Groups.size() << "\r\n" << std::endl;
    
    for (int i=0; i<m_Groups.size(); i++)
    {
        vbMeshGroup* pGroup = m_Groups.at(i);
        pGroup->ShowInfo();
    }
    
    std::cout << "MinBB = [ " << m_minBB.x << " , " << m_minBB.y << " , " << m_minBB.z <<  " ] " << std::endl;
    std::cout << "MaxBB = [ " << m_maxBB.x << " , " << m_maxBB.y << " , " << m_maxBB.z <<  " ] " << std::endl;
    std::cout << "VisibleMinBB = [ " << m_minVisibleBB.x << " , " << m_minVisibleBB.y << " , " << m_minVisibleBB.z <<  " ] " << std::endl;
    std::cout << "VisibleMaxBB = [ " << m_maxVisibleBB.x << " , " << m_maxVisibleBB.y << " , " << m_maxVisibleBB.z <<  " ] " << std::endl;
    std::cout << "-------------------------------" << std::endl;
#endif
}
