#include "vbViewInfo.hpp"

#include <iostream>
#include <cmath>

///////////////////// Navigation

void    vbViewInfo::InvalidateManipulation()
{
    m_cam_navi_3d_mode=NAVIGATION_3D_NONE;
    m_bNavigation=false;
}


bool vbViewInfo::SetNavigationMode(NavigationMode nMode)
{
    
    //mode  변경 시 뷰가 변경 될 경우.
    switch (nMode)
    {
        case NAVIGATION_3D:
        {
            if(m_cam_navi_mode==NAVIGATION_WALK) return false;
            
            ReturnTo3DView();
            
        }
        break;
            
        case NAVIGATION_2D:
        {
            if(m_cam_navi_mode==NAVIGATION_WALK) return false;            
            SetTopView();
            //FitAABBtoScreen();  //현재 카메라 위치를, 현재 카메라 거리에서 보도록한다.
        }
        break;
            
        case NAVIGATION_WALK:
        {
            if(m_cam_navi_mode==NAVIGATION_WALK)
            {
                EndFPS();
                m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
                m_cam_navi_mode = NAVIGATION_3D;
                return true;
            }
            else if(m_cam_navi_mode==NAVIGATION_3D)
                InitWalk();
            else
                return false;
        }
        break;
            
        case NAVIGATION_ME:
        {
            //현재 모드를 백업 해 둔다.
            m_cam_navi_mode_backup = m_cam_navi_mode;
        }
        break;
            
        case NAVIGATION_ENVRN:
        {
            //현재 모드를 백업 해 둔다.
            m_cam_navi_mode_backup = m_cam_navi_mode;
            SetEnvironmentalCamera();
            
        }
        break;
            
        default:
            return false;
            break;
    }
    m_cam_navi_mode = nMode;    
    return true;
}

void vbViewInfo::RecoverNavigationMode()
{
    m_cam_navi_mode = m_cam_navi_mode_backup;
}


///////////////////// Orbit Rotation
void vbViewInfo::StartFreeRotation(ivec2 finger_pos)
{
    //이후에는 카메라 포인터를 통해 직접 가져 오도록 수정 함.
    m_CameraFreeControl.SetOrientation(m_Camera.GetCameraOrientation());
    m_CameraFreeControl.SetTrackballStart(MapToSphere(finger_pos));
    
    m_DownPos_prev=finger_pos;
    m_bNavigation = true;
}

void vbViewInfo::StartRotation(ivec2 finger_pos)
{
    
    //이후에는 카메라 포인터를 통해 직접 가져 오도록 수정 함.
    m_CameraFreeControl.SetOrientation(m_Camera.GetCameraOrientation());
    
    ivec2 scr_size = m_CameraPerspective.GetScreenSize();

    //Horizontal rotation
    ivec2 prv = finger_pos;
    prv.y = scr_size.y/2;
    m_cam_reference3D_first =  MapToSphere(prv);
        
    //Vertical rotation
    prv = finger_pos;
    prv.x = scr_size.x/2;
    m_cam_reference3D_second =  MapToSphere(prv);
        
    m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
        

    m_DownPos_prev=finger_pos;

    m_bNavigation = true;
}

void vbViewInfo::StartHRotation(ivec2 finger_pos)
{
    //Horizontal rotation
    ivec2 scr_size = m_CameraPerspective.GetScreenSize();
    
    ivec2 prv = finger_pos;
    prv.y = scr_size.y/2;
    m_cam_reference3D_first =  MapToSphere(prv);
    
    m_DownPos_prev=finger_pos;
    
    m_bNavigation = true;
}


void vbViewInfo::StartVRotation(ivec2 finger_pos)
{
     //Horizontal rotation
    ivec2 scr_size = m_CameraPerspective.GetScreenSize();
    ivec2 prv = finger_pos;
    prv.x = scr_size.x/2;
    m_cam_reference3D_second =  MapToSphere(prv);
    
    m_DownPos_prev=finger_pos;
    
    m_bNavigation = true;
}

void    vbViewInfo::StartYawPitchRotation(ivec2 finger_pos)
{
    //위치가 아니라 변화량을 가지고 계산하므로 화면상의 위치만 추적한다.
    m_DownPos_prev=finger_pos;
    m_bNavigation = true;
}

bool vbViewInfo::DoFreeRotation(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        m_CameraFreeControl.AddOrientation(MapToSphere(touchpoint));
        
        //이후에는 컨트롤러에서 직접 입력하도록 함.
        //m_Camera.SetCameraOrientation(m_CameraFreeControl.GetOrientation());
        // AddOrientation에서 직접 설정 하도록 함. - 잠시 놔뒀다가 특별한 문제가 안생기면 주석 처리 된 코드는 삭제함.

        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
        
        UpdateCameraPositionFromOrbit();
        UpdateViewMatrix();
        return true;
    }
    return false;
    
}

