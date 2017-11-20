#pragma once

#include <stdio.h>
#include "vbMaterialManager.hpp"
#include <string>
using std::string;

class vbMaterialFactory
{
public:
    vbMaterialFactory();
    ~vbMaterialFactory();
    
    static bool ImportMaterialFromPMESH(FILE* fp, int material_count, bool bIsSub);
    //static bool ImportMaterialFromSBMv1(vbMaterialManager* pMtlManager, FILE* fp, int material_count);
    static bool ImportMaterialFromSBMv2(vbMaterialManager* pMtlManager, FILE* fp, int material_count, char* sbm_full_path);
};
