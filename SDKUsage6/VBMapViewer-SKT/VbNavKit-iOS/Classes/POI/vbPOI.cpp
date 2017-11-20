//
//  vbPOI.cpp
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 10. 25..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "vbPOI.h"

vbPOI::vbPOI()
{
    memset(dbKey, 0, sizeof(char)*_POI_KEY_LEN);
    memset(mszName, 0, sizeof(char)*_POI_NAME_LEN); 
    mState = vbPOI_STATE_ACTIVATED;  
    mType = vbPOI_TYPE_POSITIONAL;
}

vbPOI::~vbPOI()
{
    
}

void vbPOI::SetDBKeyWithName(char* db_key, char *szName)
{
//    strcpy(dbKey, db_key);
//    strcpy(mszName, szName);
    memset(dbKey, 0, sizeof(char)*_POI_KEY_LEN);
    memcpy(dbKey, db_key, sizeof(char)*_POI_KEY_LEN);
    memset(mszName, 0, sizeof(char)*_POI_NAME_LEN);
    memcpy(mszName, szName, sizeof(char)*_POI_NAME_LEN);
}

void    vbPOI::SetPOIAsPin(int nMapID,
                    int nPOIID,
                    vec3 pos3D,
                    const char* strNameAsDBKey,
                    int lenDBKey,
                    const char* strDescAsName,
                    int lenName)
{
    SetPOIState(vbPOI::vbPOI_STATE_INACTIVATED);
    Set3DPos(pos3D);
    SetMapID(nMapID);
    SetPOIID(nPOIID);
    memset(dbKey, 0, sizeof(char)*_POI_KEY_LEN);
    memcpy(dbKey, strNameAsDBKey, sizeof(char)*lenDBKey);
    memset(mszName, 0, sizeof(char)*_POI_NAME_LEN);
    memcpy(mszName, strDescAsName, sizeof(char)*lenName);
}