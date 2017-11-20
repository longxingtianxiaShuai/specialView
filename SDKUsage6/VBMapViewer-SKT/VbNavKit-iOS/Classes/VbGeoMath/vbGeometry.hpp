#pragma once

#include "Vector.hpp"

class vbPlane
{
//Ax+By+Cz+D = 0
public:
    vbPlane();
    ~vbPlane();
    
    enum PlaneType
    {
        PLN_P_XY=0,
        PLN_N_XY,
        PLN_P_YZ,
        PLN_N_YZ,
        PLN_P_ZX,
        PLN_N_ZX
    };
    
protected:
    float   m_param[4];
    
public:
    float*  GetPlane()                  {   return m_param; }
    void    SetPlane(float* plane_param) 
    {   m_param[0]=plane_param[0];   m_param[1]=plane_param[1];   m_param[2]=plane_param[2];   m_param[3]=plane_param[3];   }
    void    SetPlane(PlaneType PlaneType, float offset);
    void    SetPlane(vec3 ptOnPlane, vec3 norm);
    
    vec3    GetIntersection(vec3 p1, vec3 p2);
    
    
};