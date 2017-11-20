//
//  vbTouchControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 13..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbViewControlSet__
#define __VbNavKit_iOS__vbViewControlSet__

#include <iostream>
#include "Quaternion.hpp"

class vbViewControlSet
{
public:
    vbViewControlSet();
    ~vbViewControlSet();
    
protected:
    bool    m_bOnControl;
    ivec2   m_DownPos_prev;

    
    
public:
    /////////////////// SET/GET
    bool    IsOnControl()                       {   return m_bOnControl;        };
    void    SetControlState(bool bOnControl)    {   m_bOnControl = bOnControl;  };

    
    /////////////////// TOUCH HANDLER
    virtual bool    SingleFingerUp(ivec2 tPos, int tapCount, vec3& pos3d) = 0;
    virtual void    SingleFingerDown(ivec2 tPos, int tapCount) = 0;
    virtual bool    SingleFingerMove(ivec2 tPos, int tapCount) = 0;
    virtual bool    DoubleFingerUp(ivec2 pos1/*pressed*/, int tapCount1,ivec2 pos2/*unpressed*/, int tapCount2) = 0;
    virtual void    DoubleFingerDown(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2) = 0;
    virtual bool    DoubleFingerMove(ivec2 pos1, int tapCount1,ivec2 pos2, int tapCount2) = 0;
  
};
#endif /* defined(__VbNavKit_iOS__vbViewControlSet__) */
