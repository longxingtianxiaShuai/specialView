

#include "vbMaterialManager.hpp"
#include "vbMeshManager.hpp"

#include "Vector.hpp"

vbMaterialManager::vbMaterialManager()
{
    m_Mtls.clear();
}

vbMaterialManager::~vbMaterialManager()
{
    DeleteAllMaterials();
}

void    vbMaterialManager::ShowInfo()
{
#ifdef VB_CORE_DEBUG
    std::cout <<"ID,MATL_VER,bTex,GLID,Facing,TexName,TexNameLen,MatlType,AmbR,AmbG,AmbB,AmbA,DiffR,DiffG,DiffB,DiffA,SpcR,SpcG,SpcB,SpcA" << std::endl;
#endif
    
    for (vector<vbMaterial*>::iterator pMtl = m_Mtls.begin(); pMtl!=m_Mtls.end(); pMtl++)
    {
        vbMaterial* pMaterial = (*pMtl);
        
        pMaterial->ShowInfo();
    }
}

vbMaterial* vbMaterialManager::CreateMaterial(vbMaterial::VBMATL_VER matl_ver)
{
 	vbMaterial*	pNewMtl = 0;
    
    switch (matl_ver)
    {
        case vbMaterial::VBMATL_VER_V2:
            pNewMtl =   (vbMaterial*)new vbMaterial_V2();
        break;
            
        default:
        {
#ifdef VB_CORE_DEBUG
            std::cout << "[VBMAP-ERROR] Unsupported Material Version (Version = " << matl_ver << std::endl;
#endif
            return NULL;
        }
        break;
    }

	m_Mtls.push_back(pNewMtl);
	return pNewMtl;   
}

vbMaterial*  vbMaterialManager::IsExistTextureMaterial(char* pName, unsigned char name_len, vbMaterial* pRefMtl)
{
    if(m_Mtls.size()==0)    return NULL;
    
    for (vector<vbMaterial*>::iterator pMtl = m_Mtls.begin(); pMtl!=m_Mtls.end(); pMtl++)
    {
        vbMaterial* pMaterial = (*pMtl);
        
        if (pRefMtl==pMaterial) continue;   //자기 자신이면 제외
        
        if(pMaterial->GetTextureNameLength()!=name_len)    continue;
        
        //텍스쳐 이름의 길이가 같고, 내용도 같으면 해당하는 텍스쳐로 판단
        if(strcmp(pName, pMaterial->GetTextureName())==0)
        {
#ifdef VB_CORE_DEBUG
            std::cout << "Ref Name = " << pName << ", ExistingName = " << pMaterial->GetTextureName() << std::endl;
#endif
            return pMaterial;
        }
    }
    
    return NULL;
}

void vbMaterialManager::DeleteAllMaterials()
{
    for (vector<vbMaterial*>::iterator pMtl = m_Mtls.begin(); pMtl!=m_Mtls.end(); pMtl++)
    {
        vbMaterial* pMaterial = (*pMtl);
        
        if(pMaterial)   delete (pMaterial);
        (*pMtl) = NULL;
    }
    m_Mtls.clear();
}

vbMaterial* vbMaterialManager::GetMaterial(std::string strMtlID)
{
    for (vector<vbMaterial*>::iterator pMtl = m_Mtls.begin(); pMtl!=m_Mtls.end(); pMtl++)
    {
        if( (*pMtl)->GetID()==strMtlID)
        {
             return (*pMtl);
        }
    }
#ifdef VB_CORE_DEBUG
    std::cout << "Not matched Matl ID = " << strMtlID << std::endl;
#endif

    return NULL;
}

vbMaterial::VBMESH_TYPE vbMaterialManager::GetMaterialType(std::string strMtlID)
{
    for (vector<vbMaterial*>::iterator pMtl = m_Mtls.begin(); pMtl!=m_Mtls.end(); pMtl++)
    {
        if( (*pMtl)->GetID()==strMtlID)
        {
            return (*pMtl)->GetMaterialType();
            
            /*
            else
            {
                float* diff = (*pMtl)->GetDiffuse();
                if(diff[3]<0.99f)       return vbMeshManager::VBMESH_BLEND;
                else    break;
            }
             */
        }
    } 
    return vbMaterial::VBMESH_COLOR;

}
