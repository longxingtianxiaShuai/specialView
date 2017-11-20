#pragma once

#include <string>

#define UNDEFINED_MATERIAL_ID   -1
#define UNDEFINED_MATERIAL_NAME "UNDEF"
#define BASE_MATERIAL_NAME_SIZE 64


class vbMaterial
{
public:
    vbMaterial();
    ~vbMaterial();
    
    enum    VBMESH_TYPE
    {
        VBMESH_TEXTURE      =   0,
        VBMESH_COLOR        =   1,
        VBMESH_BLEND        =   2,
        VBMESH_BLENDTEXTURE = 3,
        vbMESH_UNDEFINED    = 100
    };
    
    enum    VBMATL_VER
    {
        VBMATL_VER_V2   =   2,
        VBMATL_VER_V4   =   4
    };
    
protected:
    VBMATL_VER  m_Ver;
    
    std::string     m_id;//SBM Name + number
    bool            m_bTex;
    bool            m_bCompleted;
    
    unsigned short  m_GLtexID;
    unsigned char   m_facing;
    
    char*           m_strTexName;
    unsigned char   m_nTexNameLength;
    
    VBMESH_TYPE     m_materila_type;
    
    float           m_ColorRGBA[12];
    bool            m_bReferencingGLTex;

    
public:
    bool    IsGLReferencing()               {   return m_bReferencingGLTex;    }
    void    GLReferencing(bool bRef)        {   m_bReferencingGLTex = bRef;    }

    
    //
    bool    IsCompleted()   {   return m_bCompleted;    }
    void    Completed()     {   m_bCompleted = true;    }
    
    //COLOR
    void    SetAmbient(float ambR, float ambG, float ambB, float ambA);
    void    SetDiffuse(float diffR, float diffG, float diffB, float diffA);
    void    SetSpecular(float spcR, float spcG, float spcB, float spcA);

    float*  GetAmbient();
    float*  GetDiffuse();
    float*  GetSpecular();
    
    void    ShowInfo();
    
    
    //VERSION
    VBMATL_VER      GetMaterialVersion()                        {  return m_Ver;        };
    void            SetMaterialVersion(VBMATL_VER   version)    {  m_Ver = version;     };
    
    //ID
    std::string     GetID()                         {   return m_id;    };
    void            SetID(std::string strId)        {   m_id = strId;     };
    
    //Texture or Diffuse
    bool            IsTexture()                                 {   return m_bTex;                  };
    void            SetTextureMaterial(bool bIsTexture)         {   m_bTex = bIsTexture;            };
    VBMESH_TYPE     GetMaterialType()                           {   return m_materila_type;         };
    void            SetMaterialType(VBMESH_TYPE ucMtlType)      {   m_materila_type = ucMtlType;    };
    
    //TexID
    unsigned short  GetGLTexID()                        {   return  m_GLtexID;      };
    void            SetGLTexID(unsigned short GLtxID)   {   m_GLtexID = GLtxID;     };
    
    //Facing
    unsigned char   GetFacing()                         {    return m_facing;       };
    void            SetFacing(unsigned char facing)     {   m_facing = facing;      };
    
    //Name
    char*           GetTextureName()                   {   return m_strTexName;      };
    unsigned char   GetTextureNameLength()             {   return m_nTexNameLength;  };
    void            SetTextureName(const char* tex_name, unsigned short tex_name_len_without_NULL);
   
    
    
};


class vbMaterial_V2 : public vbMaterial
{
 
public:
    vbMaterial_V2();
    ~vbMaterial_V2();
    
    
private:

    
public:
    
    
};

class vbMaterial_V4 : public vbMaterial
{
    
public:
    vbMaterial_V4();
    ~vbMaterial_V4();
    
    enum    SBMCOLOR_TYPE
    {
        SBMCOLOR_TYPE_UBYTE = 0,
        SBMCOLOR_TYPE_FLOAT = 1
    };
    
    
private:
    SBMCOLOR_TYPE   m_eColorType;
    //원래 색상 타입이 무엇이었는지 저장한다. - 정수형으로 설정 된 색상값을 나중에 색상값을 이용하여 식별 할 필요가 있다면 그때 정수형 값을 저장 할지 여부를 다시 확인 해 보도록 한다.

    
    
public:
    SBMCOLOR_TYPE   GetColorType()                          {   return m_eColorType;    };
    void            SetColorType(SBMCOLOR_TYPE  clrType)    {   m_eColorType = clrType; };
    /////////////////////////////////////////////////////////////
    
};