//
//  vbColorCoder.cpp
//  VbNavKit-iOS
//
//  Created by Wonil Shim on 2013. 12. 24..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbColorCoder.hpp"

#include <assert.h>
#include <vector>

vbColorCoder::vbColorCoder()
{
    m_pMeshManager = 0;
}

vbColorCoder::~vbColorCoder()
{
    m_pMeshManager = 0;
}

//Return value : mesh_index 순서에 있는 Mesh의 ID (ID와 index는 다를 수 있다.)
unsigned short  vbColorCoder::GetColorCode_of_NthMesh(unsigned short mesh_index,//INPUT - MeshManager에서의 순서
                                                      vbMaterial::VBMESH_TYPE   mesh_type,  //INPUT - Mesh의 종류
                                        unsigned char *rgb)  //OUT - 변환코드
{
    //Mesh Manager는 꼭 있어야 한다.
    assert(m_pMeshManager);
    

    std::vector<vbMesh*>*  pMeshes = m_pMeshManager->GetMeshes((unsigned char)mesh_type);
   
    //반드시 적합한 타입이라야 한다.
    assert(pMeshes);
    
    //메쉬 배열의 개수보다 큰 인덱스가 넘어오면 안된다.
    assert(mesh_index<pMeshes->size());
    
    unsigned short mesh_id = pMeshes->at(mesh_index)->GetID();
    
    //unsigned short의 개수를 넘어가는 대상을 피킹으로 찾는 것은 설계가 잘못된 것
    rgb[0] = mesh_index%256;     mesh_index /= 256;
    rgb[1] = mesh_index%256;     mesh_index /= 256;
    rgb[2] = mesh_type;

    
    /*
        VBMESH_TEXTURE  =   0,
        VBMESH_COLOR    =   1,
        VBMESH_BLEND    =   2
    */

    return mesh_id;
}

//칼라를 넣어서 메쉬를 얻어오는 함수
vbMesh*     vbColorCoder::GetCodedMesh(unsigned char r, unsigned char g, unsigned char b)
{
    if(r==255 && g==255 && b==255) return NULL;
    
    //지정 된 타입이면..
    assert((vbMaterial::VBMESH_TYPE)b==vbMaterial::VBMESH_TEXTURE
           || (vbMaterial::VBMESH_TYPE)b==vbMaterial::VBMESH_COLOR
           || (vbMaterial::VBMESH_TYPE)b==vbMaterial::VBMESH_BLEND
           || (vbMaterial::VBMESH_TYPE)b==vbMaterial::VBMESH_BLENDTEXTURE
           );
    
    std::vector<vbMesh*>*  pMeshes = m_pMeshManager->GetMeshes(b);
    //메쉬 배열이 없으면 안됨
    assert(pMeshes);
    
    /*
    unsigned int cnt0 =m_pMeshManager->GetMeshes(0)->size();
    unsigned int cnt1 =m_pMeshManager->GetMeshes(1)->size();
    unsigned int cnt2 =m_pMeshManager->GetMeshes(2)->size();
    */
    
    unsigned int mesh_count = (unsigned int)pMeshes->size();
    if(mesh_count==0)   return NULL;
    
    unsigned short mesh_index = g*256 + r;
    //mesh index가 메쉬 배열의 개수보다 크거나 같으면 안됨.
    assert(mesh_index<mesh_count);
    
    return pMeshes->at(mesh_index);
}
