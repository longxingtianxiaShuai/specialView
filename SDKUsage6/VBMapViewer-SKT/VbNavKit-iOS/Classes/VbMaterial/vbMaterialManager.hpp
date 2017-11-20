#pragma once
#include "vbMaterial.hpp"
#include <vector>

using std::vector;

class vbMaterialManager
{
public:
    vbMaterialManager();
    ~vbMaterialManager();
    
private:
    vector<vbMaterial*>  m_Mtls;
    
public:
    vector<vbMaterial*>*     GetMaterials()                    {   return &m_Mtls;          }
    void                     AddMaterial(vbMaterial* pMatl)    {   m_Mtls.push_back(pMatl); }
    void                     DeleteAllMaterials();
  	vbMaterial*              CreateMaterial(vbMaterial::VBMATL_VER  matl_ver);
    
    unsigned short           GetMaterialCount()                 {  return m_Mtls.size();    }
    vbMaterial*              GetMaterial(std::string strMtlID);
    vbMaterial::VBMESH_TYPE  GetMaterialType(std::string strMtlID);
    
    vbMaterial*             IsExistTextureMaterial(char* pName, unsigned char name_len, vbMaterial* pRefMtl);
    
    void    ShowInfo();
};