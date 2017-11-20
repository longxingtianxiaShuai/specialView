//
//  vbInstanceFactory.h
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 29..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbInstanceFactory__
#define __VbNavKit_iOS__vbInstanceFactory__

#include <iostream>

#include "vbInstanceManager.h"
#include "../vbMeshes/vbMeshManager.hpp"
#include "../vbEnum.hpp"

class vbInstanceFactory
{
public:
	vbInstanceFactory();
	~vbInstanceFactory();
	
    //Mesh Instance Generator
    static VBMapErrorCode ImportMeshInstanceFromIST(vbInstanceManager* pInstMng, vbMeshManager* pMeshMng, FILE* fp);
    
    static int AddMeshInstance(const char* sbm_name,
                                          unsigned short mesh_ID,
                                          const float* transform_mat,
                                          vbInstanceManager* pInstMng,
                                          vbMeshManager* pMeshMng
                                          );
      
    
private:
    static VBMapErrorCode ImportMeshInstanceFromIST_V1(vbInstanceManager* pInstMng, vbMeshManager* pMeshMng, FILE* fp);
    
};

#endif /* defined(__VbNavKit_iOS__vbInstanceFactory__) */


