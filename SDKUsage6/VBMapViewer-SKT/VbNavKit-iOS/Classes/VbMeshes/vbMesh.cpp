#include "vbMesh.hpp"
#include "vbMeshManager.hpp"
#include <iostream>
#include "vbMeshInstance.h"
#include <assert.h>

vbMesh::vbMesh()
{
    //Properties
	m_MeshName      = "Empty_Mesh";
    m_GroupName     = "";
 	m_solid_color   = vec4(1.f,1.f,1.f,1.f);
    m_bByteIndex    = false;
	m_minBound = vec3( FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    
    //Linked ID
	m_VBOID[0]= m_VBOID[1] = 0;
	m_MaterialID		= "";
	
    //Buffer
    m_fcCount = 0;
	m_vtCount = 0;
    m_texcoords         = NULL;
    m_vt_array_buffer   = NULL;
	m_indices           = NULL;		//실제 index 정보
    m_indicesB          = NULL;
    
    //Instance
    m_bHasInstance  = false;
    m_bIsStatic     = false;
    m_aMeshInstances.resize(0);
    
    m_bShow = true;
}

vbMesh::~vbMesh()
{
    ClearBuffer();
    m_aMeshInstances.resize(0); //메쉬가 없어지기 전에 instance를 따로 지울 일이 없으니까.
}

void vbMesh::ClearBuffer()
{
    if(m_vt_array_buffer)   delete [] m_vt_array_buffer;
	if(m_indices)           delete [] m_indices;		//실제 index 정보
    if(m_indicesB)          delete [] m_indicesB;
    
    m_vt_array_buffer   = NULL;
	m_indices           = NULL;		//실제 index 정보
    m_indicesB          = NULL;
    
    m_texcoords         = NULL;     //texture coords.는 vt_array_buffer와 함께 붙어서 만들어지며, 이 포인터는 가리키는 용도로만 쓰이기 때문에 delete를 하지 않는 것임.
    
    //메쉬가 없어지기 전에 버퍼를 VBO로 전환 할 경우, 버퍼만 지우지 인스턴스는 남겨놔야 하기 때문에 여기서 인스턴스도 지우지는 않는다.
}


size_t vbMesh::GetVertexArrayBufferSizeInByte()
{
    if(m_material_type==vbMaterial::VBMESH_TEXTURE || m_material_type==vbMaterial::VBMESH_BLENDTEXTURE)//Texture = Pos(3)+Normal(3)+Tex(2)
        return sizeof(float) * m_vtCount * (3+3+2);
    else//Color = Pos(3)+Normal(3)
        return sizeof(float) * m_vtCount * (3+3);
}

size_t vbMesh::GetVertexBufferSizeInByte()
{
	return	m_vtCount * sizeof(float) * 3; // Vertex = float X 3
}

size_t vbMesh::GetNormalBufferSizeInByte()
{
	return	m_vtCount * sizeof(float) * 3; // Normal = float X 3
}

size_t vbMesh::GetTexCoordBufferSizeInByte()
{
	return	m_vtCount * sizeof(float) * 2; // TexCoord = float X 2
}

size_t vbMesh::GetIndexBufferSizeInByte()
{
	return	m_fcCount * sizeof(unsigned short) * 3; // Triangle = unsigned short X 3
}

size_t vbMesh::GetByteIndexBufferSizeInByte()
{
	return	m_fcCount * sizeof(unsigned char) * 3; // Triangle = unsigned char X 3
}


float* vbMesh::GetVertexArrayBuffer()
{
    return m_vt_array_buffer;
}

float* vbMesh::GetVertexBuffer()
{
//	return	m_vertices;
    return	&m_vt_array_buffer[0];
}

float* vbMesh::GetNormalBuffer()
{
//	return	m_normals;
    return	&m_vt_array_buffer[m_vtCount*3];
}

float* vbMesh::GetTexCoordBuffer()
{
// Texture가 없을 수도 있기 때문에, 별도 변수를 둬서 텍스쳐가 있을 경우, 이 포인터가 해당 위치를 가리키도록 한다.
    if(m_material_type!=vbMaterial::VBMESH_TEXTURE && m_material_type!=vbMaterial::VBMESH_BLENDTEXTURE)//Texture
    {
#ifdef VB_CORE_DEBUG
        std::cout << "[WARNNING]Try to use texture coords of color mesh. Please free tex. coords of color mesh.\r\n";
#endif
        return NULL;
    }
	return	m_texcoords;
}

void  vbMesh::FreeTexCoordForNoneTexMesh()
{
    if((m_material_type!=vbMaterial::VBMESH_TEXTURE && m_material_type!=vbMaterial::VBMESH_BLENDTEXTURE) && m_texcoords)
    {
        delete [] m_texcoords;
        m_texcoords = NULL;
#ifdef VB_CORE_DEBUG
        std::cout << "[CHECK!!!] Tex. coords are not allocated, and it is justing pointing. It has not to be dealloc.\r\n";
        std::cout << "[CHECK!!!] Mesh ID = " << GetMeshName() << std::endl;
#endif
    }
}

unsigned short*	 vbMesh::GetIndexBuffer()
{
	return	m_indices;
}

unsigned char* vbMesh::GetByteIndexBuffer()
{
    return m_indicesB;
}

vec3    vbMesh::GetCenter()
{
    return (m_minBound+m_maxBound)/2.f;     
}

void	vbMesh::InitAABB()
{
	m_minBound = vec3( FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
}

void	vbMesh::UpdateAABB()
{
    if(m_vtCount==0 || m_vt_array_buffer==NULL)    return;
    
	m_minBound = vec3( FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

    float vx,vy,vz;
    for (int k=0;k<m_vtCount;k++)
    {
        vx = m_vt_array_buffer[3*k+0];
        vy = m_vt_array_buffer[3*k+1];
        vz = m_vt_array_buffer[3*k+2];
        
        //항상 가장 앞의 3개가 위치 정보이다.
        //x
        if(vx<m_minBound.x)	m_minBound.x = vx;
        if(vx>m_maxBound.x)	m_maxBound.x = vx;
        //y
        if(vy<m_minBound.y)	m_minBound.y = vy;
        if(vy>m_maxBound.y)	m_maxBound.y = vy;
        //z
        if(vz<m_minBound.z)	m_minBound.z = vz;
        if(vz>m_maxBound.z)	m_maxBound.z = vz;
    }
}

void	vbMesh::UpdateAABB(vec3	add_vt)
{
	//x
	if(add_vt.x<m_minBound.x)	m_minBound.x = add_vt.x;
	if(add_vt.x>m_maxBound.x)	m_maxBound.x = add_vt.x;
	//y
	if(add_vt.y<m_minBound.y)	m_minBound.y = add_vt.y;
	if(add_vt.y>m_maxBound.y)	m_maxBound.y = add_vt.y;
	//z
	if(add_vt.z<m_minBound.z)	m_minBound.z = add_vt.z;
	if(add_vt.z>m_maxBound.z)	m_maxBound.z = add_vt.z;		
}

void vbMesh::AllocVertexArrayBuffer(int vtCount)
{
    //Vtx count
    m_vtCount = vtCount;
    
    int vt_size = 6;        // Vertex Pos + Normal
    //Texture
    if(m_material_type==vbMaterial::VBMESH_TEXTURE || m_material_type==vbMaterial::VBMESH_BLENDTEXTURE)  vt_size +=2;        // UV(2)
    
    if(m_vt_array_buffer) delete [] m_vt_array_buffer;
    m_vt_array_buffer = new float[m_vtCount * vt_size];
    memset(m_vt_array_buffer, 0, sizeof(float)* m_vtCount * vt_size);
    
    //Texture - *** Pointing만 하는 용도이지 alloc은 하지 않는다. 즉, dealloc하면 안된다.
    if(m_material_type==vbMaterial::VBMESH_TEXTURE || m_material_type==vbMaterial::VBMESH_BLENDTEXTURE)  m_texcoords = &m_vt_array_buffer[m_vtCount * 6];
    else                    m_texcoords = NULL;
}

void vbMesh::AllocFaceBuffer(int fcCount)
{
     //face count
     m_fcCount = fcCount;
 
     //face buffer
    if(m_indices)   delete [] m_indices;
    if(m_indicesB)  delete [] m_indicesB;
    m_indices   = NULL;
    m_indicesB  = NULL;
    
    m_indices = new unsigned short[m_fcCount*3];   //Fc0,1,2
    m_bByteIndex = false;
 }

void vbMesh::RemoveTextureBuffer()
{
    float* new_vtbuffer = new float[m_vtCount * 6];   // pos + normal
    memcpy(new_vtbuffer, m_vt_array_buffer, sizeof(float)*m_vtCount * 6);
    delete m_vt_array_buffer;
    
    m_vt_array_buffer = new_vtbuffer;
    new_vtbuffer = NULL;
}

void vbMesh::ShortenIndexBufferIntoByteIndexBuffer()
{
    if(m_fcCount*3>=256)
    {
        m_szIdxCount = m_fcCount * 3;
        return;
    }
    
    if(m_indicesB)   delete [] m_indicesB;
    m_indicesB = NULL;
    m_indicesB = new unsigned char[m_fcCount*3];   //Fc0,1,2
    
    
    for(int i=0;i<m_fcCount;i++)
    {
        m_indicesB[i*3+0] = (unsigned char)m_indices[i*3+0];
        m_indicesB[i*3+1] = (unsigned char)m_indices[i*3+1];
        m_indicesB[i*3+2] = (unsigned char)m_indices[i*3+2];
    }
     
    if(m_indices)   delete [] m_indices;
    m_indices = NULL;
    
    m_bByteIndex = true;
    
    m_szIdxCount = m_fcCount * 3;
}

VBMapErrorCode    vbMesh::AddMeshInstance(vbMeshInstance* pInst)
{
    //Static mesh를 Instance화 하려고 할 때
    if(m_bIsStatic==true)   return VBMapError_INSTANCING_WITH_STATIC_MESH;
    
    //없는 Instance를 추가 하려고 할 때
    if(pInst==NULL)         return VBMapError_BAD_INPUT_PARAMETER;

    //Instance 추가
    if(m_bHasInstance==false)   m_bHasInstance = true;
    m_aMeshInstances.push_back(pInst);
    
    return VBMapError_NO_ERROR;
}

void    vbMesh::recalcNormalBuffer()
{
    assert(m_vt_array_buffer);
    

    vec3 vt0,vt1,vt2,nrm;
    float* pNrmBuff = GetNormalBuffer();
    int vtID0,vtID1,vtID2;
    
    //Vt assign
    if (m_indices)
    {
        for (unsigned short fcIdx=0; fcIdx<m_fcCount; fcIdx++)
        {

            vtID0 = m_indices[fcIdx*3+0];
            vtID1 = m_indices[fcIdx*3+1];
            vtID2 = m_indices[fcIdx*3+2];
            
            //get vt
            vt0.x = m_vt_array_buffer[vtID0 * 3 + 0];
            vt0.y = m_vt_array_buffer[vtID0 * 3 + 1];
            vt0.z = m_vt_array_buffer[vtID0 * 3 + 2];
            
            vt1.x = m_vt_array_buffer[vtID1 * 3 + 0];
            vt1.y = m_vt_array_buffer[vtID1 * 3 + 1];
            vt1.z = m_vt_array_buffer[vtID1 * 3 + 2];
            
            vt2.x = m_vt_array_buffer[vtID2 * 3 + 0];
            vt2.y = m_vt_array_buffer[vtID2 * 3 + 1];
            vt2.z = m_vt_array_buffer[vtID2 * 3 + 2];
            
            //face normal
            nrm = (vt1 - vt0).Cross(vt2 - vt0).Normalized();
            
            //set normal buffer
            pNrmBuff[vtID0 * 3 + 0] = nrm.x;
            pNrmBuff[vtID0 * 3 + 1] = nrm.y;
            pNrmBuff[vtID0 * 3 + 2] = nrm.z;
            
            pNrmBuff[vtID1 * 3 + 0] = nrm.x;
            pNrmBuff[vtID1 * 3 + 1] = nrm.y;
            pNrmBuff[vtID1 * 3 + 2] = nrm.z;
            
            pNrmBuff[vtID2 * 3 + 0] = nrm.x;
            pNrmBuff[vtID2 * 3 + 1] = nrm.y;
            pNrmBuff[vtID2 * 3 + 2] = nrm.z;
            
        }
    }
    else if(m_indicesB)
    {
        for (unsigned short fcIdx=0; fcIdx<m_fcCount; fcIdx++)
        {
            vtID0 = m_indicesB[fcIdx*3+0];
            vtID1 = m_indicesB[fcIdx*3+1];
            vtID2 = m_indicesB[fcIdx*3+2];
            
            //get vt
            vt0.x = m_vt_array_buffer[vtID0 * 3 + 0];
            vt0.y = m_vt_array_buffer[vtID0 * 3 + 1];
            vt0.z = m_vt_array_buffer[vtID0 * 3 + 2];
            
            vt1.x = m_vt_array_buffer[vtID1 * 3 + 0];
            vt1.y = m_vt_array_buffer[vtID1 * 3 + 1];
            vt1.z = m_vt_array_buffer[vtID1 * 3 + 2];
            
            vt2.x = m_vt_array_buffer[vtID2 * 3 + 0];
            vt2.y = m_vt_array_buffer[vtID2 * 3 + 1];
            vt2.z = m_vt_array_buffer[vtID2 * 3 + 2];
            
            //face normal
            nrm = (vt1 - vt0).Cross(vt2 - vt0).Normalized();
            
            //set normal buffer
            pNrmBuff[vtID0 * 3 + 0] = nrm.x;
            pNrmBuff[vtID0 * 3 + 1] = nrm.y;
            pNrmBuff[vtID0 * 3 + 2] = nrm.z;
            
            pNrmBuff[vtID1 * 3 + 0] = nrm.x;
            pNrmBuff[vtID1 * 3 + 1] = nrm.y;
            pNrmBuff[vtID1 * 3 + 2] = nrm.z;
            
            pNrmBuff[vtID2 * 3 + 0] = nrm.x;
            pNrmBuff[vtID2 * 3 + 1] = nrm.y;
            pNrmBuff[vtID2 * 3 + 2] = nrm.z;
        }
    }
    else
        assert(0);
    
}