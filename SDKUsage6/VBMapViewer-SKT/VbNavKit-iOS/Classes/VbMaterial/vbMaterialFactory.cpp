

#include "vbMaterialFactory.hpp"


// ldj
#include <iostream>
#include <stdio.h>
#include <string.h>


using namespace std;


vbMaterialFactory::vbMaterialFactory()
{
    
}

vbMaterialFactory::~vbMaterialFactory()
{
    
}
//SubTextureMap과 SubMaterial을 사용하지 않는다.

bool vbMaterialFactory::ImportMaterialFromPMESH(FILE* fp, int material_count, bool bIsSub)
{
    //PMESH isn't well specified. So this format will not be used.
    return false;
    
    if(!fp) return false;
    
    //Read materials
    int material_id;
    float ambient[3],diffuse[3],specular[3];
    float alpha;
    //Subtexture - NOT USE
    int subtexCount;
    int name_length;
    char tmp_buffer[256];
    float   uv_offset[2],uv_tiling[2],uv_angle;
    //SubMaterial
    int num_subMaterial;
    //Iterator 
    int i,j;
 
    //Material loop
    for (i=0; i<material_count; i++)
    {
        //ID
        fread(&material_id, sizeof(int), 1, fp);
        
        //Ambient
        fread(ambient, sizeof(float), 3, fp);
        
        //Diffuse
        fread(diffuse, sizeof(float), 3, fp);
        
        //Specular
        fread(specular, sizeof(float), 3, fp);
        
        //Alpha
        fread(&alpha, sizeof(float), 1, fp);
        
        //SubTexture - SMESH에는 포함되어 있지만 김영부장님과 사용하지 않기로 하였으므로 읽기만 하고 저장하지 않음.
        fread(&subtexCount, sizeof(int), 1, fp);
        for(j=0;i<subtexCount;j++)
        {
            //Name length
            fread(&name_length, sizeof(int), 1, fp);
            
            //Name
            fread(tmp_buffer, sizeof(char), name_length, fp);
            
            //Map name length
            fread(&name_length, sizeof(int), 1, fp);
            
            //Map name
            fread(tmp_buffer, sizeof(char), name_length, fp);
            
            //Map UV param
            fread(uv_offset, sizeof(float), 2, fp);
            fread(uv_tiling, sizeof(float), 2, fp);
            fread(&uv_angle, sizeof(float), 1, fp);
        }
        
        //Sub-Material
        fread(&num_subMaterial, sizeof(int), 1, fp);
        if(num_subMaterial>0)   ImportMaterialFromPMESH(fp, num_subMaterial, true);//bIsSub=true : 읽기만 하고 저장하지는 않는다.
        
        //텍스쳐 경로는?????
        
        
        ////////////////////////////////// ADD MATERIAL ////////////////////////////
        
        
        
        ////////////////////////////////// RELEASE ALLOCATED BUFFER /////////////////
        
    }
    
    
    
    return true;
}

/*
bool vbMaterialFactory::ImportMaterialFromSBMv1(vbMaterialManager* pMtlManager, FILE* fp, int material_count)
{
     
    //ID
    unsigned	short		nMtlID;			//Material ID
        
    //Color
    float					fAmbient[4];	//Material color - ambient
    float					fDiffuse[4];	//Material color - diffuse
    float					fSpecular[4];	//Material color - specular
        
    //Facing
    unsigned char			nFacing;
        
    //Texture
    unsigned	short		nTexNameLength;	//Texture name length - NULL문자 없는 순수 문자들의 길이
    char					pTexName[256];	
    
    
    //Reserved texture name
    std::string default_name("default.png");
    std::string new_default_name("default_.png");
        
    vbMaterial_V2*	pMaterial_V2 = NULL;
    
    pMtlManager->GetMaterials()->reserve(material_count);
    
    
       
    for(int i=0;i<material_count;i++)
    {
        //Read
        
        //ID
        fread(&nMtlID,sizeof(unsigned short),1,fp);
        //Color
        fread(fAmbient,sizeof(float),4,fp);
        fread(fDiffuse,sizeof(float),4,fp);
        fread(fSpecular,sizeof(float),4,fp);
        //Facing
        fread(&nFacing,sizeof(unsigned char),1,fp);
        //TextureName
        fread(&nTexNameLength,sizeof(unsigned short),1,fp);
        
        memset( pTexName, 0, 256);
        fread(pTexName,sizeof(char),nTexNameLength,fp);
     
        pMaterial_V2 = (vbMaterial_V2*)pMtlManager->CreateMaterial(vbMaterial::VBMATL_VER_V2);
           
        //Assign
        pMaterial_V2->SetID(nMtlID);
        pMaterial_V2->SetAmbient(fAmbient[0],fAmbient[1],fAmbient[2],fAmbient[3]);
        pMaterial_V2->SetDiffuse(fDiffuse[0],fDiffuse[1],fDiffuse[2],fDiffuse[3]);
        pMaterial_V2->SetSpecular(fSpecular[0],fSpecular[1],fSpecular[2],fSpecular[3]);
        pMaterial_V2->SetFacing(nFacing);
        if(nTexNameLength==0)
        {
            pMaterial_V2->SetTextureMaterial(false);
            
            if(fDiffuse[3]>0.99f)   pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_COLOR);
            else                    pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_BLEND);
        }
        else 
        {
           std::string material_name(pTexName,nTexNameLength);
            
            //PNG면 반투명 텍스쳐
            std::string extension = material_name.substr(material_name.length()-3,material_name.length());
            
            if(extension=="png" || extension=="PNG")
            {
                pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_BLENDTEXTURE);
                //std::cout << "PNG Texture" << std::endl;
            }
            else
                pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_TEXTURE);
 
            if (default_name==material_name)
            {
                pMaterial_V2->SetName(new_default_name.c_str(),nTexNameLength);
            }
            else
                pMaterial_V2->SetName(pTexName,nTexNameLength);
                
            pMaterial_V2->SetTextureMaterial(true);
        }
            
        pMaterial_V2 = NULL;
            
    }
        
    return true;
}
 */

