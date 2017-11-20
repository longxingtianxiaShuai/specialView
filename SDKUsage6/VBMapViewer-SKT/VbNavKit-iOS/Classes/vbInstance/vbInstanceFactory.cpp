//
//  vbInstanceFactory.cpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 29..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#include "vbInstanceFactory.h"
#include "../vbMeshes/vbMesh.hpp"
#include <assert.h>

//Mesh Instance Generator
VBMapErrorCode vbInstanceFactory::ImportMeshInstanceFromIST(vbInstanceManager* pInstMng, vbMeshManager* pMeshMng, FILE* fp)
{
    char    str[64];
    
    //VB_INSTANCE_DESC_VER
    fscanf(fp,"%s",str);
    int     version = 0;
    fscanf(fp,"%d", &version);
    
    switch (version)
    {
        case 1:
            return ImportMeshInstanceFromIST_V1(pInstMng,pMeshMng,fp);
        break;
            
        default:
            break;
    }
    
    return VBMapError_UNSUPORRTED_VERSION;
}

VBMapErrorCode vbInstanceFactory::ImportMeshInstanceFromIST_V1(vbInstanceManager* pInstMng, vbMeshManager* pMeshMng, FILE* fp)
{
    bool bDebug = false;
    
    //Version까지 읽은 상태
    char    str[64];
    
    //NUM_INSTANCE
    int nNumInstance = 0;
    memset(str,0,sizeof(char)*64);  fscanf(fp, "%s",str);   fscanf(fp, "%d",&nNumInstance);

    //Instance
    char    strSBM[128],instanceName[128];
    float   fMat[16];
    vbMesh* pLinkedMesh = 0;
    for (int i=0; i<nNumInstance; i++)
    {
        //INSTANCE_ID
        int nInstanceID = 0;
        memset(str,0,sizeof(char)*64);  fscanf(fp, "%s",str);   fscanf(fp, "%d",&nInstanceID);
#ifdef VB_CORE_DEBUG
        if(bDebug)  std::cout << str << " - " << nInstanceID << std::endl;
#endif
        //MESH_REFERENCE
        memset(strSBM,0,sizeof(char)*128);
        memset(str,0,sizeof(char)*64);  fscanf(fp, "%s",str);   fscanf(fp, "%s",strSBM);
#ifdef VB_CORE_DEBUG
        if(bDebug)  std::cout << str << " - " << strSBM << std::endl;
#endif
        //MESH_ID
        int nMeshID = 0;
        memset(str,0,sizeof(char)*64);  fscanf(fp, "%s",str);   fscanf(fp, "%d",&nMeshID);
#ifdef VB_CORE_DEBUG
        if(bDebug)  std::cout << str << " - " << nMeshID << std::endl;
 #endif
        //MAT
        memset(fMat, 0, sizeof(float)*16);
        memset(str,0,sizeof(char)*64);  fscanf(fp, "%s",str);
#ifdef VB_CORE_DEBUG
        if(bDebug)  std::cout << str << " - ";
#endif
        for(int k=0;k<16;k++)
        {
            fscanf(fp, "%f",&(fMat[k]));
#ifdef VB_CORE_DEBUG
            if(bDebug)  std::cout << fMat[k] << " - ";
#endif
        }
#ifdef VB_CORE_DEBUG
        if(bDebug)  std::cout << std::endl;
#endif
        //Verify reference information
        memset(instanceName,0,sizeof(char)*128);
        sprintf(instanceName, "%s_%d",strSBM,nMeshID);
        if(pMeshMng->IsExistMesh(std::string(instanceName), pLinkedMesh)<0)
        {
            //없는 메쉬
            pLinkedMesh = NULL;
            return VBMapError_REFER_IMPORPER_MESH;
        }
        assert(pLinkedMesh);
      
        //Create & Assign Instance properties
        vbMeshInstance* pNewInst = pInstMng->CreateMeshInstance();//ID를 Instance들의 일련번호가 부여 됨.
        pNewInst->SetName(std::string(instanceName));   //Mesh의 이름
        pNewInst->SetMesh(pLinkedMesh);
        pNewInst->SetMat((const float*)fMat);
        pLinkedMesh->AddMeshInstance(pNewInst);
        
        pNewInst = 0;
        pLinkedMesh = 0;
    }
    
    return VBMapError_NO_ERROR;
}


int vbInstanceFactory::AddMeshInstance(const char* sbm_name,
                                      unsigned short mesh_ID,
                                      const float* mat,
                                      vbInstanceManager* pInstMng,
                                      vbMeshManager* pMeshMng
                                      )
{
    //Mandatory parameters
    assert(sbm_name);
    assert(mat);
    assert(pInstMng);
    assert(pMeshMng);
    
    //Verify reference information
    char instanceName[128];
    memset(instanceName,0,sizeof(char)*128);
    sprintf(instanceName, "%s_%d",sbm_name,(int)mesh_ID);

    vbMesh* pLinkedMesh = NULL;
    if(pMeshMng->IsExistMesh(std::string(instanceName), pLinkedMesh)<0)
    {
        //없는 메쉬
        pLinkedMesh = NULL;
        return -1;
    }
    
    assert(pLinkedMesh);
    
    //Create & Assign Instance properties
    vbMeshInstance* pNewInst = pInstMng->CreateMeshInstance();//ID를 Instance들의 일련번호가 부여 됨.
    pNewInst->SetName(std::string(instanceName));   //Mesh의 이름
    pNewInst->SetMesh(pLinkedMesh);
    pNewInst->SetMat((const float*)mat);
    pLinkedMesh->AddMeshInstance(pNewInst);

    int new_inst_id = (int)pNewInst->GetID();
    
    pNewInst = 0;
    pLinkedMesh = 0;
    
    return new_inst_id;

}






