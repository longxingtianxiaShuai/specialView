
#include "vbViewInfo.hpp"

#include <cmath>
#include <float.h>

#include <iostream>


//For manipulation
void vbViewInfo::UpdateCameraPositionFromOrbit()
{
    //update camera position
    vec3    init_cam_pos = VEC3_UNITZ * m_CameraZoomControl.GetDistance();
    vec3    cam_position = m_CameraTrackControl.GetTrackCenter()+m_Camera.GetCameraOrientation() * init_cam_pos;
    m_Camera.SetCameraPosition(cam_position);
    
    UpdateNearFarClip();
}

void vbViewInfo::UpdateCameraTargetPosition()
{
    //Yaw&Pitch는 갱신되어 있고, 현 상태로 목표 지점을 입력함.
    vec3 dir = m_Camera.GetFront() * 1000.f;
    vec3 pos = m_Camera.GetCameraPosition();
    m_Camera.SetCameraLookAt(pos+dir);
}

//For FPS
void vbViewInfo::UpdateOrbitCenterFromCamPosition()
{
    //update camera position
    vec3    init_center_pos = VEC3_UNITZ * m_CameraZoomControl.GetDistance() * (-1.f);
    vec3    cam_orbit_cntr = m_Camera.GetCameraPosition() + m_Camera.GetCameraOrientation() * init_center_pos;
    m_CameraTrackControl.SetTrackCenter(cam_orbit_cntr);
    
    UpdateNearFarClip();
}

void vbViewInfo::InitOrientation(bool bUpdateViewMat/*true*/)
{
    //Camera 방향을 -Z를 향하게 하고  fitting을 하면 된다.
    m_Camera.SetCameraOrientation(m_pConf->qCameraQuaternion);
    if(bUpdateViewMat)  UpdateViewMatrix();

}

Quaternion vbViewInfo::GetOrientation()
{
    return m_Camera.GetCameraOrientation();
}

vec3 vbViewInfo::GetDirection()
{
    return m_Camera.GetFront();
}

vec3 vbViewInfo::GetRight()
{
    return m_Camera.GetRight();
}

vec3 vbViewInfo::GetUp()
{
    return m_Camera.GetUp();
}

void vbViewInfo::FitAABBtoScreen(bool bBoundMinMaxDist/*false*/)
{
    //반드시 BB가 먼저 설정되어 있어야 한다. 즉, 설정 된 BB에 대한 연산이다.
    vec3 tmp = m_bound_min;
    vector<vec3> vertices;
    vertices.push_back(m_bound_min);
    vertices.push_back(m_bound_max);
    tmp.x = m_bound_max.x;  vertices.push_back(tmp);
    tmp.y = m_bound_max.y;  vertices.push_back(tmp);
    tmp.x = m_bound_min.x;  vertices.push_back(tmp);
    tmp.z = m_bound_max.z;  vertices.push_back(tmp);
    tmp.y = m_bound_min.y;  vertices.push_back(tmp);
    tmp.x = m_bound_max.x;  vertices.push_back(tmp);
    

    m_ground_plane.SetPlane(vbPlane::PLN_P_ZX, 0);//m_bound_min.y);
    
    
    
    FullExtendVertexVector(vertices,bBoundMinMaxDist);
    
    UpdateViewMatrix();    
}

void vbViewInfo::FitAABBtoVertices(std::vector<vec3>&	target_vtx, bool bBoundMinMaxDist/*false*/)
{
    m_ground_plane.SetPlane(vbPlane::PLN_P_ZX, 0.f);
   FullExtendVertexVector(target_vtx,bBoundMinMaxDist);
    
    UpdateViewMatrix();
}

