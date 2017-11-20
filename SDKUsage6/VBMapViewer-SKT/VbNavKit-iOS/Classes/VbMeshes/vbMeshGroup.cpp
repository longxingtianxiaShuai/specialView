//
//  vbMeshGroup.cpp
//  VbNavKit-iOS
//
//  Created by fieldbim on 2015. 2. 17..
//  Copyright (c) 2015년 dev4. All rights reserved.
//

#include "vbMeshGroup.h"
#include <float.h>
#include <iostream>

vbMeshGroup::vbMeshGroup()
{
    m_group_name="";
    m_bVisible = true;
    m_minBB = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_maxBB = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
}

vbMeshGroup::~vbMeshGroup()
{
    
}

//Show info
void    vbMeshGroup::ShowInfo()
{
#ifdef VB_CORE_DEBUG
    std::cout << "Group Name = " << m_group_name << std::endl;

    if(m_bVisible)  std::cout << "Visibility = TRUE" << std::endl;
    else            std::cout << "Visibility = FALSE" << std::endl;
    
    std::cout << "MinBB = [ " << m_minBB.x << " , " << m_minBB.y << " , " << m_minBB.z <<  " ] " << std::endl;
    std::cout << "MaxBB = [ " << m_maxBB.x << " , " << m_maxBB.y << " , " << m_maxBB.z <<  " ] " << std::endl;
    std::cout << "-------------------------------" << std::endl;
#endif
}