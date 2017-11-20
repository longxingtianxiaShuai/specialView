#pragma once

#include "vbMesh.hpp"
#include "vbMaterialManager.hpp"
#include "vbEdges.hpp"

#define NUM_MESH_VECTORS    4

class vbMeshManager
{
private:
	
	//Mesh
    //vector<vbMesh*>		m_Meshes;		//Will be the Mesh Manager

    vector<vbMesh*>     m_TextureMesh;
    vector<vbMesh*>     m_BlendMesh;
    vector<vbMesh*>     m_ColorMesh;
    vector<vbMesh*>     m_BlendTextureMesh;

    
    vector<vbMesh*>*    m_meshes[NUM_MESH_VECTORS];//TX(0)-CL(1)-BL(2)-BT(3)
    
    vbEdges         m_Edges;
    
    float*          m_pGoundGridVts;
    
    
	//Bounding box
	vec3	m_minBound;
	vec3	m_maxBound;
    
    //Number of Static mesh
    unsigned short  m_StaticMeshCount;
    unsigned short  m_InstanceMeshCount;

public:
	vbMeshManager();
	~vbMeshManager();
    

    
    void        ClearData();
    

	//Mesh array
	vbMesh*		CreateMesh(vbMaterial::VBMESH_TYPE type = vbMaterial::VBMESH_TEXTURE, bool bAsStatic=true);
	void		DeleteMesh();
    
    //Edges
    vbEdges*    GetEdges()      {   return &m_Edges;    }

    //	vector<vbMesh*>*		GetMeshes()		{	return &m_Meshes; }
    vector<vbMesh*>*		GetTexMeshes()		{	return &m_TextureMesh; }
    vector<vbMesh*>*		GetBlendMeshes()	{	return &m_BlendMesh; }
    vector<vbMesh*>*		GetColorMeshes()	{	return &m_ColorMesh; }
    vector<vbMesh*>*		GetBlendTextureMeshes()		{	return &m_BlendTextureMesh; }
    vector<vbMesh*>*        GetMeshes(int kind);
    
    //return - mesh type (-1 : not exist)
    int                     IsExistMesh(std::string mesh_name/*SBM_name+ID*/, vbMesh* &pCollected_Mesh);
    
    //BB
    void                    UpdateAABB(bool bUpdateMesh);
    vec3                    GetMinBound()   {   return m_minBound;  }
    vec3                    GetMaxBound()   {   return m_maxBound;  }
    vec3                    GetCenter()     {   return (m_minBound+m_maxBound)/2.f;     }  
    
    void    FreeUnusedTexCoord(vbMaterialManager* pMtlMng);     //텍스쳐 없는 재질을 쓰는데 texture 좌표를 가지고 있는 모델에서 텍스쳐 좌표 반환
    
    //Number of Static mesh
    unsigned short  GetStaticMeshCount()        {   return m_StaticMeshCount;   }
    unsigned short  GetInstanceMeshCount()      {   return m_InstanceMeshCount; }
    
    //Visibility
    void    SetGroupVisibility(bool bShow, std::string part_of_group_name);
    
    const float*    GetGridVts()    {   return (const float*)m_pGoundGridVts;   }
  
private:
    void    UpdateGroundGridWithBB();

};