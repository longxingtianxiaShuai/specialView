//
//  vbPOI.h
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 10. 25..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef PISAiOS_vbPOI_h
#define PISAiOS_vbPOI_h

#include "Vector.hpp"

#define _POI_KEY_LEN    20
#define _POI_NAME_LEN   128



class   vbPOI
{
    

public:
    enum vbPOIState
    {
        vbPOI_STATE_INACTIVATED = 0,
        vbPOI_STATE_ACTIVATED = 1
    };
    
    enum vbPOIType
    {
        vbPOI_TYPE_UNKNOWN = 0,
        vbPOI_TYPE_POSITIONAL = 1
    };
    
    vbPOI();
    ~vbPOI();
    
private:
    
    char    dbKey[_POI_KEY_LEN];
    char    mszName[_POI_NAME_LEN];  
    vec3    poi3D;
    ivec2   poi2D;
    
    vbPOIState  mState;
    vbPOIType   mType;
    
    int     mMapID;
    int     mPOIID;
    
    
    public:
    
    int     GetMapID()                  {   return mMapID;      };
    void    SetMapID(int nMapID)        {   mMapID = nMapID;    };
    
    int     GetPOIID()                  {   return mPOIID;      };
    void    SetPOIID(int nPOIID)        {   mPOIID = nPOIID;    };
    
    ivec2   Get2DPos()                  {   return poi2D;       };
    void    Set2DPos(ivec2 win_pos)     {   poi2D = win_pos;    };

    vec3    Get3DPos()                  {   return poi3D;       };
    void    Set3DPos(vec3 world_pos)    {   poi3D = world_pos;  };
    
    char*   GetDBKey()                  {   return dbKey;       };
    
    char*   GetName()                  {   return mszName;       };
    
    //void    SetDBKey(char* db_key);
    void SetDBKeyWithName(char* db_key, char *szName);
    
    vbPOIState  GetPOIState()                   {   return mState;   };
    void        SetPOIState(vbPOIState state)   {   mState = state;  };
    
    vbPOIType   GetPOIType()                   {   return mType;   };
    void        SetPOIType(vbPOIType type)     {   mType = type;   };
    
    //
    void    SetPOIAsPin(int nMapID,
                        int nPOIID,
                        vec3 pos3D,
                        const char* strNameAsDBKey,
                        int lenDBKey,
                        const char* strDescAsName,
                        int lenName);
    

};


#endif
