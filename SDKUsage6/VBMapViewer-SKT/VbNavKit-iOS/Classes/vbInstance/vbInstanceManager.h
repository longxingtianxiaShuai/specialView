//
//  vbInstanceManager.h
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 25..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbInstanceManager__
#define __VbNavKit_iOS__vbInstanceManager__

#include <iostream>
#include <vector>

class vbMeshInstance;

class vbInstanceManager
{
    
public:
    vbInstanceManager();
    ~vbInstanceManager();
    
private:
    std::vector<vbMeshInstance*>    m_pInstances;

    
public:
    const std::vector<vbMeshInstance*>*       GetInstances()
                {   return (const std::vector<vbMeshInstance*>*)&m_pInstances;    }
    
    vbMeshInstance*                             CreateMeshInstance();
    void    Clear();
    
};
#endif /* defined(__VbNavKit_iOS__vbInstanceManager__) */
