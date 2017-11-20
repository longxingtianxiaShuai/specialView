//
//  vbPolyManager.cpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 31..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#include "vbPolyManager.h"
#include "vbPolygon.h"

vbPolyManager::vbPolyManager()
{
    m_polygons.resize(0);
}

vbPolyManager::~vbPolyManager()
{
    for (int i=0; i<m_polygons.size(); i++)
    {
        delete m_polygons[i];
        m_polygons[i] = 0;
    }
    m_polygons.resize(0);
}

void vbPolyManager::Clear()
{
    for (int i=0; i<m_polygons.size(); i++)
    {
        delete m_polygons[i];
        m_polygons[i] = 0;
    }
    m_polygons.resize(0);
}

vbPolygon*  vbPolyManager::CreatePolygon()
{
    vbPolygon* pNewPoly = new vbPolygon;
    m_polygons.push_back(pNewPoly);
    return pNewPoly;
}