//Target으로 넘겨진 데이터들을 Fitting 할 때 사용
void vbViewInfo::FullExtendVertexVector(std::vector<vec3>&	target_vtx, bool bBoundMinMaxDist/*false*/)
{
	float		fitting_ratio = 1.f;		//향후, 외부 설정 파일 등으로 관리 하도록 한다.
    
	if(m_projection_mode==PROJECTION_PERSPECTIVE)
	{//Perspective
        const float*    perspective_param = m_CameraPerspective.GetPerspectiveParam();
        
		//Input
		vec3		cam_dir	= GetDirection();			//카메라 방향
		vec3		cam_up	= GetUp();					//카메라 Up-Vector
		float	fov_y_radian= DEG2RAD(perspective_param[0]);		//카메라의 세로 방향 시야각
		float	fov_x_radian= fov_y_radian * perspective_param[1];// 카메라의 가로 방향 시야각 - AspectRatio = width / height; 
		float	fit_ratio	= fitting_ratio;
        
		//Output
		vec3	new_cam_pos;
        vec3    new_orbit_center;
		bool	fit_vertical;
        float   dist;
        
		//Calc new camera position
		bool result =   GetPerspectiveFittingCameraPosition(cam_dir, cam_up, fov_y_radian, fov_x_radian, target_vtx, fit_ratio,	//INPUT
                                                            new_cam_pos, new_orbit_center, fit_vertical, dist, bBoundMinMaxDist);					//OUTPUT
		if(!result)	return;
        
		//Apply new camera position
        SetCameraPosition(new_cam_pos);
        SetCameraDistance(dist,bBoundMinMaxDist);
        //SetOrbitCenter(new_orbit_center); //회전 중심은 화면 중앙과 바닥 평면과의 교점으로 한다.
        
 	}
	else
	{//Orthogonal
        
		//Input
		vec3		cam_dir	= GetDirection();				//카메라 방향
		vec3		cam_up	= GetUp();						//카메라 Up-Vector
		float	fit_ratio	= fitting_ratio;
		float	cam_distance= 0.f;							//ArchiSpace에서는 직교에서도 거리에 따라 원근감의 모의하므로 이 기준에 맞게 거리를 적용한다.
        ivec2   scr_size = m_CameraOrthogonal.GetScreenSize();
		float	view_aspect_ratio = (float)scr_size.x/(float)scr_size.y; // Width/Height
        
		//Output
		vec3		new_cam_pos;
		float		view_width;
		float		view_height;
		bool		fit_vertical;
        
		//Orthogonal mode에서 Fitting
		bool result =   GetOrthogonalFittingCameraPosition(cam_dir, cam_up,target_vtx, fit_ratio,cam_distance,view_aspect_ratio,	//INPUT
                                                           new_cam_pos,view_width,view_height,fit_vertical);						//OUTPUT
        
		if(!result)	return;
        
		//Apply new camera position
		//카메라의 위치는 Orbit에서 거리에 따른 배율을 감안하여, Fitting에 적용 된 배율에 맞는 거리로 지정한다.
		cam_distance = view_height* float(-1.11); //1.11 = MagicNumber : NaviCamera의 mRateOrtho값이 변하면 이 값도 그에 맞게 변해야 한다.
        
        if(bBoundMinMaxDist)
        {
            if(fabs(cam_distance)<m_CameraZoomControl.GetDistanceMin()) cam_distance = cam_distance / fabs(cam_distance) * m_CameraZoomControl.GetDistanceMin();
            else if(fabs(cam_distance)>m_CameraZoomControl.GetDistanceMax()) cam_distance = cam_distance / fabs(cam_distance) * m_CameraZoomControl.GetDistanceMax();
        }
        
		SetCameraPosition(new_cam_pos+GetDirection()*cam_distance);//이 위치에서 뒤로 cam_distance만큼 간다.(뒤로 가기 위해 distance에 -가 곱해졌다.)
     }
	
    UpdateCameraDistWithScreenCenter(bBoundMinMaxDist);
    UpdateNearFarClip();  
}

