//
//  vbPathManager.cpp
//  VbBIMRenderKit
//
//  Created by Dev4_Air on 12. 8. 21..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#include "vbPathManager.h"
#include <float.h>

vbPathManager::vbPathManager()
{
    m_vPathes.resize(0);
    m_vPathes.reserve(50);
    m_pVisiblePath=NULL;
    
    
    m_bShowMyPos = false;
    
    m_bShowPath = false;
    

    m_fForwardPathColor[0] = 0.96f;    m_fForwardPathColor[1] = 0.58f;    m_fForwardPathColor[2] = 0.11f;    m_fForwardPathColor[3] = 1.f;
    
    m_fPassedPathColor[0] = 0.6f;    m_fPassedPathColor[1] = 0.6f;    m_fPassedPathColor[2] = 0.6f;    m_fPassedPathColor[3] = 1.f;
    
    m_fPathShadowColor[0] = 0.2f;    m_fPathShadowColor[1] = 0.2f;    m_fPathShadowColor[2] = 0.2f;    m_fPathShadowColor[3] = 0.7f;

}

vbPathManager::~vbPathManager()
{
    Clean();
}

vbPath* vbPathManager::CreatePath()
{
    vbPath*  newPath = new vbPath;
    m_vPathes.push_back(newPath);
    return newPath;
}

void vbPathManager::Clean()
{
    for (auto  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr))   delete (*pItr);
        (*pItr) = NULL;
    }
    m_vPathes.clear();
    m_vPathes.resize(0);
    
    m_pVisiblePath=NULL;

}

void  vbPathManager::SetVisiblePath(int index)
{
    if(index<0)
    {
        m_pVisiblePath=NULL;
        return;
    }
    
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetPathID()==index)
        {
            m_pVisiblePath = (*pItr);
            return;
        }
    }
    m_pVisiblePath=NULL;
}

void  vbPathManager::SetVisiblePathAtFloor(int nFloor)
{
    if(nFloor==0 && m_vPathes.size()>0)//전체 경로
    {
        m_pVisiblePath=m_vPathes.at(0);
        return;
    }
    
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetFloorID()==nFloor)
        {
            m_pVisiblePath = (*pItr);
            return;
        }
    }
    m_pVisiblePath=NULL;
}

vbPath*   vbPathManager::GetPathwithID(int index)
{
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetPathID()==index)
        {
            return (*pItr);
        }
    }
    return NULL;
}


vbPath*   vbPathManager::GetNthPath(int nth_zero_base)
{
    if (m_vPathes.size()<=nth_zero_base) {
        return NULL;
    }
    
    return m_vPathes.at(nth_zero_base);
}

vbPath*   vbPathManager::GetNearestPathAtFloor(float px, float py, int nFloor)
{
    vbPath* pNearestCandidates = NULL;
    int onSameFloorPathCount = 0;

    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetFloorID()==nFloor)
        {
            if(nFloor==0)  return (*pItr); //전체 경로는 하나 뿐이므로 무조건 같으면 리턴
            
            onSameFloorPathCount++;
            pNearestCandidates = (*pItr);
        }
    }
    
    
    if(pNearestCandidates!=NULL && onSameFloorPathCount==1) //같은 층에 하나 뿐인 경우
        return pNearestCandidates;
    else if(pNearestCandidates==NULL)   return NULL;        //해당 층에 데이터가 없다면 거리 구할 것도 없다.
    
    //거리를 구하는 연산이 상대적으로 비싸고, 한층에 여러개의 경로가 있는 경우가 많지 않기 때문에 다른 방법으로 먼저 걸러내고 마지막에 거리를 구하도록 한다.
    vec3 pt(px,0.f,-py);
    float minDist = FLT_MAX;
    float dist = FLT_MAX;
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetFloorID()==nFloor)
        {
            
            dist = (*pItr)->GetNearestNodeDistance(pt);
            if(dist<minDist)
            {
                minDist = dist;
                pNearestCandidates = (*pItr);
            }
        }
    }
    return pNearestCandidates;
}


void vbPathManager::ShowMyPos(bool bShowMyPos)
{
    m_bShowMyPos = bShowMyPos;
}

void vbPathManager::SetMyPos(vec3 mypos_original, vec3 mypos_corrected, vec3 cam_cntr)
{
    m_input_mypos = mypos_original;
    m_corrected_mypos = mypos_corrected;
    m_cam_cntr = cam_cntr;
}

void vbPathManager::SetForwardPathColorRGBA(const float* pColor)
{
    memcpy(m_fForwardPathColor, pColor, sizeof(float)*4);
}

void vbPathManager::SetPassedPathColorRGBA(const float* pColor)
{
    memcpy(m_fPassedPathColor, pColor, sizeof(float)*4);
    
}

void vbPathManager::SetPathShadowColorRGBA(const float* pColor)
{
    memcpy(m_fPathShadowColor, pColor, sizeof(float)*4);
    
}

