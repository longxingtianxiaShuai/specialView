//
//  vbMeshInstance.h
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 25..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbMeshInstance__
#define __VbNavKit_iOS__vbMeshInstance__

#include <string>

class   vbMesh;
class   vbMeshInstance
{
public:
    vbMeshInstance();
    ~vbMeshInstance();
    
private:
	//Identification
	std::string		m_name;
    unsigned short  m_id;
    
    //Mesh
    vbMesh*         m_pMesh;
    float           m_Mat[16];
    
	
public:
	//Identification
	std::string		GetName()                       {   return m_name;  }
	void            SetName(std::string	name)       {   m_name = name;  }
    
    unsigned short  GetID()                             {   return m_id;        }
    void            SetID(unsigned short instance_id)   {   m_id = instance_id; }
    
    //Mesh
    vbMesh*         GetMesh()               {   return m_pMesh;     }
    void            SetMesh(vbMesh* pMesh)  {   m_pMesh = pMesh;    }
    
    //Transform matrix
    const float*    GetMat() const                  {   return (const float*)m_Mat; }   //for rendering
    float*          MatPointer()                    {   return m_Mat;   }               //For binary reading
    void            SetMat(const float* pMat);
    
    
	
    
    
};


#endif /* defined(__VbNavKit_iOS__vbMeshInstance__) */