void vbViewInfo::UpdateCameraDistWithScreenCenter(bool bBoundMinMaxDist/*false*/)
{
    ivec2 scr_size = m_CameraPerspective.GetScreenSize();
    vec3 cam_orbit_cntr = GetRayIntersectionWithGround(scr_size/2);
    m_CameraTrackControl.SetTrackCenter(cam_orbit_cntr);
    
    float cam_distance = (m_CameraTrackControl.GetTrackCenter() - m_Camera.GetCameraPosition()).size();
    
    if(bBoundMinMaxDist)
    {
        vec3 dir_from_cntr_to_cam = (m_Camera.GetCameraPosition() - cam_orbit_cntr).Normalized();
        
        if(cam_distance < m_CameraZoomControl.GetDistanceMin())         cam_distance = m_CameraZoomControl.GetDistanceMin();
        else if(cam_distance > m_CameraZoomControl.GetDistanceMax())    cam_distance = m_CameraZoomControl.GetDistanceMax();
        
        //카메라 위치 조정
        vec3 new_cam = cam_orbit_cntr + dir_from_cntr_to_cam * cam_distance;
        m_Camera.SetCameraPosition(new_cam);
    }
    
    m_CameraZoomControl.SetDistance(cam_distance);
        
    //메트릭스를 갱신한다.
    UpdateViewMatrix();
   
}

//Perspective mode에서 Fitting
bool vbViewInfo::GetPerspectiveFittingCameraPosition(vec3	inputCameraDirection,/*대상을 바라보고 있을 카메라의 방향. 보통 현재 카메라의 방향*/
                                                     vec3	inputCameraUp,/*대상을 바라보고 있을 카메라의 Upvector.*/
                                                    float				inputCameraFOV_Y_radian,/*카메라의 세로 방향 시야각*/
                                                    float				inputCameraFOV_X_radian,/*카메라의 가로 방향 시야각*/
                                                    std::vector<vec3>&	inputVts,/*Fitting 대상이 되는 점들*/
                                                    float				inputFit_ratio,/* 1 : 화면에 꼭 맞게, 0.5 : 화면의 절반 크기에 맞게, 0.01 : 최소값*/
                                                    vec3&               outputCameraPosition,/*카메라의 위치*/
                                                    vec3&               outputOrbitCenter,//New orbit center
                                                    bool&               outVerticalFit,/*세로방향으로 맞췄는지 여부*/   
                                                    float&              outputDistance,
                                                     bool            bBoundMinMaxDist/*false*/)//

