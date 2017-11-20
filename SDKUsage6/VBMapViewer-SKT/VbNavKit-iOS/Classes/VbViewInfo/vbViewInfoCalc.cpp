
#include "vbViewInfo.hpp"


vec3 vbViewInfo::MapToSphere(ivec2 touchpoint)
{
    //가로 보기 등을 할 경우 수정 해야 할 수도 있다.
    ivec2   scr_size = m_CameraPerspective.GetScreenSize();
   
    vec2 p = touchpoint - scr_size / 2;
    
    // Flip the Y axis because pixel coords increase towards the bottom.
    p.y = -p.y;
    
    const float radius = m_CameraFreeControl.GetVBallRadius();
    const float safeRadius = radius - 1;
    
    if (p.Length() > safeRadius) {
        float theta = atan2(p.y, p.x);
        p.x = safeRadius * cos(theta);
        p.y = safeRadius * sin(theta);
    }
    
    float z = sqrt(radius * radius - p.LengthSquared());
    vec3 mapped = vec3(p.x, p.y, z);
    return mapped / radius;
}

vec3 vbViewInfo::GetRayFromCamera(ivec2 win_pos, vec3& fromPt)
{
    //장비 방향에 따라서 계산이 달라진다.
    vec3    ray;
    
    if(m_projection_mode==PROJECTION_PERSPECTIVE)
    {//카메라에서 나가는 벡터의 방향을 반환한다.
        
        ivec2   scr_size = m_CameraPerspective.GetScreenSize();
       const float*    perspective_param = m_CameraPerspective.GetPerspectiveParam();
        
        //vertical
        double v_std_dist = ((double)scr_size.y/2.0) / tan(DEG2RAD((double)perspective_param[0])/2.0);  //m_perspective_param[0] = fovy
        double vAngle = atan2f(((double)scr_size.y/2.0) - (double)win_pos.y, v_std_dist);
        
        //horizontal
        double h_std_dist = ((double)scr_size.x/2.0) / tan(DEG2RAD(perspective_param[0])*perspective_param[1]/2.0);  //m_perspective_param[1] = aspect
        double hAngle = atan2f((double)win_pos.x - ((double)scr_size.x/2.0), h_std_dist);
        
        //3D Vector
        ray.x = sinf(hAngle);
        ray.z = -cosf(hAngle);
        ray.y = tanf(vAngle);
        ray.Normalize();
        
        //From camera
        fromPt = m_Camera.GetCameraPosition();
        
        return m_Camera.GetCameraOrientation() * ray;
    }
    else// if(m_projection_mode==PROJECTION_ORTHO)
    {//현재 카메라의 위치에서 옵셋을 구한다. 방향은 카메라의 방향과 동일하다.
        ivec2   scr_size = m_CameraOrthogonal.GetScreenSize();
        
        //-1~1사이로 조정한다.
        double h_ratio = (double)(win_pos.x-scr_size.x/2.0) / (double)(scr_size.x/2.0);
        double v_ratio = (double)(scr_size.y/2.0-win_pos.y) / (double)(scr_size.y/2.0);
        
        //float   m_orthogonal_param[6];      //[0] - left,   [1] - right,    [2] - bottom
        //OrthoView의 좌표를 곱해서 공간에서의 위치를 구한다.
        const float* ortho_param = m_CameraOrthogonal.GetOrthogonalParam();
        double h_offset = h_ratio * (ortho_param[1]-ortho_param[0]) / 2.0;
        double v_offset = v_ratio * (ortho_param[3]-ortho_param[2]) / 2.0;
        
        //3D Vector
        ray.x = h_offset;
        ray.y = v_offset;
        ray.z = 0.f;
        
        //From camera + offset
        fromPt = m_Camera.GetCameraPosition() + ray;
        
        return GetDirection();
    }
}

//Ground = XY Plane
vec3 vbViewInfo::GetRayIntersectionWithGround(ivec2 ray_win_pos)
{
    vec3 ray_dir, fromPt;
    ray_dir = GetRayFromCamera(ray_win_pos, fromPt);
    fromPt  = GetCameraPosition();
    
    
    //ray와 지정 된 평면의 교점을 구한다.
    vec3    intersect_pt;
    intersect_pt = m_ground_plane.GetIntersection(fromPt, fromPt+ray_dir);
    
#ifdef VB_CORE_DEBUG
 //   printf("[Camera] Ray X Ground = %f, %f, %f\r\n", intersect_pt.x, intersect_pt.y, intersect_pt.z);
#endif
    
    return intersect_pt;
}