bool vbViewInfo::DoOrbit(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        vec3 end;
        Quaternion delta;

        ivec2 diff = touchpoint - m_DownPos_prev;
            
        if(m_cam_navi_3d_mode ==NAVIGATION_3D_NONE && diff.Length()>5)
        {
            if (abs(diff.x)>abs(diff.y))    m_cam_navi_3d_mode = NAVIGATION_3D_ORBIT_H;
            else                            m_cam_navi_3d_mode = NAVIGATION_3D_ORBIT_V;
        }
             
        if (m_cam_navi_3d_mode == NAVIGATION_3D_ORBIT_H)
        {//Rotate about World Y axis
            m_CameraTrackControl.AddYawPitchDegree(float(diff.x), 0);
            //m_Camera.SetCameraOrientation(m_CameraTrackControl.GetOrientation()); - 내부에서 처리
        }
        else if (m_cam_navi_3d_mode == NAVIGATION_3D_ORBIT_V)
        {//Rotate about Orbit center X axis
            m_CameraTrackControl.AddYawPitchDegree(0.f,-float(diff.y)/2.f);
            //m_Camera.SetCameraOrientation(m_CameraTrackControl.GetOrientation());
        }
        m_DownPos_prev = touchpoint;
        
        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
        
        UpdateCameraPositionFromOrbit();
        UpdateViewMatrix();
        return true;
    }    
    return false;
}
 