{
	//Fit ratio는 0.01로 제한한다. 특별한 이유는 없지만, 너무 작게 맞추는 것은 의미가 없다. 
	//더 작게 맞출 필요가 있을 때 함수를 수정하기 바란다.
	if(inputFit_ratio<0.01f)
	{
		//cout << "Too small ratio for PerspectiveFitting\r\n";
		return false;
	}
    
	//대상 점들이 없는 경우
	if(inputVts.size()==0)
	{
		//AfxMessageBox("No vertices");
		return false;
	}
    
	//대상 점들의 중심 좌표를 구함.
	vec3		center(0.f,0.f,0.f);	//대상 점들의 중심
	for(std::vector<vec3>::iterator vtx_iterator = inputVts.begin();vtx_iterator!=inputVts.end();vtx_iterator++)
	{	center += (*vtx_iterator);	}
	center /= float(inputVts.size());
    
    //새로운 회전 중심
    outputOrbitCenter = center;
    
    
	//Bounding Box를 원점에서 +X를 바라보고, UpVector는 +Y를 향하도록 하는 변환    
	Quaternion quat_cam_dir_as_px   = Quaternion::CreateFromVectors(inputCameraDirection, VEC3_UNITX);  //현재 카메라 방향을 X축에 맞추는 변환
    vec3 upvector                   = quat_cam_dir_as_px * inputCameraUp;                               //x축 변환이 된 upvector
	Quaternion quat_cam_up_as_py    = Quaternion::CreateFromVectors(upvector, VEC3_UNITY);              //카메라 방향이 X축일때 UpVector를 맞추는 변환
 	Quaternion quat_cam_to_set		= quat_cam_up_as_py * quat_cam_dir_as_px;                           //두 변환이 적용 된 변환
   
    
	//XY, XZ평면에 투영 된 점들을 이용하여 카메라의 기준 좌표를 구함.
	/*
     Viewing Frustum을 XY평면에 투영했을 때 만들어지는 두 개의 외접 직선을 구함.
     Y = Axy X + B1 과 Y = -Axy X + B2
     Viewing Frustum을 XZ평면에 투영했을 때 만들어지는 두 개의 외접 직선을 구함.
     Z = Axz X + B1 과 Z = -Axz X + B2
     */
	double	Axy = tan(inputCameraFOV_Y_radian/2.0);     //XY에 투영 된 직선의 기울기. 일단, 정확한 값을 구하기 위해 Table 없이 구함.
	double	Bpositive_xy = FLT_MIN;						//XY에 투영 된, 기울기가 양인 외접 직선의 Y절편, 최대값을 구해야 하므로 최소값으로 초기화 함.
	double	Bnegative_xy = FLT_MAX;						//XY에 투영 된, 기울기가 음인 외접 직선의 Y절편, 최소값을 구해야 하므로 최대값으로 초기화 함.
	double	Axz = tan(inputCameraFOV_X_radian/2.0);    //XZ에 투영 된 직선의 기울기. 일단, 정확한 값을 구하기 위해 Table 없이 구함.(좌우폭은 FOV계산과 달라서 튜닝함.)
	double	Bpositive_xz = FLT_MIN;						//XZ에 투영 된, 기울기가 양인 외접 직선의 Z절편, 최대값을 구해야 하므로 최소값으로 초기화 함.
	double	Bnegative_xz = FLT_MAX;						//XZ에 투영 된, 기울기가 음인 외접 직선의 Z절편, 최소값을 구해야 하므로 최대값으로 초기화 함.
	double	tmpB;
	vec3	one_vt;
       
	for(std::vector<vec3>::iterator vtx_iterator = inputVts.begin();vtx_iterator!=inputVts.end();vtx_iterator++)
	{
		//Transform
		(*vtx_iterator)	-= center;							//원점 주변으로 이동
		(*vtx_iterator)	= quat_cam_to_set * (*vtx_iterator);//점을 기준 축으로 변환
        
		//Projection on to XY
		//Y = AX + B	--> B = Y - AX
		tmpB	=	(*vtx_iterator).y - Axy * (*vtx_iterator).x;
		if(tmpB>Bpositive_xy)	Bpositive_xy = tmpB;		//XY에 투영 된 양의 기울기 직선의 최대 Y절편을 구한다.
		//Y = -AX + B	--> B = Y + AX
		tmpB	=	(*vtx_iterator).y + Axy * (*vtx_iterator).x;
		if(tmpB<Bnegative_xy)	Bnegative_xy = tmpB;		//XY에 투영 된 음의 기울기 직선의 최소 Y절편을 구한다.
        
		//Projection on to XZ
		//Z = AX + B	--> B = Z - AX
		tmpB	=	(*vtx_iterator).z - Axz * (*vtx_iterator).x;
		if(tmpB>Bpositive_xz)	Bpositive_xz = tmpB;		//XZ에 투영 된 양의 기울기 직선의 최대 Z절편을 구한다.
		//Z = -AX + B	--> B = Z + AX
		tmpB	=	(*vtx_iterator).z + Axz * (*vtx_iterator).x;
		if(tmpB<Bnegative_xz)	Bnegative_xz = tmpB;		//XZ에 투영 된 음의 기울기 직선의 최소 Z절편을 구한다.
	}
    
	//FOV가 너무 작은 경우
	if(Axz<0.001 || Axz<0.001)
	{
		return false;
        //	AfxMessageBox("Too small FOV");
	}
    
	//두 경우 중, X값이 더 작은 경우가 종횡비를 고려하여 꼭 맞는 카메라의 위치가 된다.
	float	CX_xy = float((Bnegative_xy-Bpositive_xy) / ( 2. * Axy));	//X = (B2-B1)/2A
	float	CY_xy = float((Bnegative_xy+Bpositive_xy) / 2.);			//Y = (B2+B1)/2
	float	CX_xz = float((Bnegative_xz-Bpositive_xz) / ( 2. * Axz));	//X = (B2-B1)/2A
	float	CZ_xz = float((Bnegative_xz+Bpositive_xz) / 2.);			//Z = (B2+B1)/2
	vec3 cam_pos;
	if(CX_xy<CX_xz)//XY평면 투영 기준(화면상에서 상하를 맞춤)
	{
		cam_pos.x =	CX_xy /inputFit_ratio;	//배율은 X좌표에 의해서 정해짐.
		cam_pos.y =	CY_xy;
        
		cam_pos.z = CZ_xz;
        
		outVerticalFit = true;
	}
	else//XZ평면 투영 기준(화면상에서 좌우를 맞춤)
	{
		cam_pos.x =	CX_xz /inputFit_ratio;	//배율은 X좌표에 의해서 정해짐.;
		cam_pos.z =	CZ_xz;
        
		cam_pos.y = CY_xy;
        
		outVerticalFit = false;
	}
    
	//새로 구한 위치를 원래 방향으로 역변환을 한다.
	outputCameraPosition = quat_cam_to_set.Inverse() * cam_pos;
	outputCameraPosition +=center;
    
    outputDistance      = outputCameraPosition.size();
    outputOrbitCenter   = outputCameraPosition + GetDirection() * outputDistance;
    
    if(bBoundMinMaxDist)
    {
        if(outputDistance<m_CameraZoomControl.GetDistanceMin())         outputDistance = m_CameraZoomControl.GetDistanceMin();
        else if(outputDistance>m_CameraZoomControl.GetDistanceMax())    outputDistance = m_CameraZoomControl.GetDistanceMax();
        
        outputCameraPosition = outputOrbitCenter - GetDirection() * outputDistance;
    }
    
	return true;
}

