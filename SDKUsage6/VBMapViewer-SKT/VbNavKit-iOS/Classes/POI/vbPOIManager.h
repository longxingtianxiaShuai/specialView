//
//  vbPOI.h
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 10. 25..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PISAiOS_vbPOIManager_h
#define PISAiOS_vbPOIManager_h

#include    <vector>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "vbPOI.h"



using   std::vector;
class vbMe;



class vbPOIManager
{
public:
    vbPOIManager();
    ~vbPOIManager();

    
private:
    
   //POI
    vector<vbPOI*>  m_vPOIs;
    
    vbPOI           m_pPOIStart;
    vbPOI           m_pPOIEnd;
    
    //Render buffer
    float*          m_pPOIPositions;
    
    //Update flag
    bool            m_bUpdated; //Rendering 이전에 변경되었으면 true, 
    
public:
    //Base
    unsigned int    GetPOICount()               {   return (unsigned int)m_vPOIs.size();  }
    vbPOI*          GetPOIwithIndex(unsigned int index);
    vbPOI*          GetPOIwithPOIID(int poi_id);
    vbPOI*          GetPOIwithKey(char* dbKey);
    vector<vbPOI*>* GetPOIvector()              {   return &m_vPOIs;        }
    
    vbPOI*      GetPOI(int mapID, int poiID);
    
    //Create
    vbPOI*          CreatePOI();    
    void            Clean();
    
    //Start/End
    vbPOI*          GetStartPOI()       {   return &m_pPOIStart;    }
    vbPOI*          GetEndPOI()         {   return &m_pPOIEnd;      }
    
    
    //2D projection
    void            Update2DPos(mat4* pModelview, mat4* pProjection, ivec2 viewport, vbMe* pMe=NULL);
    
    //Rendering
    void            RebuildRenderingBuffer();
    float*          POI3DPositionBuffer()               {   return m_pPOIPositions; }
    
    //Updated flag
    bool            IsUpdated()                         {   return m_bUpdated;      }
    void            SetUpdatedFlag(bool bUpdated)       {   m_bUpdated = bUpdated;  }
      
};


#endif