bool vbViewInfo::DoYawRotation(ivec2 touchpoint)
{
    if (m_bNavigation)
    {        
        ivec2 diff = touchpoint - m_DownPos_prev;
       
        m_CameraTrackControl.AddYawPitchDegree(float(diff.x)/2.f, 0);
        //m_Camera.SetCameraOrientation(m_CameraTrackControl.GetOrientation());
        
        m_DownPos_prev = touchpoint;
             
        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
        
        UpdateCameraPositionFromOrbit();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

//수평 연속 회전
bool vbViewInfo::DoContinuousYawRotation(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        //화면 크기
        ivec2 cntr = m_CameraPerspective.GetScreenSize()/2;
        
        ivec2 prv =  m_DownPos_prev - cntr;
        ivec2 cur = touchpoint - cntr;
        
        float cross = prv.x * cur.y - cur.x * prv.y;//부호 판단
        double vec_size = sqrt((float)(prv.x * prv.x + prv.y * prv.y)) * sqrt((float)(cur.x * cur.x + cur.y * cur.y));
        
        float theta = 0.f;
        theta = RAD2DEG(acos((double)(prv.x*cur.x + prv.y * cur.y)/vec_size));
        if(theta>0 && theta<180.f)
        {
            if(cross<0.f)   theta = -theta;
            m_CameraTrackControl.AddYawPitchDegree(-theta, 0);
        }
        //std::cout << "Vec-Size = " << vec_size << ", Theta = " << theta << std::endl;
        
        m_DownPos_prev = touchpoint;
        
        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
        
        UpdateCameraPositionFromOrbit();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

bool vbViewInfo::DoPitchRotation(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        ivec2 diff = m_DownPos_prev - touchpoint;
        
        m_CameraTrackControl.AddYawPitchDegree(0.f,float(diff.y)/2.f);
        //m_Camera.SetCameraOrientation(m_CameraTrackControl.GetOrientation());
        
        m_DownPos_prev = touchpoint;
        
        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
        
        UpdateCameraPositionFromOrbit();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

bool vbViewInfo::DoYawPitchRotation(ivec2 tPos)
{
    if(!m_bNavigation)  return false;
    
    float h_deviation = (tPos.x - m_DownPos_prev.x)/4.f;    //수평 방향 변위
    float v_deviation = -(tPos.y - m_DownPos_prev.y)/4.f;    //수평 방향 변위
    
    //픽셀 이동량의 1/10 각도를 회전 시킨다.
    m_CameraTrackControl.AddYawPitchDegree(h_deviation, v_deviation);
    //m_Camera.SetCameraOrientation(m_CameraTrackControl.GetOrientation());
    
    m_DownPos_prev = tPos;
    
    UpdateCameraPositionFromOrbit();
    UpdateViewMatrix();
    return true;
    
}
bool vbViewInfo::DoLandscapeYawPitchRotation(ivec2 tPos)
{
    if(!m_bNavigation)  return false;
    
    float h_deviation = (tPos.x - m_DownPos_prev.x)/10.f;    //수평 방향 변위
    float v_deviation = -(tPos.y - m_DownPos_prev.y)/10.f;    //수평 방향 변위
    
    //픽셀 이동량의 1/10 각도를 회전 시킨다.
    m_CameraTrackControl.AddYawPitchDegree(v_deviation, h_deviation);
    
    m_DownPos_prev = tPos;
         
    UpdateViewMatrix();
    return true;

}

void vbViewInfo::EndRotation()
{
    m_bNavigation = false;
}

void vbViewInfo::JumpOrbit(ivec2 finger_pos)
{
    InitOrientation(false);
    UpdateCameraPositionFromOrbit();
    UpdateViewMatrix();
    m_bNavigation = false;    
}

///////////////////// Orbit Rotation


///////////////////// Panning
void vbViewInfo::StartHPanning(ivec2  finger_pos)
{
    //클릭 된 지점에서 ray를 구한다.
    vec3 intersect_pt = GetRayIntersectionWithGround(finger_pos);
       
    //Pan 시작 지점을 저장한다.
    m_CameraHPanControl.SetRefPoint(intersect_pt);
    
    float dist = (m_Camera.GetCameraPosition() - m_CameraTrackControl.GetTrackCenter()).size();
    
    m_CameraZoomControl.SetDistance(dist);
    
    m_bNavigation = true;
    m_cam_navi_3d_mode = NAVIGATION_3D_PAN;
}

bool vbViewInfo::DoHPanning(ivec2 finger_pos)
{
    
    if (m_bNavigation && m_cam_navi_3d_mode==NAVIGATION_3D_PAN)
    {
        //터치 된 지점을 구한다.
        vec3 pos = GetRayIntersectionWithGround(finger_pos); 
        
        //변화량을 구하여 중심에 더해, 유효한 영역인지 확인한다. 유효하면 변경 된 좌표를 반영하여 true를 리턴한다.
        if(!m_CameraHPanControl.DoHPanning(pos,m_CameraTrackControl.GetTrackCenter())) return false;
        
        //새로 panning 된 위치를 회전에도 반영 할 수 있도록 한다. - Panning중에는 회전과 무관하니 참조 위치를 다른 위치로 바꿔야 할 것이다.
        m_CameraTrackControl.SetTrackCenter(pos);
        
        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
              
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        return true;
        
        
    }    
    return false;
}

void vbViewInfo::EndHPanning()
{
    m_bNavigation = false;
    m_cam_navi_3d_mode = NAVIGATION_3D_NONE;        
}

void vbViewInfo::JumpPanning(ivec2 finger_pos)
{
    vec3 new_cntr = GetRayIntersectionWithGround(finger_pos); 
    JumpPanning(new_cntr);
}

bool vbViewInfo::StartHRotationAnimation(float fStepYaw)
{
    if(m_CameraAnimationControl.IsAnimationUsed())
    {
        m_CameraAnimationControl.SetYawStep(fStepYaw);
        m_cam_ani_mode              = CAMERA_ANI_YAW_ROTATE;
        
        m_CameraAnimationControl.SetProgressRatio(0.f);
        m_CameraAnimationControl.SetAnimating(true);
        
    }
    return true;
}

bool vbViewInfo::EndHRotationAnimation()
{
    m_cam_ani_mode = CAMERA_ANI_NONE;
    m_CameraAnimationControl.SetAnimating(false);
    
    return true;
}

void vbViewInfo::JumpPanning(vec3 new_orbit_cntr)
{
    if(m_CameraAnimationControl.IsAnimationUsed())
    {
        vec3 diff = new_orbit_cntr  - m_CameraTrackControl.GetTrackCenter();
        Quaternion cam_rot = m_Camera.GetCameraOrientation();
        InitCameraMoveAni(m_Camera.GetCameraPosition(), m_Camera.GetCameraPosition() + diff, cam_rot, cam_rot, CAMERA_ANI_JUMPPAN);
    }
    else
    {
        //현재 위치를 구한다.
        m_CameraTrackControl.SetTrackCenter(new_orbit_cntr);
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        m_bNavigation = false;
        float cam_distance = (m_CameraTrackControl.GetTrackCenter() - m_Camera.GetCameraPosition()).size();
        m_CameraZoomControl.SetDistance(cam_distance);
        
    }
}

void    vbViewInfo::JumpPanRotation(vec3 new_orbit_cntr, Quaternion new_orientation)
{
    if(m_CameraAnimationControl.IsAnimationUsed())
    {
        InitCameraMoveAni(m_CameraTrackControl.GetTrackCenter(), new_orbit_cntr, m_Camera.GetCameraOrientation(), new_orientation, CAMERA_ANI_JUMPPANROT);
    }
    else
    {
        //현재 위치를 구한다.
        m_CameraTrackControl.SetTrackCenter(new_orbit_cntr);
        m_Camera.SetCameraOrientation(new_orientation);
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        m_bNavigation = false;
        float cam_distance = (m_CameraTrackControl.GetTrackCenter() - m_Camera.GetCameraPosition()).size();
        m_CameraZoomControl.SetDistance(cam_distance);
        
    }
}

void vbViewInfo::UpdateValidRange(vec3 bbMin, vec3 bbMax)
{
    //for Dynamic near clip control
    m_bound_min = bbMin;
    m_bound_max = bbMax;

    m_rad_bounding_sphere = (bbMax - bbMin).size()/2.f;
    m_bounding_center = (bbMax+bbMin)/2.f;
    
    
  
    //meter단위로 export 된 모델인 경우
    if (m_rad_bounding_sphere < 1000.f) {
        m_min_near_clip = 1.f;
        m_CameraZoomControl.SetDistanceMin(10.f);
        m_CameraZoomControl.SetDistanceMax(m_rad_bounding_sphere * 3.f);
        
        m_pan_valid_ratio = 5.f;
    }
    else
    {
        //Max. distance
        const float*    perspective_param = m_CameraPerspective.GetPerspectiveParam();
        
        float max_dist = (m_rad_bounding_sphere * 2.f * m_cam_fMaxDistRatioBBSize) / tan(DEG2RAD(perspective_param[0]*perspective_param[1]));
        m_CameraZoomControl.SetDistanceMax(max_dist);
        m_CameraZoomControl.SetDistanceMin(10.f);

        m_pan_valid_ratio = 2.f;
        
    }
    
    //for pan input validation
    vec3 expand = (bbMax - bbMin)*m_pan_valid_ratio;
    m_CameraHPanControl.SetPanBoundMin(bbMin - expand);
    m_CameraHPanControl.SetPanBoundMax(bbMax + expand);
}

void    vbViewInfo::SetPanValidRange(vec3 min, vec3 max, float pRatio)
{
    m_pan_valid_ratio = pRatio;
	
	//	아래 0.25를 해주어야지만 화면을 이동시 4분할 한 범위로 이동하지 않고 1:1로 이동한다.	shin 절반 화면이동
//	min.x *= 0.5f;
//	max.x *= 0.5f;
//	
//	min.z *= 0.5f;
//	max.z *= 0.5f;
	
	//////////////////////////////////////////////////////////////////////////////////////
	
    m_CameraHPanControl.SetPanBoundMin(min);
    m_CameraHPanControl.SetPanBoundMax(max);
	
	
//	if (new_ref_pos.x<m_PanBoundMin.x)          new_ref_pos.x = m_PanBoundMin.x;
//	else if (new_ref_pos.x>m_PanBoundMax.x)    new_ref_pos.x = m_PanBoundMax.x;
//	
//	if (new_ref_pos.z<m_PanBoundMin.z)          new_ref_pos.z = m_PanBoundMin.z;
//	else if (new_ref_pos.z>m_PanBoundMax.z)    new_ref_pos.z = m_PanBoundMax.z;

}

///////////////////// Panning


///////////////////// Zoom
//Zoom
void vbViewInfo::StartZooming(ivec2 finger_pos)
{
    //현재 카메라 거리를 구한다.
    m_CameraZoomControl.SetBaseDistance(GetCameraDistance());
    
    //Zoom 시작 지점을 저장한다.
    m_DownPos_prev = finger_pos;
    m_bNavigation = true;    
}

bool vbViewInfo::DoZooming(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        //변화량을 구한다.
        ivec2   diff = touchpoint - m_DownPos_prev;
        
        ivec2   scr_size = m_CameraPerspective.GetScreenSize();
        
        float ratio = (float)diff.y/(float)scr_size.y;
        
        float cam_distance = m_CameraZoomControl.GetBaseDistance() * ( 1.f - ratio);
        m_CameraZoomControl.SetDistance(cam_distance);
        
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        return true;
    }     
    
    return false;
}

void vbViewInfo::EndZooming()
{
    m_bNavigation = false;    
}

void    vbViewInfo::StartZooming2Pt(ivec2 finger_pos1,ivec2 finger_pos2)
{
    //현재 카메라 거리를 구한다.
    m_CameraZoomControl.StartZoomWithRatio((finger_pos1-finger_pos2).size());
    
    m_bNavigation = true;
}

bool    vbViewInfo::DoZooming2Pt(ivec2 finger_pos1,ivec2 finger_pos2)
{
    if (m_bNavigation)
    {
        //터치 사이 거리 비로 배율, 즉 카메라 거리를 갱신한다.
        m_CameraZoomControl.UpdateDistanceWithBaseDistanceRatio((float)(finger_pos1-finger_pos2).size());

         //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
        
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        return true;
    }
    
    return false;
}



void    vbViewInfo::StartZooming2PtEnvironment(ivec2 finger_pos1,ivec2 finger_pos2)
{
    //현재 카메라 거리를 구한다.
    //m_cam_fYawPitchOffset = GetCameraDistance();
    
    //Zoom 시작 지점을 저장한다.
    m_DownPos_prev  = (finger_pos1-finger_pos2);
    
    m_CameraZoomControl.SetBaseDistance(m_CameraZoomControl.GetDistance());
    m_CameraZoomControl.SetBaseRatio((finger_pos1-finger_pos2).size());
    
    m_bNavigation = true;
}

bool    vbViewInfo::DoZooming2PtEnvironment(ivec2 finger_pos1,ivec2 finger_pos2)
{
    if (m_bNavigation)
    {
        //변화량을 구한다.
        //ivec2   diff = (finger_pos1-finger_pos2) - m_DownPos_prev;
        
        m_CameraZoomControl.UpdateDistanceFromBaseDistPlusBaseDistRatio((finger_pos1-finger_pos2).size());
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        return true;
    }
    
    return false;
}


/** \brief
    JumpZoom
 \remark
    입력 된 지점이 화면의 중앙이 되고, 지정 된 배율로 Zoom이 됨.
 \return
    void
 */
void vbViewInfo::JumpZooming(ivec2 finger_pos ///< 입력 된 윈도우 좌표 
                             )
{
    if(m_CameraAnimationControl.IsAnimationUsed())
    {
        m_CameraZoomControl.SetBaseDistance(GetCameraDistance());
        
        vec3    new_cntr = m_CameraTrackControl.GetTrackCenter();
        Quaternion cam_rot = m_Camera.GetCameraOrientation();
        if(m_cam_jumpzoom_with_jumppan)     new_cntr = GetRayIntersectionWithGround(finger_pos);
        InitCameraMoveAni(m_CameraTrackControl.GetTrackCenter(), new_cntr, cam_rot, cam_rot, CAMERA_ANI_JUMPZOOM);
        
        //std::cout << "JUMPPAN CENTER = " << new_cntr.x << " , " << -new_cntr.z << "\r\n";

        
    }
    else
    {
        if(m_cam_jumpzoom_with_jumppan) JumpPanning(finger_pos);
        
        float cam_distance = m_CameraZoomControl.GetDistance();
        if(!m_cam_bJumpZoomed)
            cam_distance = cam_distance * m_cam_jumpzoom_ratio;//Zoom in
        else
            cam_distance = cam_distance / m_cam_jumpzoom_ratio;//Zoom out
        
        //Check Zoom limt(Camera distance)
        if(cam_distance<m_CameraZoomControl.GetDistanceMin())       cam_distance = m_CameraZoomControl.GetDistanceMin();
        else if(cam_distance>m_CameraZoomControl.GetDistanceMax())  cam_distance = m_CameraZoomControl.GetDistanceMax();
        
        m_CameraZoomControl.SetDistance(cam_distance);
        
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        m_bNavigation = false;         
    }

}


/*
    가장 확대 된 상태에서는 원래 위치로 돌아가고
    가장 확대 된 상태가 아닌 경우에는 가장 확대 된 상태로 간다.
 */

void vbViewInfo::SphereJumpZooming(ivec2 finger_pos ///< 입력 된 윈도우 좌표
                             )
{
    
    if(m_CameraAnimationControl.IsAnimationUsed())
    {
        vec3 start(0,0,0),end(0,0,0);
        
        if(m_CameraZoomControl.GetDistance() > m_CameraZoomControl.GetDistanceMax()*0.9f)
        {
            //zoom out --> zoom in
            start.z = m_CameraZoomControl.GetDistance();
            end.z = m_CameraZoomControl.GetDistanceMin();
        }
        else
        {
            //zoom in --> zoom out
            start.z = m_CameraZoomControl.GetDistance();
            end.z = m_CameraZoomControl.GetDistanceMax();
        }
        InitCameraMoveAni(start, end, m_Camera.GetCameraOrientation(), m_Camera.GetCameraOrientation(), CAMERA_ANI_JUMPZOOM);
    }
    else
    {
        //Max * 0.9보다 멀면 Min으로, 그렇지 않으면 MAX로 변경
        m_CameraZoomControl.ToggleDistanceLimit();
                 
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        m_bNavigation = false;
    }

}



///////////////////// Pan & Zoom
void vbViewInfo::StartPanZoom(ivec2 pos1, ivec2 pos2)
{
    //////////////// ZOOM ////////////////////

    //현재 카메라 거리를 구한다.
    m_CameraZoomControl.SetDistanceMin(m_pConf->fCamMinDistance);
    m_CameraZoomControl.StartZoomWithRatio((pos1-pos2).size());
    
    ///////////////// PAN ///////////////////////
    //클릭 된 지점에서 ray를 구한다.
    ivec2 center = (pos1+pos2)/2;
    vec3 intersect_pt = GetRayIntersectionWithGround(center);
    
    //Pan 시작 지점을 저장한다.
    m_CameraHPanControl.SetRefPoint(intersect_pt);
    
    
    m_DownPos_prev  = center;
    m_bNavigation = true;
    
}

bool vbViewInfo::DoPanZoom(ivec2 pos1, ivec2 pos2)
{
    if (m_bNavigation)
    {
        ////////////////// Zoom /////////////////////
        //터치 사이 거리 비로 배율, 즉 카메라 거리를 갱신한다.
        m_CameraZoomControl.UpdateDistanceWithBaseDistanceRatio((float)(pos1-pos2).size());

         ////////////////// Pan  //////////////////////
        //Orbit center for Pan
        ivec2 center = (pos1+pos2)/2;
        //현재 위치를 구한다.
        vec3 pos = GetRayIntersectionWithGround(center);
 
        //변화량을 구하여 중심에 더해, 유효한 영역인지 확인한다. 유효하면 변경 된 좌표를 반영하여 true를 리턴한다.
        if(!m_CameraHPanControl.DoHPanning(pos,m_CameraTrackControl.GetTrackCenter())) return false;
        
        //새로 panning 된 위치를 회전에도 반영 할 수 있도록 한다. - Panning중에는 회전과 무관하니 참조 위치를 다른 위치로 바꿔야 할 것이다.
        m_CameraTrackControl.SetTrackCenter(pos);
 
        
        /////////////////// Update Camera //////////////////
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();        
        
        return true;
    }
    return false;
}

void vbViewInfo::EndPanZoom()
{
    m_bNavigation = false;    
    
}

///////////////////// Zoom


///////////////////// HRotation & Zoom
void vbViewInfo::StartHRotateZoom(ivec2 pos1, ivec2 pos2)
{
    //////////////// ZOOM ////////////////////
    
    //현재 카메라 거리를 구한다.
    m_CameraZoomControl.StartZoomWithRatio((pos1-pos2).size());
    
    ///////////////// ROTATE ///////////////////////
    
    m_cam_navi_3d_mode = NAVIGATION_3D_ZOOMHROTATION;
    
    m_cam_reference3D_first.x = pos1.x;
    m_cam_reference3D_first.y = pos1.y;
    m_cam_reference3D_first.z = 0.f;
    
    m_cam_reference3D_second.x = pos2.x;
    m_cam_reference3D_second.y = pos2.y;
    m_cam_reference3D_second.z = 0.f;
    
    m_bNavigation = true;
    
}

bool vbViewInfo::DoHRotateZoom(ivec2 pos1, ivec2 pos2)
{
    if(m_cam_navi_3d_mode != NAVIGATION_3D_ZOOMHROTATION)
    {
        StartHRotateZoom(pos1,pos2);
        return true;
    }
    
    if (m_bNavigation)
    {

        
        ////////////////// Zoom /////////////////////
        //터치 사이 거리 비로 배율, 즉 카메라 거리를 갱신한다.
        if (m_bBrokeZoom==false)
        {
            m_CameraZoomControl.UpdateDistanceWithBaseDistanceRatio((float)(pos1-pos2).size());
        }
        
        ////////////////// Rotation  //////////////////////
        if (m_bBrokeRotate==false)
        {
            //대응하는 점 찾기
            ivec2 canditate1;
            canditate1.x = m_cam_reference3D_first.x;
            canditate1.y = m_cam_reference3D_first.y;
            
            ivec2 prv,cur;
            if ((canditate1-pos1).Length() < (canditate1-pos2).Length())
            {//pos1 = first
                prv.x = m_cam_reference3D_first.x - m_cam_reference3D_second.x;
                prv.y = m_cam_reference3D_first.y - m_cam_reference3D_second.y;
                
                cur = pos1 - pos2;
                
                m_cam_reference3D_first.x = pos1.x;     m_cam_reference3D_first.y = pos1.y;
                m_cam_reference3D_second.x = pos2.x;    m_cam_reference3D_second.y = pos2.y;
            }
            else
            {//pos2 = first
                prv.x = m_cam_reference3D_second.x - m_cam_reference3D_first.x;
                prv.y = m_cam_reference3D_second.y - m_cam_reference3D_first.y;
                
                cur = pos2 - pos1;
                
                m_cam_reference3D_first.x = pos2.x;     m_cam_reference3D_first.y = pos2.y;
                m_cam_reference3D_second.x = pos1.x;    m_cam_reference3D_second.y = pos1.y;
            }
            
            //std::cout << "First X = " << m_cam_reference3D_first.x << " , Y = " << m_cam_reference3D_first.y <<  ", Second X = " << m_cam_reference3D_second.x << " , Y = " << m_cam_reference3D_second.y << std::endl;
            
            
            float cross = prv.x * cur.y - cur.x * prv.y;//부호 판단
            double vec_size = sqrt((float)(prv.x * prv.x + prv.y * prv.y)) * sqrt((float)(cur.x * cur.x + cur.y * cur.y));
            
            float theta = 0.f;
            float inner_product = prv.x*cur.x + prv.y * cur.y;
            
            theta = RAD2DEG(acos((double)(inner_product)/vec_size));
            if(theta>0 && theta<180.f)
            {
                if(theta>90.f)      theta = 180.f - theta;
                
                //반대 방향 처리
                if(cross<0.f)                theta = -theta;
                
                
                m_CameraTrackControl.AddYawPitchDegree(-theta, 0);
            }
            
            //std::cout << "Yaw  = " << m_Camera.GetYawDegree() << std::endl;
            
        }
        
        
        //JumpZoom 상태를 무효화 한다.
        if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
            

        /////////////////// Update Camera //////////////////
        //Orbit center를 기준으로 카메라 위치를 갱신한다.
        UpdateCameraPositionFromOrbit();
        
        //메트릭스를 갱신한다.
        UpdateViewMatrix();
        
        return true;
    }
    return false;
}

void vbViewInfo::EndHRotateZoom()
{
    m_bNavigation = false;
    m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
    
}
///////////////////// HRotation & Zoom




///////////////////// View Direction
void vbViewInfo::SetTopView()
{
    //Backup
    m_cam_3D_backup_rotation = m_Camera.GetCameraOrientation();
    m_cam_3D_backup_orbit_cntr = m_CameraTrackControl.GetTrackCenter();
    m_cam_3D_backup_distance = m_CameraZoomControl.GetDistance();
    
    if(m_CameraAnimationControl.IsAnimationUsed())
    {
        vec3 new_pos = m_CameraTrackControl.GetTrackCenter();
        new_pos.y = m_CameraZoomControl.GetDistance();
        Quaternion new_dir = Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), vec3(0.f,1.f,0.f));
                
        InitCameraMoveAni(m_Camera.GetCameraPosition(), new_pos, m_Camera.GetCameraOrientation(), new_dir, CAMERA_ANI_3DTO2D);
    }
    else
    {
        m_Camera.SetCameraOrientation(Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), vec3(0.f,1.f,0.f)));
        UpdateCameraPositionFromOrbit();
    }
}