//Orthogonal mode에서 Fitting
bool vbViewInfo::GetOrthogonalFittingCameraPosition(vec3				inputCameraDirection/*대상을 바라보고 있을 카메라의 방향. 보통 현재 카메라의 방향*/,
                                                     vec3					inputCameraUp/*대상을 바라보고 있을 카메라의 Upvector.*/,
                                                     std::vector<vec3>&	inputVts/*Fitting 대상이 되는 점들*/,
                                                     float					inputFit_ratio/* 1 : 화면에 꼭 맞게, 0.5 : 화면의 절반 크기에 맞게, 0.01 : 최소값*/,
                                                     float					inputDistance_for_ortho/*직교투영은 원근감이 없으므로, 거리에 상관없이 동일한 화면이 나오기 때문에 거리를 지정해야 한다*/,
                                                     float					inputViewAspectRatio/* Width/height*/,
                                                     vec3&	outputCameraPosition/*카메라의 위치*/,
                                                     float&		outputCamWidth/*카메라의 Orthogonal Width*/,
                                                     float&		outputCamHeight/*카메라의 Orthogonal Height*/,
                                                     bool&		outVerticalFit/*세로방향으로 맞췄는지 여부*/
                                                     )
{
	//Fit ratio는 0.01로 제한한다. 특별한 이유는 없지만, 너무 작게 맞추는 것은 의미가 없다. 
	//더 작게 맞출 필요가 있을 때 함수를 수정하기 바란다.
	if(inputFit_ratio<0.01f)	return false;
    
	//대상 점들이 없는 경우
	if(inputVts.size()==0)
	{
		//AfxMessageBox("No vertices");
		return false;
	}
    
	//대상 점들의 중심 좌표를 구함.
	vec3		center(0.f,0.f,0.f);	//대상 점들의 중심
	for(std::vector<vec3>::iterator vtx_iterator = inputVts.begin();vtx_iterator!=inputVts.end();vtx_iterator++)
	{	center += (*vtx_iterator);	}
	center /= float(inputVts.size());
    
	//Bounding Box를 원점에서 +X를 바라보고, UpVector는 +Y를 향하도록 하는 변환
	mat4	mtxTrans_obj_to_org;
	mtxTrans_obj_to_org.Translate(-center.x,-center.y,-center.z);	//바운딩 박스를 원점으로 옮기는 이동 변환	
    
	Quaternion quat_cam_dir_as_px =Quaternion::CreateFromVectors(inputCameraDirection, VEC3_UNITX);  //현재 카메라 방향을 X축에 맞추는 변환
    vec3 upvector = quat_cam_dir_as_px * inputCameraUp;  //x축 변환이 된 upvector
	Quaternion quat_cam_up_as_py = Quaternion::CreateFromVectors(upvector, VEC3_UNITY);             //카메라 방향이 X축일때 UpVector를 맞추는 변환
 	Quaternion quat_cam_to_set		= quat_cam_up_as_py * quat_cam_dir_as_px;						//두 변환이 적용 된 변환
   
	//대상 점들을 표준 위치로 변환하며, 이때 YZ 평면의 Bounding Rect를 구한다.
	vec3	one_vt;
	double	min_y = FLT_MAX;	double	max_y = FLT_MIN;		double	min_z = FLT_MAX;		double	max_z = FLT_MIN;
	for(std::vector<vec3>::iterator vtx_iterator = inputVts.begin();vtx_iterator!=inputVts.end();vtx_iterator++)
	{
		//Transform
		(*vtx_iterator)	-= center;							//원점 주변으로 이동
		(*vtx_iterator)	= quat_cam_to_set * (*vtx_iterator);//점을 기준 축으로 변환
        
		//Calculation Bounding Rect
		if(min_y>(*vtx_iterator).y)	min_y = (*vtx_iterator).y;	//MIN Y
		if(max_y<(*vtx_iterator).y)	max_y = (*vtx_iterator).y;	//MAX Y
		if(min_z>(*vtx_iterator).z)	min_z = (*vtx_iterator).z;	//MIN Z
		if(max_z<(*vtx_iterator).z)	max_z = (*vtx_iterator).z;	//MAX Z
	}
    
	//종횡비를 고려하여 Orthoview를 설정한다.
	float  object_aspect_ratio	= float((max_z-min_z) / (max_y-min_y));
	float  ortho_height;
	float  ortho_width;
	if(inputViewAspectRatio>object_aspect_ratio)//오브젝트를 세로로 맞추는 경우
	{
		ortho_height	= float(max_y-min_y);
		ortho_width		= ortho_height * inputViewAspectRatio;
        
		outVerticalFit	= true;
	}
	else
	{//오브젝트의 가로를 맞추는 경우 -
		ortho_width		= float(max_z-min_z);
		ortho_height	= ortho_width / inputViewAspectRatio;
        
		outVerticalFit	= false;
	}
    
	vec3 calced_pos;
	calced_pos.x = float(-inputDistance_for_ortho);	
	calced_pos.y = float((min_y+max_y)/2.);
	calced_pos.z = float((min_z+max_z)/2.);
    
    
	//새로 구한 위치를 원래 방향으로 역변환을 한다.
	outputCameraPosition = quat_cam_to_set.Inverse() * calced_pos;
	outputCameraPosition +=center;
	//카메라의 가로/세로 크기
	outputCamWidth	=	ortho_width / inputFit_ratio;		//출력 결과가 inputFit_ratio만큼 더 작게 만들어져야 한다.
	outputCamHeight	=	ortho_height / inputFit_ratio;
    
	return true;
}


