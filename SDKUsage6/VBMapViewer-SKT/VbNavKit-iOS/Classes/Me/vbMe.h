
#ifndef PISAiOS_vbMe_h
#define PISAiOS_vbMe_h

#include "Quaternion.hpp"


///   \class vbMe
///   \brief [내 위치] 정보를 관리하는 클래스
class vbMe
{
private:
    
    
    //Me base info.
    vec3        m_position;         //Where I am
    vec3        m_position_default;
    
    ivec2       m_2D_pos;
    Quaternion  m_dir;
    Quaternion  m_dir_default;
    
    float       m_tolerance_rad;    //Position tolerance
    float       m_eye_height;
    
    bool        m_bEnabled;
    bool        m_bStateChanged;
    
    //Rendering info.
    unsigned char   m_tol_circle_numSeg;
    float*          m_tol_circle_vts;
    float           m_rgbTolRange[4];
    float           m_rgbTolRangeOut[4];
    
    

public:
    vbMe();
    ~vbMe();
    
    void    Init();
    
    /** \brief
     사용 가능 상태인지를 확인 함.
     \remark
     사용 가능 상태는 단순한 flag이며, 사용 불가능 상태라고 해서 사용에 문제가 있는 것은 아니다. 
     \return
     true - 사용 가능 상태, \n false - 사용 불가능 상태
     */
    bool    IsEnabled()             {   return m_bEnabled;      }
    /** \brief
     사용 가능 상태를 설정 함.
     \remark
     사용 가능 상태는 단순한 flag이며, 사용 불가능 상태라고 해서 사용에 문제가 있는 것은 아니다. 
     */
    void    Enable(bool bEnable) ///<      true - 사용 가능 상태, \n false - 사용 불가능 상태
                   {   m_bEnabled = bEnable;   }
    
    bool    IsChanged()                 {   return m_bStateChanged;     }
    void    SetChangeFlag(bool bChaged) {   m_bStateChanged = bChaged;  }
    
    //position
    vec3    GetPosition()           {   return m_position;  }
    void    SetPosition(vec3 pos);
    vec3    GetPositionDefault()    {   return m_position_default;  }
    void    SetPositionDefault(vec3 pos);
    
    ivec2   Get2DPosition()         {   return m_2D_pos;    }
    void    Set2DPosition(ivec2 pos){   m_2D_pos = pos;     }
    float   GetPositionTolerance()              {   return m_tolerance_rad; }
    void    SetPositionTolerance(float tol)     {   m_tolerance_rad = tol;  }
    
    //direction
    Quaternion  GetQuaternion()                 {   return m_dir;   }
    void        SetQuaternion(Quaternion dir)   {   m_dir = dir;    }
    
    Quaternion  GetQuaternionDefault()                 {   return m_dir_default;   }
    void        SetQuaternionDefault(Quaternion dir)   {   m_dir_default = dir;    }
    
    //eye height
    float   GetEyeHeight()              {   return m_eye_height;    }
    void    SetEyeHeight(float eHeight) {   m_eye_height = eHeight; }

   
    //Rendering info.
    //Range mark
    void            SetMarkSegmentCount(unsigned char count)    {   m_tol_circle_numSeg = count;    }
    unsigned char   GetSegmentCount()                           {   return m_tol_circle_numSeg;     }
    
    float*          GetRangeColor()             {   return m_rgbTolRange;       }
    float*          GetRangeOutColor()          {   return m_rgbTolRangeOut;    }
    float*          GetVtArray()                {   return m_tol_circle_vts;    }//첫번째 점은 중심.
    float*          GetOutVtArray()             {   return &m_tol_circle_vts[3];    }
    void            UpdateVtArray();
    
    
    
    
};

#endif
