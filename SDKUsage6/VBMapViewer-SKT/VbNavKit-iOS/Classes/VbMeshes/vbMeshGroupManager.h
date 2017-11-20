//
//  vbMeshGroupManager.h
//  VbNavKit-iOS
//
//  Created by fieldbim on 2015. 2. 17..
//  Copyright (c) 2015년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbMeshGroupManager__
#define __VbNavKit_iOS__vbMeshGroupManager__

#include <stdio.h>
#include <vector>
#include "Vector.hpp"

using std::vector;

class vbMeshGroup;
class vbMeshGroupManager
{
    
public:
    vbMeshGroupManager();
    ~vbMeshGroupManager();
    
private:
    std::vector<vbMeshGroup*>   m_Groups;

    vec3            m_minBB;        //min bounding box
    vec3            m_maxBB;        //max bounding box
    
    vec3            m_minVisibleBB;
    vec3            m_maxVisibleBB;
    
public:
    std::vector<vbMeshGroup*>*      GetGroups()     {   return &m_Groups;   }
    
    //Clear
    void    Clear();
    
    //Add Group
    void    AddGroup(std::string  group_name, vec3 minBB, vec3 maxBB, bool bVisibility);
    
    //Get Group
    vbMeshGroup*    GetNamedGroupContaining(std::string group_name);
    
    //BB
    vec3    GetMinBB()  {   return m_minBB; }
    vec3    GetMaxBB()  {   return m_maxBB; }
    vec3    GetMinVisibleBB()  {   return m_minVisibleBB; }
    vec3    GetMaxVisibleBB()  {   return m_maxVisibleBB; }
    void    UpdateBB();
    
    //info
    void    ShowInfo();
   
};
#endif /* defined(__VbNavKit_iOS__vbMeshGroupManager__) */