//Environmental map
void    vbViewInfo::SetEnvironmentalCamera()
{
    //CAMERA
    m_Camera.SetCameraPosition(vec3(0.f,0.f,0.f));              //camera position
    m_Camera.SetCameraOrientation(Quaternion(0.f,-1.f,0.f,0.f));
    m_Camera.SetPitchDegree(0.f);
    m_Camera.SetYawDegree(0.f);
 
    //ZOOM CONTROL
    m_CameraZoomControl.SetDistance(0.f);                    //camera rotation radius
    m_CameraZoomControl.SetDistanceMin(0.f);
    m_CameraZoomControl.SetDistanceMax(500.f);
    
    //ROTATION CONTROL
    m_CameraTrackControl.SetTrackCenter(vec3(0.f,0.f,-100.f));  //camera rotation center
//    m_CameraTrackControl.SetPitchDegree(0.f);
//    m_CameraTrackControl.SetYawDegree(0.f);
    m_CameraTrackControl.SetPitchMinDegree(-60.f);
    m_CameraTrackControl.SetPitchMaxDegree(60.f);
    
    m_cam_bJumpZoomed = false;
    m_bNavigation = false;
    
    UpdateViewMatrix();
    
}

//0(Min) - 1(Max)
void    vbViewInfo::SetCameraPitchRatio(float pitch_ratio)
{
    m_CameraTrackControl.SetCameraPitchRatio(pitch_ratio);

    
    //JumpZoom 상태를 무효화 한다.
    if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
    
    //파라미터 갱신
    UpdateCameraPositionFromOrbit();
    UpdateViewMatrix();
}

