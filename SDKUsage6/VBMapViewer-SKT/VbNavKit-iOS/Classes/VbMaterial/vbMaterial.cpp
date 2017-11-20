
#include "vbMaterial.hpp"
#include <iostream>
#include "Vector.hpp"

//////////////////////////      vbMaterial       //////////////////////////////////////
vbMaterial::vbMaterial()
{
    //Version
    m_Ver = VBMATL_VER_V2;
    
    //ID
    m_id = UNDEFINED_MATERIAL_ID; //Undefined
    
    //TexID
    m_GLtexID = 0;
    m_bTex = false;
    
    //Facting
    m_facing = 0;
    
    //Material type
    m_materila_type = vbMESH_UNDEFINED;
    
    //Name
    m_strTexName = NULL;
    m_nTexNameLength = 0;
    
    m_bCompleted = false;
    
    m_bReferencingGLTex = false;
    
}

vbMaterial::~vbMaterial()
{
    if(m_strTexName)  delete [] m_strTexName;
    m_strTexName = NULL;
}

void    vbMaterial::ShowInfo()
{
#ifdef VB_CORE_DEBUG
    //ID - MATL_VER - bTex - GLID - Facing - TexName - TexNameLen - MatlType - AmbRGBA - DiffRGBA - SpcRGBA
    std::cout << m_id << ",";
    std::cout << (unsigned int)m_Ver << ",";

    if(m_bTex)      std::cout << "Tex,";
    else            std::cout << "Color,";
    
    if(m_bCompleted)    std::cout << "TRUE,";
    else                std::cout << "FALSE,";

    std::cout << (int)m_GLtexID << ",";
    std::cout << (int)m_facing << ",";
    
    if(m_strTexName)    std::cout << m_strTexName << ",";
    else                std::cout << "NULL,";
    
    std::cout << (int)m_nTexNameLength << ",";
    std::cout << (unsigned int)m_materila_type << ",";

    std::cout << m_ColorRGBA[0] << ", " << m_ColorRGBA[1] << ", " << m_ColorRGBA[2] << ", " << m_ColorRGBA[3] << ",";
    std::cout << m_ColorRGBA[4] << ", " << m_ColorRGBA[5] << ", " << m_ColorRGBA[6] << ", " << m_ColorRGBA[7] << ",";
    std::cout << m_ColorRGBA[8] << ", " << m_ColorRGBA[9] << ", " << m_ColorRGBA[10] << ", " << m_ColorRGBA[11] << std::endl;
#endif
}


void vbMaterial::SetAmbient(float ambR, float ambG, float ambB, float ambA)
{
    m_ColorRGBA[0] = ambR;
    m_ColorRGBA[1] = ambG;
    m_ColorRGBA[2] = ambB;
    m_ColorRGBA[3] = ambA;
}

void vbMaterial::SetDiffuse(float diffR, float diffG, float diffB, float diffA)
{
    m_ColorRGBA[4+0] = diffR;
    m_ColorRGBA[4+1] = diffG;
    m_ColorRGBA[4+2] = diffB;
    m_ColorRGBA[4+3] = diffA;
}

void vbMaterial::SetSpecular(float spcR, float spcG, float spcB, float spcA)
{
    m_ColorRGBA[8+0] = spcR;
    m_ColorRGBA[8+1] = spcG;
    m_ColorRGBA[8+2] = spcB;
    m_ColorRGBA[8+3] = spcA;
}

float*  vbMaterial::GetAmbient()
{
    return (float*)(&m_ColorRGBA[0]);
}

float*  vbMaterial::GetDiffuse()
{
    return (float*)(&m_ColorRGBA[4]);
}

float*  vbMaterial::GetSpecular()
{
    return (float*)(&m_ColorRGBA[8]);
}


void vbMaterial::SetTextureName(const char* name, unsigned short name_len_without_NULL)
{
    if(m_strTexName)  delete [] m_strTexName;
    m_strTexName = NULL;
    
    m_nTexNameLength = name_len_without_NULL;
    m_strTexName = new char[name_len_without_NULL+1]; //plus 1 for null
    memcpy(m_strTexName, name, name_len_without_NULL);
    m_strTexName[name_len_without_NULL]='\0';
}
//////////////////////////      vbMaterial       //////////////////////////////////////

//////////////////////////      vbMaterial_V2       //////////////////////////////////////
vbMaterial_V2::vbMaterial_V2() : vbMaterial()
{
    //Color
    memset(m_ColorRGBA, 0, sizeof(float)*12);
    //Alpha=1
    m_ColorRGBA[3]=m_ColorRGBA[7]=m_ColorRGBA[11]=1.f;
}

vbMaterial_V2::~vbMaterial_V2()
{
    vbMaterial::~vbMaterial();

}


//////////////////////////      vbMaterial_V2       //////////////////////////////////////

//////////////////////////      vbMaterial_V4       //////////////////////////////////////
vbMaterial_V4::vbMaterial_V4() : vbMaterial()
{
    m_eColorType = SBMCOLOR_TYPE_FLOAT; //특별한 이유가 없다면 float형을 사용한다.
}

vbMaterial_V4::~vbMaterial_V4()
{
    vbMaterial::~vbMaterial();
}
