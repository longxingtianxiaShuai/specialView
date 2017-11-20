#include "vbMeshFactory.hpp"
#include <iostream>
#import <mach/mach.h>    // 시간 체크
#import <mach/mach_time.h>

vbMeshFactory::vbMeshFactory()
{

}
vbMeshFactory::~vbMeshFactory()
{
	
}


void vbMeshFactory::CreateTriangle(vbMesh* pNewMesh, unsigned char flags)
{
   //Vertex flag와 vertex별 float count가 정의된다.
   /* 
    pNewMesh->SetVertexFlag(flags);
    pNewMesh->SetVertexCount(3);//3각형 1개   
    pNewMesh->SetFaceCount(1);  
    
    //Create Vertex Buffer
	vector<float>* pVts = pNewMesh->GetVertexBufferVectorPointer();
    pVts->clear();
    pVts->resize(pNewMesh->GetVertexCount()*pNewMesh->GetSingleVtFloatCount()); 
    
	(*pVts)[0] = -1.f;		(*pVts)[1] = -1.f;		(*pVts)[2] = 0.f;	//P0
	(*pVts)[3] =  0.f;		(*pVts)[4] =  0.f;		(*pVts)[5] = 1.f;	//N0
	(*pVts)[6] =  0.f;		(*pVts)[7] =  0.f;							//T0
	
	(*pVts)[8] =  1.f;		(*pVts)[9] = -1.f;		(*pVts)[10] = 0.f;	//P1
	(*pVts)[11] =  0.f;		(*pVts)[12] = 0.f;		(*pVts)[13] = 1.f;	//N1
	(*pVts)[14] = 1.f;		(*pVts)[15] = 0.f;
	
	(*pVts)[16] =  0.f;		(*pVts)[17] = 1.5f;		(*pVts)[18] = 0.f;	//P2
	(*pVts)[19] =  0.f;		(*pVts)[20] = 0.f;		(*pVts)[21] = 1.f;	//N2
	(*pVts)[22] = 0.5f;		(*pVts)[23] = 1.f;    
    
    
    //Create Index Buffer
	vector<unsigned short>*     pIdx = pNewMesh->GetIndexBufferVectorPointer();
    pIdx->clear();
	pIdx->resize(3);
	(*pIdx)[0] = 0;
	(*pIdx)[1] = 1;
	(*pIdx)[2] = 2;	
    */
   	
}

void vbMeshFactory::CreateRectangle(vbMesh* pNewMesh, unsigned char flags, float edge_length)
{
    //Vertex flag와 vertex별 float count가 정의된다.
    /* 
     pNewMesh->SetVertexFlag(flags);
    pNewMesh->SetVertexCount(4);//3각형 2개   
    pNewMesh->SetFaceCount(2);  

    //half edge
    float hEdge = edge_length/2.f;
   
    //Create Vertex Buffer
	vector<float>* pVts = pNewMesh->GetVertexBufferVectorPointer();
    pVts->clear();
    pVts->resize(pNewMesh->GetVertexCount()*pNewMesh->GetSingleVtFloatCount()); 
    
	(*pVts)[0] = -hEdge;	(*pVts)[1] = -hEdge;	(*pVts)[2] = 0.f;	//P0
	(*pVts)[3] =  0.f;		(*pVts)[4] =  0.f;		(*pVts)[5] = 1.f;	//N0
	(*pVts)[6] =  0.f;		(*pVts)[7] =  0.f;							//T0
	
	(*pVts)[8] =  hEdge;	(*pVts)[9] = -hEdge;	(*pVts)[10] = 0.f;	//P1
	(*pVts)[11] =  0.f;		(*pVts)[12] = 0.f;		(*pVts)[13] = 1.f;	//N1
	(*pVts)[14] = 1.f;		(*pVts)[15] = 0.f;                          //T1
	
	(*pVts)[16] = hEdge;	(*pVts)[17] = hEdge;	(*pVts)[18] = 0.f;	//P2
	(*pVts)[19] =  0.f;		(*pVts)[20] = 0.f;		(*pVts)[21] = 1.f;	//N2
	(*pVts)[22] = 1.f;		(*pVts)[23] = 1.f;                          //T2
    
	(*pVts)[24] = -hEdge;	(*pVts)[25] = hEdge;	(*pVts)[26] = 0.f;	//P3
	(*pVts)[27] =  0.f;		(*pVts)[28] = 0.f;		(*pVts)[29] = 1.f;	//N3
	(*pVts)[30] = 0.f;		(*pVts)[31] = 1.f;                          //T3
    
    
    //Create Index Buffer
	vector<unsigned short>*     pIdx = pNewMesh->GetIndexBufferVectorPointer();
    pIdx->clear();
	pIdx->resize(6);
    //0
	(*pIdx)[0] = 0;
	(*pIdx)[1] = 1;
	(*pIdx)[2] = 2;	    
    //1
	(*pIdx)[3] = 0;
	(*pIdx)[4] = 2;
	(*pIdx)[5] = 3;	 
    */
    pNewMesh->UpdateAABB();
}