float   vbViewInfo::GetPitchRatio()
{
    return m_CameraTrackControl.GetPitchRatio();
}


float   vbViewInfo::GetPitchDegree()
{
    //return m_CameraTrackControl.GetPitchDegree();
    return m_Camera.GetPitchDegree();
}

float   vbViewInfo::GetYawDegree()
{
    //return m_CameraTrackControl.GetYawDegree();
    return m_Camera.GetYawDegree();
}
void    vbViewInfo::SetYawDegree(float fYaw)
{
    //m_CameraTrackControl.SetYawDegree(fYaw);
    m_Camera.SetYawDegree(fYaw);
}
void    vbViewInfo::SetYawDegree(vec3 dirYaw)
{
    //m_CameraTrackControl.SetYawDegree(dirYaw);
    m_Camera.SetYawDegree(dirYaw);
}

void    vbViewInfo::SetCamera(float cx, float cy, float cz,
                              float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,
                              float fDist)
{
#ifdef VB_CORE_DEBUG
   // printf("[DEBUG]SetCameraYaw = %f\r\n",yaw_deg_from_nZ);
#endif
    
    if (yaw_deg_from_nZ< (-180.f)) {
        while (yaw_deg_from_nZ < (-180.f)) {
            yaw_deg_from_nZ += 360.f;
        }
    }
    else if(yaw_deg_from_nZ > 180.f) {
        while (yaw_deg_from_nZ > 180.f) {
            yaw_deg_from_nZ -= 360.f;
        }
    }
   
    /*
    ////////////////////////////////// 카메라 수평 성분
    //카메라 방향
    vec3 prj_dir;
    prj_dir.y = 0;
    prj_dir.x = -sin(DEG2RAD(yaw_deg_from_nZ));
    prj_dir.z = -cos(DEG2RAD(yaw_deg_from_nZ));
    prj_dir.Normalize();
    
    //수평 회전 성분
    Quaternion orbit_h = Quaternion::CreateFromVectors(VEC3_UNITZ * (-1.f),prj_dir);
    orbit_h.Normalize();
    
    //////////////////////////////////위로 올릴 쿼터니언.
    
    float pitch_angle = pitch_deg_from_xz_plane;
    if(pitch_angle < m_pConf->fPitchLimitMin)       pitch_angle = m_pConf->fPitchLimitMin;
    else if(pitch_angle > m_pConf->fPitchLimitMax)  pitch_angle = m_pConf->fPitchLimitMax;
    vec3 pitch_dir;
    pitch_dir.x = 0.f;
    pitch_dir.y = sin(DEG2RAD(pitch_angle));
    pitch_dir.z = -cos(DEG2RAD(pitch_angle));
    Quaternion orbit_v = Quaternion::CreateFromVectors(VEC3_UNITZ* (-1.f),pitch_dir);
    orbit_v.Normalize();
    
    //////////////////////////////////// 파라미터 적용
     */
    
    //std::cout << "Yaw = " << yaw_deg_from_nZ << ", Pitch = " << pitch_deg_from_xz_plane <<  std::endl;
    
    Quaternion orient = Quaternion::CreateFromNegativeZtoCWYawAndPitchDegree(yaw_deg_from_nZ, pitch_deg_from_xz_plane);
    
    m_Camera.SetCameraOrientation(orient);
    m_Camera.SetYawDegree(yaw_deg_from_nZ);
    m_Camera.SetPitchDegree(pitch_deg_from_xz_plane);
    
    m_CameraTrackControl.SetTrackCenter(vec3(cx,cy,cz));
//    m_CameraTrackControl.SetYawDegree(yaw_deg_from_nZ);
//    m_CameraTrackControl.SetPitchDegree(pitch_deg_from_xz_plane);
    
    if(fDist>0)
        m_CameraZoomControl.SetDistance(fDist);
    
    //JumpZoom 상태를 무효화 한다.
    if(m_cam_bJumpZoomed) m_cam_bJumpZoomed = false;
    
    //파라미터 갱신
    UpdateCameraPositionFromOrbit();
    UpdateViewMatrix();
}