void vbViewInfo::ReturnTo3DView()
{
    if(m_CameraAnimationControl.IsAnimationUsed())
    {     
        InitCameraMoveAni(m_CameraTrackControl.GetTrackCenter(), m_cam_3D_backup_orbit_cntr, m_Camera.GetCameraOrientation(), m_cam_3D_backup_rotation, CAMERA_ANI_2DTO3D);
    }
    else
    {
        m_cam_navi_3d_mode = NAVIGATION_3D_NONE;
        
        m_Camera.SetCameraOrientation(m_cam_3D_backup_rotation);
        m_CameraTrackControl.SetTrackCenter(m_cam_3D_backup_orbit_cntr);
        m_CameraZoomControl.SetDistance(m_cam_3D_backup_distance);
        
        
        
        UpdateCameraPositionFromOrbit();
        UpdateViewMatrix();
    }
    

    
}
///////////////////// View Direction

///////////////////// FPS
void vbViewInfo::SetWalkingCamera(vec3 pos, Quaternion dir)
{    
    m_Camera.SetCameraPosition(pos);
    m_Camera.SetCameraOrientation(dir);
    //회전 중심을 카메라 위치로부터 구한다.
    UpdateOrbitCenterFromCamPosition();
    UpdateViewMatrix();       
     
}

