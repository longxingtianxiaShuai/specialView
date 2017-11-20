//
//  vbMeshGroup.h
//  VbNavKit-iOS
//
//  Created by fieldbim on 2015. 2. 17..
//  Copyright (c) 2015년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbMeshGroup__
#define __VbNavKit_iOS__vbMeshGroup__

#include <stdio.h>

#include <string>
#include "Vector.hpp"

using std::string;


class vbMeshGroup
{
    
public:
    vbMeshGroup();
    ~vbMeshGroup();
    
private:
    std::string     m_group_name;   //Group name
    bool            m_bVisible;     //Visibility
    vec3            m_minBB;        //min bounding box
    vec3            m_maxBB;        //max bounding box
    
public:
    //Set
    void    SetGroupName(std::string  group_name)   {   m_group_name = group_name;          }//Group name
    void    SetBB(vec3 minBB, vec3 maxBB)           {   m_minBB = minBB;   m_maxBB = maxBB; }
    void    SetVisibility(bool bShowGroup)          {   m_bVisible = bShowGroup;            }
    

    //Get
    std::string     GetGroupName()  {   return m_group_name;  } //Group name
    vec3            GetMinBB()      {   return m_minBB;       } //min bounding box
    vec3            GetMaxBB()      {   return m_maxBB;       } //max bounding box
    bool            IsVisible()     {   return m_bVisible;    } //Visibility
    
    //Show info
    void    ShowInfo();
};


#endif /* defined(__VbNavKit_iOS__vbMeshGroup__) */