//현재 등록 된 경로 중, 특정 그룹의 높이 범위에 속하는 부분들을 추출하여 별도의 경로로 추가함.
int  vbPathManager::ExtractAdditionalPathesWithGroupLevel(std::string target_grp_name,std::string grp_name, float minLevel, float maxLevel)
{
    int numPath = 0;
    
    return 0;
    
    vector<vbPath*>  vNewPathes;
    vNewPathes.resize(0);
    vNewPathes.reserve(m_vPathes.size());
   
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetGroupName().find(target_grp_name)!=std::string::npos)//있으면...
        {
            //범위 확인
            //적어도 이 Path와 겹치는 부분이 있어야 추출 할 수 있으므로 겹치지 않으면 대상이 아니다.
            if( (*pItr)->GetMinBound().y > maxLevel || (*pItr)->GetMaxBound().y < minLevel ) continue;
            
            //겹치는 부분이 있는 경로임.
            (*pItr)->ExtractPartialPathWithLevel(&vNewPathes, minLevel, maxLevel);
            
            
            
            
            
            
        }
    }
    return numPath;
}

// bExclusive가 true이면 다른 floor는 반대로 하고, false이면 그냥 원래 값 그대로 둔다.
void vbPathManager::SetVisibilityWithFloorID(int floorID, bool bShow, bool bExclusive)
{
#ifdef VB_CORE_DEBUG
    int nShow = bShow ? 1 : 0;
    int nExclusive = bExclusive ? 1 : 0;
    printf("[DEBUG] SetPathVisibilityWithFloorID - FloorID(%d), bShow(%d), bExclusive(%d)\r\n",floorID, nShow, nExclusive);
    printf(" -- Result \r\n");
#endif
    
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        //대상 Floor이면...
        if((*pItr)->GetFloorID()==floorID)
        {
            (*pItr)->SetVisibility(bShow);
        }
        //대상 Floor는 아닌데, 배타적인 설정으로 되어 있다면 반대로 설정 해 줘야 한다.
        else if(bExclusive==true)
        {
            (*pItr)->SetVisibility(!bShow);
        }
        //대상 Floor가 아니고, 배타적인 설정이 아니면 그냥 있는 그대로 둔다.
        
#ifdef VB_CORE_DEBUG
        if( (*pItr)->IsVisible())
            printf(" -- ID[%d], FloorID[%d], GroupName[%s] - SHOW\r\n", (*pItr)->GetPathID(), (*pItr)->GetFloorID(), (*pItr)->GetGroupName().c_str());
        else
            printf(" -- ID[%d], FloorID[%d], GroupName[%s] - HIDE\r\n", (*pItr)->GetPathID(), (*pItr)->GetFloorID(), (*pItr)->GetGroupName().c_str());
#endif
        
    }
    

}


//그룹 경로의 색상 설정
void vbPathManager::SetGroupPathColor(std::string target_grp_name, const float* pColor)
{
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetGroupName().find(target_grp_name)!=std::string::npos)//있으면...
        {
            (*pItr)->SetColorRGBA(pColor[0], pColor[1], pColor[2], pColor[3]);
        }
    }
}

void    vbPathManager::SetGroupPathVisibility(std::string target_grp_name, bool bShow, bool bExclusive)
{
#ifdef VB_CORE_DEBUG
    int nShow = bShow ? 1 : 0;
    int nExclusive = bExclusive ? 1 : 0;
    printf("[DEBUG] SetGroupPathVisibility - GroupName(%s), bShow(%d), bExclusive(%d)\r\n",target_grp_name.c_str(), nShow, nExclusive);
    printf(" -- Result \r\n");
#endif
    
    for (std::vector<vbPath*>::iterator  pItr = m_vPathes.begin(); pItr!=m_vPathes.end(); pItr++)
    {
        if((*pItr)->GetGroupName().find(target_grp_name)!=std::string::npos)//있으면...
        {
            (*pItr)->SetVisibility(bShow);
        }
        //대상 Group이 아닌데, 배타적인 설정으로 되어 있다면 반대로 설정 해 줘야 한다.
        else if(bExclusive==true)
        {
            (*pItr)->SetVisibility(!bShow);
        }
        //대상 그룹이 아니고, 배타적인 설정이 아니면 그냥 있는 그대로 둔다.
#ifdef VB_CORE_DEBUG
        if( (*pItr)->IsVisible())
            printf(" -- ID[%d], FloorID[%d], GroupName[%s] - SHOW\r\n", (*pItr)->GetPathID(), (*pItr)->GetFloorID(), (*pItr)->GetGroupName().c_str());
        else
            printf(" -- ID[%d], FloorID[%d], GroupName[%s] - HIDE\r\n", (*pItr)->GetPathID(), (*pItr)->GetFloorID(), (*pItr)->GetGroupName().c_str());
#endif
    }
}


