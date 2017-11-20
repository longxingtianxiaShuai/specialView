//
//  vbPOIFactory.cpp
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 10. 25..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//


#include "vbPOIFactory.h"

vbPOIFactory::vbPOIFactory()
{
    
}

vbPOIFactory::~vbPOIFactory()
{
    
}

bool vbPOIFactory::ImportPOIfromBinV1(vbPOIManager* pPOIManager, FILE* fp, int poi_count)
{        
	int mnVersion;		//! Version
	int mnSpaceCount;	//! # of Space
	int mnPOICount;		//! # of POI    

    /////////////// Header
    //Version
    fread(&mnVersion,sizeof(int),1,fp);
    if (mnVersion!=1)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "The parser was for version 1. But the version of input file isn't 1.\r\n";
#endif
        return false;
    }
    
    //Contents count
    fread(&mnSpaceCount,sizeof(int),1,fp);//스페이스 개수
    fread(&mnPOICount,sizeof(int),1,fp);//POI 개수
  
    int nCode =0;
    bool bContinue = true;
    while(bContinue)
    {
        
        fread(&nCode,sizeof(int),1,fp);
        switch(nCode)
        {
			case CODE_SPACE:
            {
                bContinue = ReadOneSpaceData(fp); //사용처가 아직 정해지지 않았으므로, 일단 읽기만 하고 저장하지 않음.
                break;
            }
			case CODE_POI:
            {
                bContinue = ReadOnePOIData(pPOIManager,fp);
                break;
            }
			case CODE_ENDFILE:
            {
                bContinue = false;
                return true;
                break;
            }
			default:
            {
                bContinue = false;
                break;
            }
				
        }
    }    
    
    return false; //End code를 만나지 못했음.
    
 

}

bool vbPOIFactory::ReadOnePOIData(vbPOIManager* pPOIManager, FILE* pSrcFile)
{
    return false;
    
    /*
    vbPOI*  newPOI = NULL;
    vec3    poi_pos;
    poi_pos.y = 0.f;
    
	bool bRet = true;
	char chName[256]={0};
    memset(chName, 0, 256);
	
	double vert[2];
	fread(&chName[0],sizeof(char)*256,1,pSrcFile);//Key
	fread(&vert[0],sizeof(double),2,pSrcFile);//vertex

    
    newPOI = pPOIManager->CreatePOI();
    newPOI->SetDBKey(chName);
    poi_pos.x = (float)vert[0];
    poi_pos.z = -(float)vert[1];
    newPOI->Set3DPos(poi_pos);
    
    
    
    newPOI = NULL;
    
	return bRet;
     */
}

bool vbPOIFactory::ReadOneSpaceData(FILE* pSrcFile)
{
    //일단 읽기만 하고 저장하지 않음.
    
	bool bRet = true;
	char chName[256]={0};
	int nVertCount =0;
	double* pVertexBuffer = 0;
    
	fread(&chName[0],sizeof(char),256,pSrcFile);//key value, name
	fread(&nVertCount,sizeof(int),1,pSrcFile);//vertex count
    
	pVertexBuffer = new double[nVertCount*2];
	fread(pVertexBuffer,sizeof(double),nVertCount*2,pSrcFile);//vertex info
    
	delete [] pVertexBuffer;

	return bRet;
     
}