bool vbMeshFactory::ImportMeshFromSBMv1(vbMeshManager* pMshManager, vbMaterialManager* pMtlMng, FILE* fp, int mesh_count,bool bAsStatic,  char* sbm_path, vbMeshGroupManager* pGroupMng)
{

    if(!fp) return false;
    
    //SBM File name
    char file_name[1024];
    memset(file_name, 0, sizeof(char)*1024);
    char* pFileName = strrchr(sbm_path,'/');
    sprintf(file_name, "%s",(++pFileName));

    //ID
    unsigned	short		nMeshID;
    
    //Material 
    unsigned	short		nMeshMtlID;
    
    //BB
    vec3 sbm_minBB = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    vec3 sbm_maxBB = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    
    ////Geometry
    //모든 Mesh에 각 성분이 다 있는 것이 아니기 때문에 개수를 따로 관리한다. 
    //하지만, 현재는 있으면 vt count와 같아야만 한다. 없으면 안쓰고.
    //Vertex & Face
    unsigned	short		nVtCount;			//Vertex count
    unsigned	short		nNrmCount;			//Normal count
    unsigned	short		nTxCount;			//Texture count
    unsigned	short		nFaceCount;			//Face count  
    vbMaterial::VBMESH_TYPE material_type = vbMaterial::VBMESH_COLOR;
    vbMesh*		pNewMesh = NULL;
    int i=0;
    char strSBM_id[1024];

	///// Start
    
    uint64_t start;
    uint64_t elapsed;
    mach_timebase_info_data_t sTimebaseInfo;
    
    start = mach_absolute_time();
    
    /// 시간 체크
    for(i=0;i<mesh_count;i++)
    {
        //Create New mesh
        pNewMesh = NULL;//pMshManager->CreateMesh();
        
        //ID
        fread(&nMeshID,sizeof(unsigned short),1,fp);			//ID
        fread(&nMeshMtlID,sizeof(unsigned short),1,fp);			//Material ID
        
        memset(strSBM_id, 0, sizeof(char)*1024);
        sprintf(strSBM_id, "%s_%d",file_name,(int)nMeshMtlID);
        std::string matl_id_string = strSBM_id;
        
        material_type = pMtlMng->GetMaterialType(matl_id_string);
        
        
        switch (material_type)
        {
            case vbMaterial::VBMESH_TEXTURE:    //Texture
                pNewMesh = pMshManager->CreateMesh(vbMaterial::VBMESH_TEXTURE,bAsStatic);
                break;
            case vbMaterial::VBMESH_COLOR:      //Color
                pNewMesh = pMshManager->CreateMesh(vbMaterial::VBMESH_COLOR,bAsStatic);
                break;
            case vbMaterial::VBMESH_BLEND:      //Blend
                pNewMesh = pMshManager->CreateMesh(vbMaterial::VBMESH_BLEND,bAsStatic);
                break;
            case vbMaterial::VBMESH_BLENDTEXTURE://Blend Texture
                pNewMesh = pMshManager->CreateMesh(vbMaterial::VBMESH_BLENDTEXTURE,bAsStatic);
                break;
                
            case vbMaterial::vbMESH_UNDEFINED:  //Blend Texture
               // std::cout << "Undefined Material type" << std::endl;
            default:
               // std::cout << "Unsupported Material format - MeshID = " << nMeshID << ", MtlID = " << nMeshMtlID << std::endl;
                break;
        }
        
      
        memset(strSBM_id, 0, sizeof(char)*1024);
        sprintf(strSBM_id, "%s_%d",file_name,(int)nMeshID);
        std::string mesh_id_string = strSBM_id;
        
        pNewMesh->SetID(nMeshID);
        pNewMesh->SetMeshName(mesh_id_string);//SBM+ID
        pNewMesh->SetGroupName(file_name);
        pNewMesh->SetMaterialID(matl_id_string);
        pNewMesh->SetMaterialType(material_type);
        pNewMesh->SetStaticMesh(bAsStatic);
                
        //Vertex
        fread(&nVtCount,sizeof(unsigned short),1,fp);			//Pos
        pNewMesh->AllocVertexArrayBuffer(nVtCount);
        fread(pNewMesh->GetVertexBuffer(),sizeof(float),nVtCount*3,fp);
        
        fread(&nNrmCount,sizeof(unsigned short),1,fp);			//Normal
        
        //개수가 같으면 읽는다.
        if(nVtCount==nNrmCount)
            fread(pNewMesh->GetNormalBuffer(),sizeof(float),nNrmCount*3,fp);
        else if(nNrmCount!=0)//개수가 다르면 다시 계산 하도록 한다. 0이면 다시 계산하는 것이 의도 된 것이라 볼 수 있는데, 0 아닌 다른 개수는 오류일 가능성이 높다.
        {
            //std::cout << "[WARNNING] Mesh ID(" << nMeshID << ") has different VtCount(" << nVtCount << ") with NrmCount(" << nNrmCount << ") and it's not zero.(Zero means recalc. normals.)\r\n";
            
            fseek(fp,sizeof(float)*3*nNrmCount, SEEK_CUR);
            
            //어쨋든 개수가 다르면 다시 계산하도록 한다.
        }
        
        
        fread(&nTxCount,sizeof(unsigned short),1,fp);			//Tex
        //텍스쳐 좌표 개수와 vertex 개수가 다르면 잘못 만들어진 파일일 가능성이 있으므로 일단 알려줌.
        if(nVtCount!=nTxCount && (material_type==vbMaterial::VBMESH_TEXTURE || material_type==vbMaterial::VBMESH_BLENDTEXTURE))
        {   //std::cout << "[WARNNING] Mesh ID(" << nMeshID << ") has different VtCount(" << nVtCount << ") with nTxCount(" << nTxCount << ").\r\n";
        }

        //텍스쳐 재질이고 좌표가 있을 때는 읽음.
        if((material_type==vbMaterial::VBMESH_TEXTURE || material_type==vbMaterial::VBMESH_BLENDTEXTURE) && nTxCount>0)
            fread(pNewMesh->GetTexCoordBuffer(),sizeof(float),nTxCount*2,fp);
        //그 외의 경우는 있는 텍스쳐 좌표 무시함.
        else if(nTxCount>0)
        {
           // std::cout << "[WARNNING] Mesh ID(" << nMeshID << ") isn't textured mesh. But it has texture coordinates data. It would be ignored.\r\n";
            fseek(fp,sizeof(float)*2*nTxCount, SEEK_CUR);
        }

         
        //Face
        fread(&nFaceCount,sizeof(unsigned short),1,fp);			//Face
        //Face 개수를 보고 미리 적당한 타입으로 할당 할 수는 있지만, 파일에 short으로 작성되어 있으므로, 어쨋든 short으로 한번 읽어야 한다. 따라서, 그냥 읽고 난 후, 다시 정리하도록 한다.
        pNewMesh->AllocFaceBuffer(nFaceCount);
        fread(pNewMesh->GetIndexBuffer(),sizeof(unsigned short),nFaceCount*3,fp);
        pNewMesh->ShortenIndexBufferIntoByteIndexBuffer();
        
        
        pNewMesh->UpdateAABB();
        
        //BB
        vec3 mesh_minBB = pNewMesh->GetMinBound();
        vec3 mesh_maxBB = pNewMesh->GetMaxBound();

       
        if (mesh_minBB.x < sbm_minBB.x)  sbm_minBB.x = mesh_minBB.x;
        if (mesh_minBB.y < sbm_minBB.y)  sbm_minBB.y = mesh_minBB.y;
        if (mesh_minBB.z < sbm_minBB.z)  sbm_minBB.z = mesh_minBB.z;
        
        if (mesh_maxBB.x > sbm_maxBB.x)  sbm_maxBB.x = mesh_maxBB.x;
        if (mesh_maxBB.y > sbm_maxBB.y)  sbm_maxBB.y = mesh_maxBB.y;
        if (mesh_maxBB.z > sbm_maxBB.z)  sbm_maxBB.z = mesh_maxBB.z;
        
        
        // VtCount와 NrmCount가 다른 경우 재계산
		//////////////////////////
        
        if(nVtCount!=nNrmCount && nNrmCount==0)//Normal이 0이라서 명시적으로 계산을 하도록 한 경우와 개수가 틀려서 다시 계산하는 경우
        {
#ifdef VB_CORE_DEBUG
            std::cout << "[WARNNING] Recalc normal for Mesh(" << nMeshID << ").\r\n";
#endif
            pNewMesh->recalcNormalBuffer();
        }
		//////////////////////////


        pNewMesh = NULL;
    }

	////// End
    
    if(bAsStatic==true)    pGroupMng->AddGroup(file_name, sbm_minBB, sbm_maxBB, true);
    
#ifdef VB_CORE_DEBUG
    elapsed = mach_absolute_time() - start;
    mach_timebase_info(&sTimebaseInfo);
    printf("elapsed time : %f\n", elapsed * (float)sTimebaseInfo.numer / (float)sTimebaseInfo.denom / 1000000.0);
    // 시간 체크
#endif
    
    return true;
}