void vbViewInfo::StartFPS()
{
    //Camera Position
    //vec3 pos = GetRayIntersectionWithGround(m_screen_size/2);
    m_Camera.SetCameraPosition(m_CameraTrackControl.GetTrackCenter());
    vec3 pos = m_CameraTrackControl.GetTrackCenter();
    pos.y = m_CameraFPSControl.GetEyeHeight();
    
    //Camera Direction
    vec3 projected_dir = GetDirection();
    projected_dir.y = 0.f;
    projected_dir.Normalize();
    Quaternion dir = Quaternion::CreateFromVectors(vec3(0.f,0.f,-1.f),projected_dir);
    
    SetWalkingCamera(pos, dir);
    
}

void vbViewInfo::EndFPS()
{
    //현재 위치를 회전 중심으로 한다.
    m_CameraTrackControl.SetTrackCenter(m_Camera.GetCameraPosition());
    
    //전방 아래를 향하도록 함.
    
    //처음 방향에서 현재 방향을 향하는 쿼터니언을 구함.
    vec3 dir1 = GetDirection(); 
    dir1.y = 0.; //XZ평면에 있는 수평 방향을 먼저 구함.
    dir1.Normalize();
    Quaternion q1 = Quaternion::CreateFromVectors(vec3(0.f,0.f,-1.f),dir1);
    
    //현재 방향에서 아래를 비스듬히 바라보는 쿼터니언을 구함
    vec3 d3 = dir1;
    d3. y = - 1.f;
    d3.Normalize();
    Quaternion q3 = Quaternion::CreateFromVectors(dir1,d3);
    
    //두 개의 변환을 곱하여 현재 방향을 상공에 올라가서 바라보도록 함.
    m_Camera.SetCameraOrientation(q3 *q1);
    
    //거리가 너무 작으면 멀리있게 한다.
    if(m_CameraZoomControl.GetDistance()<100.f)    m_CameraZoomControl.SetDistance(100.f);
    
    //회전 중심으로부터 카메라 위치를 구한다.
    UpdateCameraPositionFromOrbit();
    UpdateViewMatrix();    
    
}



