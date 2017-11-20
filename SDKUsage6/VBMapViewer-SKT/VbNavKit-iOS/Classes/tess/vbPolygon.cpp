//
//  vbPolygon.cpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 30..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#include "vbPolygon.h"
#include "../vbEnum.hpp"

vbPolygon::vbPolygon()
{
    m_name="";
    memset(m_RGBA,0,sizeof(unsigned char)*4);
    
    m_Outer = NULL;
    m_Inners.resize(0);
    
}

vbPolygon::~vbPolygon()
{
    Clear();
}

void vbPolygon::Clear()
{
    if(m_Outer)     delete m_Outer;
    m_Outer= NULL;
    
    for (int i=0; i<m_Inners.size(); i++)
    {
        if(m_Inners.at(i))     delete m_Inners[i];
        m_Inners[i]=NULL;
    }
    m_Inners.resize(0);
}

void vbPolygon::AddInner(vbRing* pInnerRing)
{
    m_Inners.push_back(pInnerRing);
}