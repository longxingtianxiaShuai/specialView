#include "vbMeshManager.hpp"
#include <float.h>
#include <assert.h>

vbMeshManager::vbMeshManager()
{
	m_TextureMesh.clear();
	m_BlendMesh.clear();
	m_ColorMesh.clear();
    m_BlendTextureMesh.clear();
    
    m_Edges.Clear();
    
    m_meshes[vbMaterial::VBMESH_TEXTURE] = &m_TextureMesh;
    m_meshes[vbMaterial::VBMESH_COLOR] = &m_ColorMesh;
    m_meshes[vbMaterial::VBMESH_BLEND] = &m_BlendMesh;
    m_meshes[vbMaterial::VBMESH_BLENDTEXTURE] = &m_BlendTextureMesh;
    
    
	m_minBound = vec3( FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    
    m_StaticMeshCount   = 0;
    m_InstanceMeshCount = 0;
    m_pGoundGridVts = 0;
}

vbMeshManager::~vbMeshManager()
{
	DeleteMesh();
    m_Edges.Clear();
    
    if (m_pGoundGridVts)    delete [] m_pGoundGridVts;
    m_pGoundGridVts = 0;
}

void vbMeshManager::ClearData()
{
	DeleteMesh();
    
	m_minBound = vec3( FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    if (m_pGoundGridVts)    delete [] m_pGoundGridVts;
    m_pGoundGridVts = 0;
    
    m_Edges.Clear();
    
}

vbMesh*	vbMeshManager::CreateMesh(vbMaterial::VBMESH_TYPE type, bool bAsStatic/*=true*/)
{
	vbMesh*	pNewMesh = new vbMesh();

  
    switch (type)
    {
        case vbMaterial::VBMESH_TEXTURE:
        {
           m_TextureMesh.push_back(pNewMesh); 
        }
        break;
        
        case vbMaterial::VBMESH_COLOR:
        {
            m_ColorMesh.push_back(pNewMesh); 
        }
        break;
            
        case vbMaterial::VBMESH_BLEND:
        {
            m_BlendMesh.push_back(pNewMesh); 
        }
        break;
            
        case  vbMaterial::VBMESH_BLENDTEXTURE:
        {
            m_BlendTextureMesh.push_back(pNewMesh);
        }
            break;
            
        default:
        {
            delete pNewMesh;
            return NULL;
        }
        break;
    }
    
    if(bAsStatic)   m_StaticMeshCount++;
    else            m_InstanceMeshCount++;
    
    pNewMesh->SetStaticMesh(bAsStatic);
    
	return pNewMesh;
}

vector<vbMesh*>*    vbMeshManager::GetMeshes(int kind)
{
    if(kind>=NUM_MESH_VECTORS)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "[CHECK!!] Unsupported Mesh type.!! Asked type is " << kind << std::endl;
#endif
        assert(0);
        return NULL;
    }
    
    return m_meshes[kind];
}


void	vbMeshManager::DeleteMesh()
{	
    vec3 min,max;
    
    for (int i=0; i<NUM_MESH_VECTORS; i++)
    {
        
        for (vector<vbMesh*>::iterator pMeshes = m_meshes[i]->begin(); pMeshes!=m_meshes[i]->end(); pMeshes++)
        {
            delete (*pMeshes);
            (*pMeshes) = NULL;
        }
    }
 	m_TextureMesh.clear();
	m_BlendMesh.clear();
 	m_ColorMesh.clear();
    m_BlendTextureMesh.clear();
}

void  	vbMeshManager::UpdateAABB(bool bUpdateMesh)
{
    //Mesh를 갱신하는 경우
    m_minBound = vec3( FLT_MAX, FLT_MAX, FLT_MAX);
    m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

    vbMesh* pMesh = NULL;
    vec3 min,max;
    
    for (int i=0; i<NUM_MESH_VECTORS; i++)
    {

        for (vector<vbMesh*>::iterator pMeshes = m_meshes[i]->begin(); pMeshes!=m_meshes[i]->end(); pMeshes++)
        {
            pMesh = (vbMesh*)(*pMeshes);
            if (bUpdateMesh)    pMesh->UpdateAABB();
            
            min = pMesh->GetMinBound();
            max = pMesh->GetMaxBound();
            
            //x
            if(m_minBound.x>min.x)  m_minBound.x = min.x;
            if(m_maxBound.x<max.x)  m_maxBound.x = max.x;
            
            //y
            if(m_minBound.y>min.y)  m_minBound.y = min.y;
            if(m_maxBound.y<max.y)  m_maxBound.y = max.y;
            
            //z
            if(m_minBound.z>min.z)  m_minBound.z = min.z;
            if(m_maxBound.z<max.z)  m_maxBound.z = max.z;
        }   
    
    
    }
    
    UpdateGroundGridWithBB();
    
#ifdef VB_CORE_DEBUG
    printf("Whole min-max = [%f %f %f] - [%f %f %f]\r\n",m_minBound.x,m_minBound.y,m_minBound.z,m_maxBound.x,m_maxBound.y,m_maxBound.z);
#endif

}

void    vbMeshManager::UpdateGroundGridWithBB()
{
    //X,Z는 BB를 사용하고, Y는 0인 평면에 그리드를 만든다. 일단 무조건 10등분 하는 그리드를 만든다.
    if(m_pGoundGridVts) delete [] m_pGoundGridVts;
    m_pGoundGridVts = new float[132]; //가로.세로(2) * 10칸(11) * 2개 vt(2) * 3개좌표(3)
    memset(m_pGoundGridVts, 0, sizeof(float)*132);
    
    float stepX = (m_maxBound.x - m_minBound.x) / 10.f;
    float stepZ = (m_maxBound.z - m_minBound.z) / 10.f;
    
    for (int i=0; i<11; i++)
    {
        //Along X
        ////minZ
        m_pGoundGridVts[i*6+0] = m_minBound.x + stepX * float(i);  //minZ - x
        m_pGoundGridVts[i*6+2] = m_minBound.z;                     //minZ - z
        
        ////maxZ
        m_pGoundGridVts[i*6+3] = m_minBound.x + stepX * float(i);  //maxZ - x
        m_pGoundGridVts[i*6+5] = m_maxBound.z;                     //maxZ - z
        
        
        //Along Z
        ////minX
        m_pGoundGridVts[(i+11)*6+0] = m_minBound.x;                     //minX - x
        m_pGoundGridVts[(i+11)*6+2] = m_minBound.z + stepZ * float(i);  //minX - z
        
        ////maxZ
        m_pGoundGridVts[(i+11)*6+3] = m_maxBound.x;                     //maxX - x
        m_pGoundGridVts[(i+11)*6+5] = m_minBound.z + stepZ * float(i);  //maxX - z
    }
    
    
}

//Visibility
void    vbMeshManager::SetGroupVisibility(bool bShow, std::string part_of_group_name)
{
    vbMesh* pMesh = 0;
    for (int i=0; i<NUM_MESH_VECTORS; i++)
    {
        
        for (vector<vbMesh*>::iterator pMeshes = m_meshes[i]->begin(); pMeshes!=m_meshes[i]->end(); pMeshes++)
        {
            pMesh = (vbMesh*)(*pMeshes);
            
            //포함하고 있으면...
            if(pMesh->GetGroupName().find(part_of_group_name)!=std::string::npos)
                pMesh->SetVisibility(bShow);
            
        }
    }
    
}


//텍스쳐 없는 재질을 쓰는데 texture 좌표를 가지고 있는 모델에서 텍스쳐 좌표 반환
void    vbMeshManager::FreeUnusedTexCoord(vbMaterialManager* pMtlMng)
{
    vbMesh* pMesh = NULL;

    for (int i=0; i<NUM_MESH_VECTORS; i++)//i=0 : Texture있는 모델은 그대로 유지
    {
        if(i==(int)vbMaterial::VBMESH_TEXTURE || i==(int)vbMaterial::VBMESH_BLENDTEXTURE) continue;  //텍스쳐 사용하는 메쉬들은 해당 없음.
        
        for (vector<vbMesh*>::iterator pMeshes = m_meshes[i]->begin(); pMeshes!=m_meshes[i]->end(); pMeshes++)
        {
            //Mesh pointer
            pMesh = (vbMesh*)(*pMeshes);
            pMesh->FreeTexCoordForNoneTexMesh();
        }
        
    }
    
}


int vbMeshManager::IsExistMesh(std::string mesh_name/*SBM_name+ID*/, vbMesh* &pCollected_Mesh)
{
    vbMesh* pMesh = NULL;
    
    for (int i=0; i<NUM_MESH_VECTORS; i++)//i=0 : Texture있는 모델은 그대로 유지
    {
        for (vector<vbMesh*>::iterator pMeshes = m_meshes[i]->begin(); pMeshes!=m_meshes[i]->end(); pMeshes++)
        {
            //Mesh pointer
            pMesh = (vbMesh*)(*pMeshes);
            
            //std::cout << "TEST Mesh name = " << pMesh->GetMeshName() << std::endl;
            
            if (pMesh->GetMeshName()==mesh_name)
            {
                pCollected_Mesh = pMesh;
                return i;
            }
        }
    }
#ifdef VB_CORE_DEBUG
    std::cout << "[CHECK!] Querying failed Mesh name = " << mesh_name << std::endl;
#endif
    
    pCollected_Mesh = NULL;
    return -1;//Not found
}