///////////////////// Walk
void vbViewInfo::InitWalk()
{
    StartFPS();
}

void vbViewInfo::StartWalking(ivec2 finger_pos)
{
    m_CameraAnimationControl.SetAnimating(true);
    m_bNavigation = true;
    m_DownPos_prev = finger_pos;//2 중 입력 시 원래 점을 확인하기 위해 사용한다.
    
}

bool vbViewInfo::DoWalking(ivec2 finger_pos)
{
    if (m_bNavigation)
    {
        m_DownPos_prev = finger_pos;
        return true;
    }    
    return false;
}

void vbViewInfo::EndWalking()
{
    m_CameraAnimationControl.SetAnimating(false);
    m_bNavigation = false;  
}
///////////////////////////////////////////////////
// Fixed Yaw & Pitch Rotate
void    vbViewInfo::StartFixedYawPitchRotate(ivec2 pos1)
{
    m_DownPos_prev=pos1;
    m_bNavigation = true;
}
void    vbViewInfo::StartForwardZoom(ivec2 pos1, ivec2 pos2)
{
    m_bNavigation = true;
    
    //초기화
    float base_distance = float((pos1 - pos2).size());
    
    m_CameraForwardControl.SetBaseValue(base_distance);
    m_CameraForwardControl.SetStartPosition(m_Camera.GetCameraPosition());
    m_CameraForwardControl.SetCurrentPosition(m_Camera.GetCameraPosition());
    m_CameraForwardControl.SetIncrement(abs(m_Camera.GetCameraPosition().y)/2.f);
    m_CameraForwardControl.SetForwardDir(m_Camera.GetFront());
}

