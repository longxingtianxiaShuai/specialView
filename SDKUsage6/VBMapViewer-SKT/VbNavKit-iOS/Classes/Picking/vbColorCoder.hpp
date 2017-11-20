//
//  vbColorCoder.h
//  VbNavKit-iOS
//
//  Created by Wonil Shim on 2013. 12. 24..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbColorCoder__
#define __VbNavKit_iOS__vbColorCoder__

#include <iostream>
#include "vbMeshManager.hpp"

class vbColorCoder
{
public:
    vbColorCoder();
    ~vbColorCoder();
    
private:
    vbMeshManager*  m_pMeshManager;
    
    
public:
    vbMeshManager*   GetMeshManager()    {   return m_pMeshManager;      }
    void             SetMeshManager(vbMeshManager*  pMeshManager)    {   m_pMeshManager = pMeshManager;  }
    
    //Return value : mesh_index 순서에 있는 Mesh의 ID (ID와 index는 다를 수 있다.)
    unsigned short  GetColorCode_of_NthMesh(unsigned short mesh_index,//INPUT - MeshManager에서의 순서
                                            vbMaterial::VBMESH_TYPE   mesh_type,//INPUT - Mesh의 종류
                                            unsigned char *rgb);  //OUT - 변환코드
    //Return value : Picking 된 메쉬의 포인터
    vbMesh*     GetCodedMesh(unsigned char r, unsigned char g, unsigned char b);
    
};

#endif /* defined(__VbNavKit_iOS__vbColorCoder__) */