//표준 테이블을 써야 하는데 준비가 안되었으므로 일단 그냥 텍스쳐 이름만 추출하여 쓴다.

bool vbMaterialFactory::ImportMaterialFromSBMv2(vbMaterialManager* pMtlManager, FILE* fp, int cur_material_count,  char* sbm_path)
{
    
    //ID
    unsigned    short        nMtlID;            //Material ID
    
    //Color
    float                    fAmbient[4];    //Material color - ambient
    float                    fDiffuse[4];    //Material color - diffuse
    float                    fSpecular[4];    //Material color - specular
    
    //Facing
    unsigned char            nFacing;
    
    //Texture
    unsigned    short        nTexNameLength;    //Texture name length - NULL문자 없는 순수 문자들의 길이
    char                    pTexName[256];
    
    unsigned short previouse_mtl_count = pMtlManager->GetMaterialCount();
    unsigned short new_material_count = previouse_mtl_count + cur_material_count;
    pMtlManager->GetMaterials()->reserve(new_material_count);
    
    //SBM File name
    char file_name[1024];
    memset(file_name, 0, sizeof(char)*1024);
    
 
    
   
    char *pFileName = strrchr(sbm_path,'/');
    sprintf(file_name, "%s",(++pFileName));
    
    vbMaterial_V2*    pMaterial_V2 = NULL;
    for(int i=0;i<cur_material_count;i++)
    {
       
        //ID
        fread(&nMtlID,sizeof(unsigned short),1,fp);
        //Color
        fread(fAmbient,sizeof(float),4,fp);
        fread(fDiffuse,sizeof(float),4,fp);
        fread(fSpecular,sizeof(float),4,fp);
        //Facing
        fread(&nFacing,sizeof(unsigned char),1,fp);
        //TextureName
        fread(&nTexNameLength,sizeof(unsigned short),1,fp);
        
        memset( pTexName, 0, 256);
        fread(pTexName,sizeof(char),nTexNameLength,fp);
        // ldj
        //        std::cout << "texture file is " << pTexName << ". \r\n";
        
        pMaterial_V2 = (vbMaterial_V2*)pMtlManager->CreateMaterial(vbMaterial::VBMATL_VER_V2);
        //Read
        
        //Assign
        char matl_name[1024];
        memset(matl_name, 0, sizeof(char)*1024);
        sprintf(matl_name,"%s_%d",file_name,(int)nMtlID);
        
        pMaterial_V2->SetID(string(matl_name));
        
        pMaterial_V2->SetAmbient(fAmbient[0],fAmbient[1],fAmbient[2],fAmbient[3]);
        pMaterial_V2->SetDiffuse(fDiffuse[0],fDiffuse[1],fDiffuse[2],fDiffuse[3]);
        pMaterial_V2->SetSpecular(fSpecular[0],fSpecular[1],fSpecular[2],fSpecular[3]);
        pMaterial_V2->SetFacing(nFacing);
        if(nTexNameLength==0)
        {
            pMaterial_V2->SetTextureMaterial(false);
            
            if(fDiffuse[3]>0.99f)   pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_COLOR);
            else                    pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_BLEND);
        }
        else
        {
            std::string material_name(pTexName,nTexNameLength);
            std::string raw_material_name = material_name.substr(material_name.rfind("\\")+1);
            
            //PNG면 반투명 텍스쳐
            std::string extension = material_name.substr(material_name.length()-3,material_name.length());
            
            if(extension=="png" || extension=="PNG")
            {
                pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_BLENDTEXTURE);
                //std::cout << "PNG Texture" << std::endl;
            }
            else
                pMaterial_V2->SetMaterialType(vbMaterial::VBMESH_TEXTURE);

            
            pMaterial_V2->SetTextureName(raw_material_name.c_str(),raw_material_name.length());
            
            pMaterial_V2->SetTextureMaterial(true);
        }
        
        pMaterial_V2 = NULL;
        
        
    }
    
    //Debug
    /*
    vbMaterial_V2* pMtl = 0;
    std::cout << "[IMPORTED MATERIALS]" << std::endl;
    for(int i=0;i<pMtlManager->GetMaterialCount();i++)
    {
        pMtl = (vbMaterial_V2*)pMtlManager->GetMaterials()->at(i);
     
        //ID
        std::cout << pMtl->GetID() << std::endl;
    }
    */
    
    return true;
}

