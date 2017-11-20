
#include "vbGeometry.hpp"

vbPlane::vbPlane()
{
    m_param[0]=m_param[1]=m_param[2]=m_param[3]=0.f; 
}

vbPlane::~vbPlane()
{
    
}

vec3 vbPlane::GetIntersection(vec3 p1, vec3 p2)
{
    //
    float denominator   = m_param[0] * (p1.x - p2.x) + m_param[1] * (p1.y - p2.y) + m_param[2] * (p1.z - p2.z);
    float numerator     = m_param[0] * p1.x + m_param[1] * p1.y + m_param[2] * p1.z + m_param[3];
    
    if(denominator==0.f)    return vec3(0.f,0.f,0.f);
    float u = numerator / denominator;
    
    return p1 + (p2 - p1) * u;
}

void vbPlane::SetPlane(vec3 ptOnPlane, vec3 norm)
{//AX+BY+CZ+D=0
    m_param[0] = norm.x;
    m_param[1] = norm.y;
    m_param[2] = norm.z;
    m_param[3] = -(ptOnPlane.x*norm.x+ptOnPlane.y*norm.y+ptOnPlane.z*norm.z);
}

void vbPlane::SetPlane(PlaneType PlaneType, float offset)
{
    switch (PlaneType)
    {
        case PLN_P_XY:
        {
            m_param[0]=m_param[1]=0.f;
            m_param[2]=1.f; m_param[3]=-offset; 
        }
        break;
        case PLN_N_XY:
        {
            m_param[0]=m_param[1]=0.f;
            m_param[2]=-1.f; m_param[3]=offset; 
        }
        break;
        case PLN_P_YZ:
        {
            m_param[1]=m_param[2]=0.f;
            m_param[0]=1.f; m_param[3]=-offset; 
        }
        break;
        case PLN_N_YZ:
        {
            m_param[1]=m_param[2]=0.f;
            m_param[0]=-1.f; m_param[3]=offset; 
        }
        break;
        case PLN_P_ZX:
        {
            m_param[0]=m_param[2]=0.f;
            m_param[1]=1.f; m_param[3]=-offset; 
        }
        break;
        case PLN_N_ZX:
        {
            m_param[0]=m_param[2]=0.f;
            m_param[1]=-1.f; m_param[3]=offset; 
        }
        break;
            
    }
}