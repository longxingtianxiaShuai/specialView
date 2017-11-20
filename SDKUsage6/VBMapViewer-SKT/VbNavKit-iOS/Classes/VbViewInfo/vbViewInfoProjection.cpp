#include "vbViewInfo.hpp"

#include <iostream>

void vbViewInfo::SetPerspectiveParameters(float* perspective_param)
{
    m_CameraPerspective.SetPerspectiveParam((const float*)perspective_param);
}

void vbViewInfo::SetPerspectiveParameters(float fovy, float aspect, float near, float far)
{
    m_CameraPerspective.SetPerspectiveParam(fovy, aspect, near, far);
}

const float*  vbViewInfo::GetPerspectiveParameters()
{
    return m_CameraPerspective.GetPerspectiveParam();
}



//DoPanning,DoZooming,FullExtendVertexVector
void vbViewInfo::UpdateNearFarClip()
{
    float perspective_param[4];
    memcpy(perspective_param, m_CameraPerspective.GetPerspectiveParam(), sizeof(float)*4);
           
    if(m_cam_navi_mode==vbViewInfo::NAVIGATION_ENVRN)
    {//Environment map
        perspective_param[2]= 10.f;  //Near
        perspective_param[3]= 1000000.f; //Far
    }
    else
    {
        switch (m_eViewControlSet)
        {
            case vbViewControlSetGongVueTypeA:
            {
                //near값은 카메라의 고도에서 바운딩 박스의 높이값을 뺀다.
                vec3 camera_pos = m_Camera.GetCameraPosition();
                float dist_min = (camera_pos-m_bound_min).size();
                float dist_max = (camera_pos-m_bound_max).size();
                
                if(camera_pos.y > m_bound_max.y)
                    perspective_param[2]= (camera_pos.y - (m_bound_max.y - m_bound_min.y))*0.9f;
                else if(camera_pos.y > 150.f)
                    perspective_param[2]= (camera_pos.y - 150.f)*0.9f;
                else
                    perspective_param[2] = 100.f;
                    
                if(dist_max>dist_min)   perspective_param[3]= dist_max * 1.1f;
                else                    perspective_param[3]= dist_min * 1.1f;
                
            }
            break;
                
            default:
            {
                //Orbit 모드에서는
                float   margin_rad = m_rad_bounding_sphere * 1.5f;
                float   dist = (m_bounding_center - m_Camera.GetCameraPosition()).size(); //cam_distance를 갱신하는 것은 조정이 끝난 후.
                
                if (dist>margin_rad)
                {//카메라가 Bounding Sphere 외부에 있는 경우
                    perspective_param[2]= dist - margin_rad; //Near
                    perspective_param[3]= dist + margin_rad; //Far
                }
                else
                {//카메라가 Bounding Sphere 내부에 있는 경우
                    perspective_param[2]= m_min_near_clip;  //Near
                    perspective_param[3]= m_rad_bounding_sphere * 3.f; //Far
                }
            }
            break;
        }

    }
    m_CameraPerspective.SetPerspectiveParam((const float*)perspective_param);


    
    UpdateProjectionMatrix();
}

//Orthogonal
void vbViewInfo::SetOrthogonalParameters(float left, float right, float bottom, float top, float near, float far)
{
    m_CameraOrthogonal.SetOrthogonalParam(left, right, bottom, top, near, far);
}

const float*  vbViewInfo::GetOrthogonalParameters()
{
    return m_CameraOrthogonal.GetOrthogonalParam();
}

void vbViewInfo::UpdateProjectionMatrix()
{
    switch (m_projection_mode)
    {
        case PROJECTION_ORTHO:
        {
            
        }
        break;
            
        case PROJECTION_PERSPECTIVE:
        default:
        {
            
                                            
            if (m_cam_navi_mode==NAVIGATION_ENVRN)
            {//광각
                const float*  perspective_param= m_CameraPerspective.GetPerspectiveParam();
                m_prj_matrix = mat4::Perspective(perspective_param[0]*2.f,//fovy
                                                 perspective_param[1],//aspect
                                                 perspective_param[2],//near
                                                 perspective_param[3]);//far
            }
            else
            {//일반뷰
                m_prj_matrix = m_CameraPerspective.GetProjectionMatrix();
            }
           
        }
        break;
    }
    m_view_prj_matrix = m_view_matrix * m_prj_matrix;
    m_bViewUpdated = true;

}

void vbViewInfo::UpdateViewMatrix()
{
    if(m_cam_navi_mode!=NAVIGATION_ENVRN)
    {
        vec3 cam_pos = m_Camera.GetCameraPosition();
        vec3 cam_target = m_Camera.GetCameraLookAt();
   
       
        mat4 rotation = mat4::LookAt(cam_pos,cam_target,GetUp());
        mat4 translate= mat4::Translate(-cam_pos.x, -cam_pos.y, -cam_pos.z);
        m_view_matrix = translate * rotation;
   
         /*       
        std::cout << "[CAMERA] Pitch = " << m_Camera.GetPitchDegree() << ", Yaw = " << m_Camera.GetYawDegree() << ", POS =(" << cam_pos.x << ", " << cam_pos.y << ", " << cam_pos.z << "\n";
        

        mat4 pitch_rotation = mat4::RotateX(-m_Camera.GetPitchDegree());
        mat4 yaw_rotation = mat4::RotateY(-m_Camera.GetYawDegree());
        mat4 offset_trans = mat4::Translate(-cam_pos.x, -cam_pos.y, -cam_pos.z);
        //mat4 offset_trans = mat4::Translate(0, -4000.f, 0);
        m_view_matrix = offset_trans * yaw_rotation * pitch_rotation;
       // m_view_matrix = pitch_rotation * yaw_rotation * offset_trans;
         */
    }
    else
    {
        vec3 org(0.f,0.f,0.f);
        vec3 up(0.f,1.f,0.f);
        mat4 roll_rotation_Z = mat4::RotateZ(-90.f);
        mat4 pitch_rotation = mat4::RotateX(m_Camera.GetPitchDegree());
        mat4 yaw_rotation = mat4::RotateY(m_Camera.GetYawDegree());
        mat4 offset_trans = mat4::Translate(0.f, 0.f, m_CameraZoomControl.GetDistance());
        m_view_matrix = yaw_rotation * pitch_rotation * roll_rotation_Z * offset_trans;
    }
    
    m_view_prj_matrix = m_view_matrix * m_prj_matrix;
    m_bViewUpdated = true;
}

ivec2 vbViewInfo::WorldToWindow(vec3  world_pt)
{
    ivec2 win_pos;
            
    vec3  projected_3d = (m_view_matrix * m_prj_matrix) * world_pt;

#ifdef VB_CORE_DEBUG
    std::cout << "Projected PT = " << projected_3d.x << ", " << projected_3d.y << ", " << projected_3d.z <<  "\r\n";
#endif
    
    ivec2 scr_size = m_CameraPerspective.GetScreenSize();
    float x,y,z;
    x = scr_size.x * (projected_3d.x + 1.f)/2.f;
    y = scr_size.y - scr_size.y * (projected_3d.y + 1.f)/2.f;
    z = (projected_3d.z + 1.f)/2.f;
     
    //Convert from Homogeneous coordinate
    win_pos.x = (int)(x/z);
    win_pos.y = (int)(y/z);
    
    return win_pos;
}

void vbViewInfo::WindowToWorld(ivec2 win_pos, vec3& near_pt, vec3& far_pt)
{
    
}

