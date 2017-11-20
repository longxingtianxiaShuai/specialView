
#include <iostream>
#include "vbMe.h"

#include <cmath>

/** \brief
        vbMe의 생성자
    \remark
        초기 설정을 함.
    \return
        없음.
 */
vbMe::vbMe()
{
    Init();   
    
}

vbMe::~vbMe()
{
    if(m_tol_circle_vts)    delete [] m_tol_circle_vts;
    
}

/** \brief
 vbMe를 초기화 함.
 \remark
 각 종 변수들을 초기화 하고, Coex에 맞는 기본 위치를 설정함.
 \return
 없음.
 */
void vbMe::Init()
{
    m_position = vec3(0.f,0.f,0.f);         //Where I am
    m_2D_pos = ivec2(0,0);
    m_tolerance_rad = 0.f;    //Position tolerance
    m_eye_height = 180.f;
    
    m_bEnabled = false;
    m_bStateChanged = false;
    
    
    //Coex Init
    m_position_default = m_position = vec3(-27052.1f, 180.f, 13640.2f);
    m_dir_default = m_dir = Quaternion(0.f, -0.377526f, 0.f, 0.925999f);
    
    m_tolerance_rad = 1000.f;
    
    //Rendering info.
    m_tol_circle_vts = NULL;
    m_tol_circle_numSeg = 40;
    
    m_rgbTolRange[0] = 0.85f;
    m_rgbTolRange[1] = 0.8f;
    m_rgbTolRange[2] = 0.0f;
    m_rgbTolRange[3] = 0.5f;
    
    m_rgbTolRangeOut[0] = 1.f;
    m_rgbTolRangeOut[1] = 0.f;
    m_rgbTolRangeOut[2] = 0.f;
    m_rgbTolRangeOut[3] = 0.f;   
    
    UpdateVtArray();
}

/** \brief
 내 위치를(3D) 지정 함.
 \remark
 위치를 설정하면 높이는 무조건 10으로 조정 됨.
 \return
 없음.
 */
void vbMe::SetPosition(vec3 pos) ///< 새로운 '내위치' 좌표 
{
    m_position = pos;   
    m_position.y = 10.f;
}

//////// Rendering info
void vbMe::UpdateVtArray()
{
    if(m_tol_circle_vts)    delete [] m_tol_circle_vts; 
    
    m_tol_circle_vts = new float[(m_tol_circle_numSeg+2)*3];
    
    float step_angle = Pi * 2.f / (float)m_tol_circle_numSeg;
    
    m_tol_circle_vts[0]= m_tol_circle_vts[2]=0.f;
    m_tol_circle_vts[1]= 0.f;
    
    for (unsigned char i=1; i<m_tol_circle_numSeg; i++)
    {
        m_tol_circle_vts[i*3+0] = GetPositionTolerance() * cos(step_angle*(float)i);
        m_tol_circle_vts[i*3+1] = 0.f;
        m_tol_circle_vts[i*3+2] = -GetPositionTolerance() * sin(step_angle*(float)i);
    }
    m_tol_circle_vts[m_tol_circle_numSeg*3+0]= m_tol_circle_vts[3];
    m_tol_circle_vts[m_tol_circle_numSeg*3+1]= m_tol_circle_vts[4];
    m_tol_circle_vts[m_tol_circle_numSeg*3+2]= m_tol_circle_vts[5];
    
}
