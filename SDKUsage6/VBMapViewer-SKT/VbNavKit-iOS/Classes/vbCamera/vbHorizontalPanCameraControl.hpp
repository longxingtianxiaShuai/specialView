//
//  vbHorizontalPanCameraControl.h
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#ifndef __VbNavKit_iOS__vbHorizontalPanCameraControl__
#define __VbNavKit_iOS__vbHorizontalPanCameraControl__


#include "vbMoveCameraControl.hpp"

class vbHorizontalPanCameraControl : public vbMoveCameraControl
{
    
public:
    vbHorizontalPanCameraControl();
    ~vbHorizontalPanCameraControl();
    
private:
    vec3    m_PanBoundMin;
    vec3    m_PanBoundMax;
    vec3    m_ref_pt;
    
    
    
public:
    //virtual func
    vec3    GetPosition();
    
    //Set/Get
    vec3    GetPanBoundMin()                        {   return m_PanBoundMin;           };
    void    SetPanBoundMin(vec3 pan_bound_min){
		m_PanBoundMin = pan_bound_min;
//		m_PanBoundMin.x = -14434.0195f;
//		m_PanBoundMin.z = 14606.0000f;
	};
    vec3    GetPanBoundMax(){   return m_PanBoundMax;           };
    void    SetPanBoundMax(vec3 pan_bound_max){
		m_PanBoundMax = pan_bound_max;
//		m_PanBoundMax.x = 14434.0195f;
//		m_PanBoundMax.z = 14606.0000f;
	};
    vec3    GetRefPoint()                           {   return m_ref_pt;                };
    void    SetRefPoint(vec3 ref_pt)                {   m_ref_pt = ref_pt;              };
    
    //Function
    bool    DoHPanning(vec3& new_ref_pos, vec3 cur_pos);
    
    
};
#endif /* defined(__VbNavKit_iOS__vbHorizontalPanCameraControl__) */
