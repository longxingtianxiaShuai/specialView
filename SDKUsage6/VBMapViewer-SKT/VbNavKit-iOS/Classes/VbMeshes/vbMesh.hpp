#pragma once

#include "Vector.hpp"
#include <float.h>
#include <vector>
#include <string>

#include "vbMaterial.hpp"
#include "vbMeshInstance.h"
#include "vbEnum.hpp"

using std::vector;
using std::string;

enum VertexFlags {
    VertexFlagsNormals = 1 << 0,
    VertexFlagsTexCoords = 1 << 1,
};

struct  vbSBMHeader_V2
{
	unsigned short	material_count;	//Material count
	unsigned short	mesh_count;		//Mesh count
};

struct  vbSBMHeader_V4_2
{
    unsigned    char    minor_version;  // Minor version
    char                unit_of_length; //Unit of length (0=mm, 1=cm, 2=m, 3=km, 4=inch)
    
	unsigned short      material_count;	//Material count
	unsigned short      mesh_count;		//Mesh count
    unsigned short      instance_count; //Instance count
    float               AABB[6];    //Min(x,y,z)+Max(x,y,z)
};



class vbMesh
{
    
public:
    vbMesh();
    //vbMesh(int fcCnt);
    ~vbMesh();
    
private:
    //Type 
    vbMaterial::VBMESH_TYPE   m_material_type;    //0(Texture), 1(Blend), 2(Color), 3(Blend Texture)
    
	//Name
    std::string		m_MeshName;//SBM+ID
    std::string		m_GroupName;//SBM
    unsigned short  m_id;
	
	//Mesh info
    unsigned short	m_vtCount;
	unsigned short	m_fcCount;
    
    //Visibility
    bool            m_bShow;
   
	float*              m_texcoords;	//실제 texcoords 정보 - 메모리 할당은 되지 않고 가리키기만 한다. 해제 주의
    float*              m_vt_array_buffer;  //VBO를 위한 vertex 통합 버퍼
    
	unsigned short*     m_indices;		//실제 index 정보
	unsigned char*      m_indicesB;		//실제 index 정보
    
    
	unsigned char		m_vertex_flag;	//vertex flag
	unsigned char		m_singlevt_float_cnt;	//

	unsigned int	m_VBOID[2];         //[0]-VertexBuffer, [1]-IndexBuffer-GL 버젼이 다를 수 있으므로 GL type이 아니라 표준 타입을 쓴다.
    std::string     m_MaterialID;       //Textuer ID
    unsigned short  m_szIdxCount;       //Index buffer의 사이즈(타입에 Short냐, byte냐에 따라 다름.)
	
	
	//Bounding box
	vec3	m_minBound;
	vec3	m_maxBound;
	
	//Solid color
	vec4	m_solid_color;	
    
    bool    m_bByteIndex;
    
    //Mesh Instance
    bool    m_bIsStatic;
    bool    m_bHasInstance;
    std::vector<vbMeshInstance*>    m_aMeshInstances;

public:
    bool    IsByteIndexBuffer() const       {   return m_bByteIndex; }
    
    //TYPE
    vbMaterial::VBMESH_TYPE   GetMaterialType()   {   return m_material_type;         }
    void            SetMaterialType(vbMaterial::VBMESH_TYPE material_type)
                                        {   m_material_type = material_type;    }

    
    //ID
    void            SetID(unsigned short id)    {   m_id = id;  }
    unsigned short  GetID()                     {   return m_id;  }
    
    //Visibility
    void        SetVisibility(bool bShow)       {   m_bShow = bShow;        }
    bool        IsVisible()                     {   return m_bShow;         }
                                                 

	//Count
    unsigned short		GetVertexCount()        {  return  m_vtCount;       }
    unsigned short		GetFaceCount()          {  return  m_fcCount;       }
    
    void    SetVertexCount(unsigned short vtCount)         {   m_vtCount       = vtCount;      }
    void    SetFaceCount(unsigned short fcCount)           {   m_fcCount       = fcCount;      }

    //Buffer
	float*			GetVertexBuffer();
	float*			GetNormalBuffer();
	float*			GetTexCoordBuffer();
    float*          GetVertexArrayBuffer();
    
	unsigned short* GetIndexBuffer();
    unsigned char*  GetByteIndexBuffer();
    
    void            FreeTexCoordForNoneTexMesh();
	
    size_t          GetVertexArrayBufferSizeInByte();
 	size_t			GetVertexBufferSizeInByte();
	size_t			GetNormalBufferSizeInByte();
	size_t			GetTexCoordBufferSizeInByte();
	size_t			GetIndexBufferSizeInByte();
    size_t          GetByteIndexBufferSizeInByte();
    
    const unsigned short    GetIndexCount() const { return m_szIdxCount; }; //Index가 UByte냐 UShort냐에 따라 랜더링 할 때마다 계산하지 않도록 미리 계산함.
    
    void            AllocVertexArrayBuffer(int vtCount);
    void            AllocFaceBuffer(int fcCount);
    void            RemoveTextureBuffer();
    void            ClearBuffer();
    void            recalcNormalBuffer();
 
    void            ShortenIndexBufferIntoByteIndexBuffer();
    
	/////// VBO - DO NOT USE
	unsigned int	GetVBOVertexBufferID()		{	return	m_VBOID[0];	}
	unsigned int	GetVBOIndexBufferID()		{	return	m_VBOID[1];	}
	void			SetVBOVertexBufferID(unsigned int	vbID)	{	m_VBOID[0]	= vbID;	}
	void			SetVBOIndexBufferID(unsigned int ibID)		{	m_VBOID[1]	= ibID;	}
	
    
	//Name
	std::string		GetMeshName()							{	return m_MeshName;		}
	void			SetMeshName(std::string mesh_name)		{	m_MeshName = mesh_name;	}

    //Group
    std::string		GetGroupName()                          {   return m_GroupName;     }//SBM
    void            SetGroupName(std::string grp_name)      {   m_GroupName = grp_name; }//SBM
    
    
    
	//Material
    std::string     GetMaterialID()							{	return	m_MaterialID;       }
	void			SetMaterialID(std::string matlID)       {	m_MaterialID	= matlID;	}


	//vbMesh
	//Bounding box
	void	InitAABB();
	void	UpdateAABB();
	void	UpdateAABB(vec3	add_vt);
	vec3	GetMinBound()	{	return m_minBound;	}
	vec3	GetMaxBound()	{	return m_maxBound;	}
    vec3    GetCenter();
	
	//Solid color
	vec4	GetSolidColor()					{	return m_solid_color;	}
	void	SetSolidColor(vec4 sColor)		{	m_solid_color = sColor;	}
	
    //Mesh Instance
    bool    IsStaticMesh() const                                {   return m_bIsStatic;         }
    void    SetStaticMesh(bool bIsStatic)                       {   m_bIsStatic = bIsStatic;    }
    bool    HasInstance() const                                 {   return m_bHasInstance;      }
    const std::vector<vbMeshInstance*>*    GetInstances() const {   return &m_aMeshInstances;   }
    
    VBMapErrorCode    AddMeshInstance(vbMeshInstance* pInst);
    

	
};

