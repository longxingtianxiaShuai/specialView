//
//  vbHorizontalPanCameraControl.cpp
//  VbNavKit-iOS
//
//  Created by Dev4_Air on 13. 2. 7..
//  Copyright (c) 2013년 dev4. All rights reserved.
//

#include "vbHorizontalPanCameraControl.hpp"

vbHorizontalPanCameraControl::vbHorizontalPanCameraControl()
{
    
}

vbHorizontalPanCameraControl::~vbHorizontalPanCameraControl()
{
    
}

vec3  vbHorizontalPanCameraControl::GetPosition()
{
    vec3 new_pos;
    
    return new_pos;
}

bool vbHorizontalPanCameraControl::DoHPanning(vec3& new_ref_pos, vec3 cur_pos)
{
    //변화량을 구한다.
    vec3 diff = new_ref_pos - m_ref_pt;
    //새로운 위치 후보를 구한다.
    new_ref_pos = cur_pos - diff;

//	new_ref_pos = new_ref_pos * 0.25;
	
    //Pan 유효 영역인지 확인한다.
    if (new_ref_pos.x<m_PanBoundMin.x)
		new_ref_pos.x = m_PanBoundMin.x;
    else if (new_ref_pos.x>m_PanBoundMax.x)
		new_ref_pos.x = m_PanBoundMax.x;

    if (new_ref_pos.z<m_PanBoundMin.z)
		new_ref_pos.z = m_PanBoundMin.z;
    else if (new_ref_pos.z>m_PanBoundMax.z)
		new_ref_pos.z = m_PanBoundMax.z;
    
    /* 하나라도 벗어나면 못가게 처리 할 경우, 유효 범위에서 걸리는 듯한 느낌을 준다. 모두 벗어가는 경우만 실패처리하고 그렇지 않으면 허용 가능한 쪽은 움직이도록 한다.
    if (new_ref_pos.x<m_PanBoundMin.x
     //   || new_ref_pos.y<m_PanBoundMin.y      //수평 이동이므로 높이 방향의 제한은 검사하지 않는다. 지하층이 걸리는 이유가 됨.
        || new_ref_pos.z<m_PanBoundMin.z
        || new_ref_pos.x>m_PanBoundMax.x
     //   || new_ref_pos.y>m_PanBoundMax.y
        || new_ref_pos.z>m_PanBoundMax.z)
    {
     
        //reference를 갱신해야 다음 액션이 부드럽다.
        m_ref_pt =  new_ref_pos;
        
        return false;
    }
     */
     
    
    return true;
}
