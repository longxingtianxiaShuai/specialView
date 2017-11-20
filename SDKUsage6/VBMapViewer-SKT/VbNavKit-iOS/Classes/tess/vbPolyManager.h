//
//  vbPolyManager.h
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 31..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbPolyManager__
#define __VbNavKit_iOS__vbPolyManager__

#include <iostream>
#include <vector>

class vbPolygon;
class vbPolyManager
{
public:
    vbPolyManager();
    ~vbPolyManager();
    
private:
    std::vector<vbPolygon*>     m_polygons;

public:
    const std::vector<vbPolygon*>*        GetPolygons()       {   return (const std::vector<vbPolygon*>*)&m_polygons;   }
    void            Clear();
    vbPolygon*      CreatePolygon();

    
};

#endif /* defined(__VbNavKit_iOS__vbPolyManager__) */
