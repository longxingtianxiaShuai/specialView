//
//  vbPolygon.h
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 30..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbPolygon__
#define __VbNavKit_iOS__vbPolygon__

#include <iostream>
#include <string>
#include <vector>

struct vbRing;
class vbPolygon
{
public:
    vbPolygon();
    ~vbPolygon();
    
private:
    std::string     m_name;
    unsigned char   m_RGBA[4];//256 --> Mesh가 될 때 0~1로 변경 된다.
    
    vbRing*                 m_Outer;
    std::vector<vbRing*>    m_Inners;
    
public:
    void            Clear();
    //Name
    std::string     GetName()                   {   return m_name;  }
    void            SetName(std::string name)   {   m_name = name;  }
    
    //RGBA
    unsigned char*  RGBA()                  {   return m_RGBA;                          };
    const unsigned char*    GetRGBA()       {   return (const unsigned char*)m_RGBA;    };
    void            SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        m_RGBA[0]=r;    m_RGBA[1]=g;    m_RGBA[2]=b;    m_RGBA[3]=a;
    };
    
    //Ring
    vbRing*     GetOuter()                      {   return m_Outer;     }
    void        SetOuter(vbRing* pOuter)        {   m_Outer = pOuter;   }
    std::vector<vbRing*>*       GetInners()     {   return &m_Inners;   }
    void        AddInner(vbRing* pInnerRing);
    
};

#endif /* defined(__VbNavKit_iOS__vbPolygon__) */