void  vbViewInfo::SetViewTurnDirection(vbViewDirection dir, bool bAnimating/*=false*/)
{
    m_eViewDirection = dir;
    
    if(bAnimating)  InitDeviceRotateAni(dir);
    else            UpdateDeviceAngleDeg();
}

void vbViewInfo::UpdateDeviceAngleDeg()
{
    switch (m_eViewDirection)
    {
            //1-Device oriented vertically, home button on the bottom
        case vbViewOrientationPortrait:                 m_fDeviceAngle = 0.f;       break;
            //2-Device oriented vertically, home button on the top
        case vbViewOrientationPortraitUpsideDown:       m_fDeviceAngle = 180.f;     break;
            //3-Device oriented horizontally, home button on the right
        case vbViewOrientationLandscapeLeft:            m_fDeviceAngle = -90.f;     break;
            //4-Device oriented horizontally, home button on the left - Rotate
        case vbViewOrientationLandscapeRight:           m_fDeviceAngle = 90.f;      break;
        default:break;
    }
}

ivec2 vbViewInfo::ConvertToRotatedDeviceCoord(ivec2 tPos)
{
    ivec2 new_pos = tPos;
    ivec2 scr_size = m_CameraPerspective.GetScreenSize();
    
    switch (m_eViewDirection)
    {
        case vbViewOrientationPortraitUpsideDown:
        {
            new_pos.x = scr_size.x - tPos.x;
            new_pos.y = scr_size.y - tPos.y;
        }
        break;
            
        case vbViewOrientationLandscapeLeft:
        {
            new_pos.x = float(tPos.y) / float(scr_size.y) * float(scr_size.x);
            new_pos.y = float(scr_size.x - tPos.x) / float(scr_size.x) * float(scr_size.y);
        }
        break;
            
        case vbViewOrientationLandscapeRight:
        {
            new_pos.x = float(scr_size.y - tPos.y) / float(scr_size.y) * float(scr_size.x);
            new_pos.y = float(tPos.x) / float(scr_size.x) * float(scr_size.y);
        }
        break;
            
        default:break;
    }
    return new_pos;
    
}
/////////////////////////////////////////////////
void vbViewInfo::SetCameraPosition(vec3 cam_pos)
{
    m_Camera.SetCameraPosition(cam_pos);
}

vec3 vbViewInfo::GetCameraPosition()
{
    return m_Camera.GetCameraPosition();
}

//camera distance
float vbViewInfo::GetCameraDistance()
{
    return  m_CameraZoomControl.GetDistance();
}

void vbViewInfo::SetCameraDistance(float dist, bool bBoundMinMaxDistance/*false*/)
{
    if(bBoundMinMaxDistance)
    {
        float minDist = m_CameraZoomControl.GetDistanceMin();
        float maxDist = m_CameraZoomControl.GetDistanceMax();
        
        if (dist < minDist)         dist = minDist;
        else if(dist > maxDist)     dist = maxDist;
    }
    m_CameraZoomControl.SetDistance(dist);
}

void    vbViewInfo::SetCameraMinMaxDistance(float fMinDistance, float fMaxDistance, bool bBoundMinMaxDist/*false*/)
{
    m_CameraZoomControl.SetDistanceMin(fMinDistance);
    m_CameraZoomControl.SetDistanceMax(fMaxDistance);
    
    if(bBoundMinMaxDist)
    {
        float curDist = GetCameraDistance();
        if (curDist < fMinDistance)        SetCameraDistance(fMinDistance);
        else if(curDist > fMaxDistance)     SetCameraDistance(fMaxDistance);
    }
}


//camera orientation
void vbViewInfo::SetOrientation(Quaternion qtn)
{
    m_Camera.SetCameraOrientation(qtn);
}


/////////////////////////////////////////// FPS Control
float   vbViewInfo::GetEyeHeight()
{
    return m_CameraFPSControl.GetEyeHeight();
}    

void    vbViewInfo::SetEyeHeight(float eye_h)
{
    m_CameraFPSControl.SetEyeHeight(eye_h);
}
