//
//  vbPOI.cpp
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 10. 25..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "vbPOIManager.h"
#include "vbMe.h"

vbPOIManager::vbPOIManager()
{
    m_pPOIPositions = NULL;
    m_bUpdated = true;
    
    m_pPOIStart.SetPOIState(vbPOI::vbPOI_STATE_INACTIVATED);
    m_pPOIEnd.SetPOIState(vbPOI::vbPOI_STATE_INACTIVATED);    
    
}

vbPOIManager::~vbPOIManager()
{
    Clean();
}

vbPOI* vbPOIManager::GetPOIwithIndex(unsigned int index)
{
    if (index>=m_vPOIs.size())
    {
        return NULL;
    }    
    return m_vPOIs.at(index);
}

vbPOI* vbPOIManager::GetPOIwithPOIID(int poi_id)
{
    for (std::vector<vbPOI*>::iterator  pItr = m_vPOIs.begin(); pItr!=m_vPOIs.end(); pItr++)
    {
        int _poi_id = (*pItr)->GetPOIID();
        if(_poi_id==poi_id) return (*pItr);
    }
    return NULL;
}

vbPOI* vbPOIManager::GetPOI(int mapID, int poiID)
{
    int _poi_id, _map_id;
    for (std::vector<vbPOI*>::iterator  pItr = m_vPOIs.begin(); pItr!=m_vPOIs.end(); pItr++)
    {
        _poi_id = (*pItr)->GetPOIID();
        _map_id = (*pItr)->GetMapID();
        
        if(_poi_id==poiID && _map_id==mapID) return (*pItr);
    }
    return NULL;
}

vbPOI* vbPOIManager::GetPOIwithKey(char* dbKey)
{
    if (dbKey==NULL) {
        return NULL;
    }
    
    char* poi_key = NULL;
    for (std::vector<vbPOI*>::iterator  pItr = m_vPOIs.begin(); pItr!=m_vPOIs.end(); pItr++)
    {
        if(strcmp(dbKey, poi_key)==0)   return (*pItr);
    }   
    return NULL;
}

//POI 계산할 때 한꺼번에 계산함.
void vbPOIManager::Update2DPos(mat4* pModelview, mat4* pProjection, ivec2 viewport, vbMe* pMe)
{
    //if (m_vPOIs.size()==0)        return;  //내위치/시작점/출발점 등이 있으므로 항상 갱신한다.
         
    mat4 prj_mat = (*pModelview) * (*pProjection);
    vec3 prt_pt;
    ivec2 win_pt;
    std::vector<vbPOI*>::iterator  pItr = m_vPOIs.begin();
    float px,py,pz;
    for (; pItr!=m_vPOIs.end(); pItr++)
    {
        prt_pt = prj_mat * (*pItr)->Get3DPos();

        px = (viewport.x * (prt_pt.x + 1.f))/2.f;
        py = viewport.y - (viewport.y * (prt_pt.y + 1.f))/2.f;  //window coordinate변환을 위해 Y방향 Flip
        pz = ((prt_pt.z + 1.f)/2.f);
        
        win_pt.x = int(px/pz);
        win_pt.y = int(py/pz);


        (*pItr)->Set2DPos(win_pt);
    }
    
    if(pMe)
    {
        prt_pt = prj_mat * pMe->GetPosition();
        win_pt.x = (int)((viewport.x * (prt_pt.x + 1.f))/2.f);
        win_pt.y = viewport.y - (int)((viewport.y * (prt_pt.y + 1.f))/2.f);  //window coordinate변환을 위해 Y방향 Flip
        pMe->Set2DPosition(win_pt);
    }
    
    if(m_pPOIStart.GetPOIState()==vbPOI::vbPOI_STATE_ACTIVATED)
    {
        prt_pt = prj_mat * m_pPOIStart.Get3DPos();
        win_pt.x = (int)((viewport.x * (prt_pt.x + 1.f))/2.f);
        win_pt.y = viewport.y - (int)((viewport.y * (prt_pt.y + 1.f))/2.f);  //window coordinate변환을 위해 Y방향 Flip
        m_pPOIStart.Set2DPos(win_pt);
    }
    if(m_pPOIEnd.GetPOIState()==vbPOI::vbPOI_STATE_ACTIVATED)
    {
        prt_pt = prj_mat * m_pPOIEnd.Get3DPos();
        win_pt.x = (int)((viewport.x * (prt_pt.x + 1.f))/2.f);
        win_pt.y = viewport.y - (int)((viewport.y * (prt_pt.y + 1.f))/2.f);  //window coordinate변환을 위해 Y방향 Flip
        m_pPOIEnd.Set2DPos(win_pt);
    }

}

vbPOI* vbPOIManager::CreatePOI()
{
    vbPOI*  newPOI = new vbPOI;
    m_vPOIs.push_back(newPOI);
    return newPOI;
}

void vbPOIManager::Clean()
{
    for (std::vector<vbPOI*>::iterator  pItr = m_vPOIs.begin(); pItr!=m_vPOIs.end(); pItr++)
    {
        if((*pItr))   delete (*pItr);
        (*pItr) = NULL;
    }  
    m_vPOIs.clear();
    
    if(m_pPOIPositions) delete m_pPOIPositions;
    m_pPOIPositions = NULL;
    
    m_bUpdated = false;

}

void vbPOIManager::RebuildRenderingBuffer()
{
    if(m_pPOIPositions) delete m_pPOIPositions;
    m_pPOIPositions = NULL;
    
    unsigned short POI_count = m_vPOIs.size();
    if(POI_count==0)   return;
    
    m_pPOIPositions = new float[POI_count*3];
    for (int i=0; i<POI_count; i++)
    {
        memcpy(&m_pPOIPositions[i*3],(m_vPOIs.at(i))->Get3DPos().Pointer(),sizeof(float)*3);
    }
}