void    vbViewInfo::StartElevation(ivec2 pos1)
{
    m_DownPos_prev=pos1;
    m_bNavigation = true;
 
    m_CameraForwardControl.SetBaseValue(float(pos1.y));
    m_CameraForwardControl.SetStartPosition(m_Camera.GetCameraPosition());
    m_CameraForwardControl.SetCurrentPosition(m_Camera.GetCameraPosition());
    m_CameraForwardControl.SetIncrement(m_Camera.GetCameraPosition().y/400.f);
     
}

bool    vbViewInfo::DoForwardZoom(ivec2 pos1, ivec2 pos2)
{
    if (m_bNavigation)
    {
        //초기화
        float curr_distance = float((pos1 - pos2).size());
        
        //새로운 위치
        m_CameraForwardControl.MoveForward(curr_distance);
        
        UpdateCameraTargetPosition();
        UpdateNearFarClip();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

void    vbViewInfo::EndForwardZoom()
{
    m_bNavigation = false;
}


bool    vbViewInfo::DoFixedYawRotate(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        ivec2 diff = m_DownPos_prev - touchpoint;
        
        m_CameraTrackControl.AddYawPitchDegree(float(diff.x)/4.f,0.f);
        m_DownPos_prev = touchpoint;
        
        UpdateCameraTargetPosition();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

bool    vbViewInfo::DoFixedPitchRotate(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        ivec2 diff = m_DownPos_prev - touchpoint;

        m_CameraTrackControl.AddYawPitchDegree(0.f,float(diff.y)/4.f);
        m_DownPos_prev = touchpoint;
        
        UpdateCameraTargetPosition();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

bool    vbViewInfo::DoFixedYawPitchRotate(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        ivec2 diff = m_DownPos_prev - touchpoint;
        
        m_CameraTrackControl.AddYawPitchDegree(float(diff.x)/4.f,float(diff.y)/4.f);
        m_DownPos_prev = touchpoint;
        
        UpdateCameraTargetPosition();
        UpdateViewMatrix();
        return true;
    }
    return false;
}

bool    vbViewInfo::DoElevation(ivec2 touchpoint)
{
    if (m_bNavigation)
    {
        //새로운 위치
        m_CameraForwardControl.MoveVertical(float(touchpoint.y));
        
        UpdateCameraTargetPosition();
        UpdateNearFarClip();
        UpdateViewMatrix();
        return true;
    }
    return false;
 
}

void    vbViewInfo::EndFixedYawPitchRotate()
{
    
}



/////////////////////////////////////////////////
void vbViewInfo::SetOrbitCenter(vec3 orbit_center)
{
    m_CameraTrackControl.SetTrackCenter(orbit_center);
}

vec3 vbViewInfo::GetOrbitCenter()
{
    return m_CameraTrackControl.GetTrackCenter();
}


/////////////////////////////////////////////////
float   vbViewInfo::GetPreviousZoomDistance()
{
    return m_CameraZoomControl.GetBaseDistance();
}
void    vbViewInfo::SetPreviousZoomDistance(float prevDist)
{
    m_CameraZoomControl.SetBaseDistance(prevDist);
}

float   vbViewInfo::GetTrackBallRatio()
{
    return m_CameraFreeControl.GetVBallRadius();
}
void    vbViewInfo::SetTrackBallRatio(float trRatio)
{
    m_CameraFreeControl.SetVBallRadius(trRatio);
}
