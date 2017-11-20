//
//  vbPathManager.h
//  VbBIMRenderKit
//
//  Created by Dev4_Air on 12. 8. 21..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#ifndef VbBIMRenderKit_vbPathManager_h
#define VbBIMRenderKit_vbPathManager_h

#include    <vector>
#include "Vector.hpp"
#include "vbPath.h"



using   std::vector;


class vbPathManager
{
public:
    vbPathManager();
    ~vbPathManager();
    
    
private:
    //DEBUG용 속성 - Path와는 전혀 상관 없는데 마땅히 넣을 곳이 없어서 여기에 두는 것 뿐임.
    bool            m_bShowMyPos;
    
    bool            m_bShowPath;
    vec3            m_corrected_mypos;
    vec3            m_input_mypos;
    vec3            m_cam_cntr;
    
    //경로 정보
    vector<vbPath*>  m_vPathes;
    vbPath*          m_pVisiblePath;
    
    //색상
    float           m_fForwardPathColor[4];
    float           m_fPassedPathColor[4];
    float           m_fPathShadowColor[4];
    
 
public:
    //Base
    bool            IsPathVisible()                 {  return m_bShowPath;         }
    void            SetPathVisibility(bool bShow)   {   m_bShowPath = bShow;        }
    
    unsigned int    GetPathCount()               {   return (unsigned int)m_vPathes.size();  }
    vbPath*         GetPathwithID(int index);
    vbPath*         GetNthPath(int nth_zero_base);
    vbPath*         GetNearestPathAtFloor(float px, float py, int nFloor);
    
    vbPath*         GetVisiblePath()                {   return m_pVisiblePath;  }
    void            SetVisiblePath(int index);
    void            SetVisiblepath(vbPath* pVisiblePath)    {   m_pVisiblePath = pVisiblePath;  }
    void            SetVisiblePathAtFloor(int nFloor);
    
    //Create
    vbPath*         CreatePath();
    void            Clean();
    
    bool            IsMyPosVisible()    {   return m_bShowMyPos;    }
    void            ShowMyPos(bool bShowMyPos);
    void            SetMyPos(vec3 mypos_original, vec3 mypos_corrected, vec3 cam_cntr);
    vec3            GetMyInputPos()     {   return m_input_mypos;   }
    vec3            GetMyCorrectedPos() {   return m_corrected_mypos;   }
    vec3            GetCameraCenter()   {   return m_cam_cntr;          }
    //현재 등록 된 경로 중, 특정 그룹의 높이 범위에 속하는 부분들을 추출하여 별도의 경로로 추가함.
    //추출 된 그룹에서 다시 추출하지 않도록, 추출을 통해 만들어지지 않은 경로는 이름을 공백으로 지정함.
    int             ExtractAdditionalPathesWithGroupLevel(std::string target_grp_name,std::string grp_name, float minLevel, float maxLevel);
    
    //Color
    const float*    GetForwardPathColor()   {   return m_fForwardPathColor; }
    const float*    GetPassedPathColor()    {   return m_fPassedPathColor;  }
    const float*    GetPathShadowColor()    {   return m_fPathShadowColor;  }
    
    void            SetForwardPathColorRGBA(const float* pColor);
    void            SetPassedPathColorRGBA(const float* pColor);
    void            SetPathShadowColorRGBA(const float* pColor);
    
    //그룹
    void            SetGroupPathColor(std::string target_grp_name, const float* pColor);
    void            SetGroupPathVisibility(std::string target_grp_name, bool bShow, bool bExclusive);
    
    //Visibility
    // bExclusive가 true이면 다른 floor는 반대로 하고, false이면 그냥 원래 값 그대로 둔다.
    void            SetVisibilityWithFloorID(int floorID, bool bShow, bool bExclusive);
    
    

};

#endif
