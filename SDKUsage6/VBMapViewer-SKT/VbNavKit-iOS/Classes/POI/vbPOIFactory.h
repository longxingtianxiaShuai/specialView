//
//  vbPOIFactory.h
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 10. 25..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PISAiOS_vbPOIFactory_h
#define PISAiOS_vbPOIFactory_h
#include <iostream>
#include <stdio.h>

#include "vbPOIManager.h"


class vbPOIFactory
{
public:
    vbPOIFactory();
    ~vbPOIFactory();
    
    enum enumCode
    {
        CODE_SPACE = 100,		//Space code
        CODE_POI = 200,			//POI code
        CODE_ENDFILE = 10000,	//Finilize code
    };    
    
    
    /*
     bool ReadData(FILE* fp, bool bShowlog=false);
     //! Header Data
     bool _getHeader(FILE* pFile,bool bShowLog=false);
     //! Space Data
     bool _getSpaceData(FILE* pSrcFile,bool bShowLog=false);
     //! POI Data
     bool _getPOIData(FILE* pSrcFile,bool bShowLog=false);

     */
    
    
    //Parsing
    static bool ImportPOIfromBinV1(vbPOIManager* pPOIManager, FILE* fp, int poi_count);

private:
    static bool ReadOneSpaceData(FILE* pSrcFile);
    static bool ReadOnePOIData(vbPOIManager* pPOIManager, FILE* pSrcFile);
      
};


#endif
