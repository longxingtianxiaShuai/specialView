
#ifndef PISAiOS_vbPath_h
#define PISAiOS_vbPath_h

#include "Vector.hpp"
#include <vector>
#include <string>

using std::vector;

class vbPathRenderer;

class vbPath
{
public:
    vbPath();
    ~vbPath();
    
private:
    int             m_pathID;
    int             m_mapID;
    int             m_floorID;
    std::string     m_groupName;
    
    vector<vec3>    m_vt_path;
    unsigned short  m_segment_count;
    float           m_fWholeDistanceCm;  //여러개의 경로로 이뤄진 경로에서 진행율을 파악하기 위한 용도
    
    float           m_fPathDistance;
    
    vec3            m_pos_start;
    vec3            m_pos_end;
    
    vec3            m_minBound;
    vec3            m_maxBound;
    
    //Rendering
    float           m_corner_rad;
    float*          m_path_road_vts;
    unsigned short  m_road_vts_num;
    float           m_road_width_cm;
    float           m_gndOffset;
    
    float           m_fProgress;
    vec3            m_curr_yaw;
    float           m_RGBA[4];
    
    unsigned short  m_nRenderMode; //0=Solid,1=Triangle
    
    bool            m_bShow;
    
public:
    const float*    GetColorRGBA()                      {   return (const float*)&m_RGBA[0];          }
    void            SetColorRGBA(float r, float g, float b, float a)
    {   m_RGBA[0] = r;  m_RGBA[1] = g;  m_RGBA[2] = b; m_RGBA[3] = a;               }
    
    int             GetPathID()                         {   return m_pathID;        }
    void            SetPathID(int nPathID)              {   m_pathID = nPathID;     }
    int             GetMapID()                          {   return m_mapID;         }
    void            SetMapID(int nMapID)                {   m_mapID = nMapID;       }
    int             GetFloorID()                        {   return m_floorID;       }
    void            SetFloorID(int nFloorID)            {   m_floorID = nFloorID;   }
    std::string     GetGroupName()                      {   return m_groupName;     }
    void            SetGroupName(std::string grp_name)  {   m_groupName = grp_name; }
    
    void            SetStartEndPoint(vec3 start, vec3 end)  
    {   m_pos_start = start;    m_pos_end = end;    }
    vec3            GetStartPoint()                     {   return m_pos_start;     }
    vec3            GetEndPoint()                       {   return m_pos_end;       }
    
    unsigned short  GetSegmentCount()                   {   return m_segment_count; }
    unsigned short  GetVtCount()                        {   return m_road_vts_num;  }
    
    vec3            GetMinBound()                       {   return m_minBound;      }
    vec3            GetMaxBound()                       {   return m_maxBound;      }
   
    
    float           GetWholeCmDistance()                {   return m_fWholeDistanceCm;   }
    void            SetWholeCmDistance(float dist)      {   m_fWholeDistanceCm = dist;   }
    
    float           GetPathDistance()                   {   return m_fPathDistance; }
    
    float           GetNearestNodeDistance(vec3 pt3D);
    void            SetProgress(float fProgress)        {   m_fProgress = fProgress;    }
    void            SetProgress(vec3 ptAbout);
    float           GetProgress()                       {   return m_fProgress;     }
    vec3            GetCurrentYaw()                     {   return m_curr_yaw;      }
    
    //Set
    
    void            SetData(vector<float>& vt_list);
    vector<vec3>*   GetPathVts()                        {   return &m_vt_path;      }
    void            ClearData();
    float*          GetVtArray();
    

    //Rendering
    //Ground offset
    float   GetGroundOffset()                       {   return m_gndOffset;             }
    void    SetGroundOffset(float gndoffset)        {   m_gndOffset = gndoffset;        }
    
    //Road width
    void    SetRoadWidth(float widthCm)             {   m_road_width_cm = widthCm;      }
    float   GetRoadWidth()                          {   return m_road_width_cm;         }
    
    //Corner radius
    void    SetCornerRad(float rad)                 {   m_corner_rad = rad;             }
    float   GetCornerRad()                          {   return m_corner_rad;            }
    
    int    BuildPathRoadVts(bool bThickPath=false, bool bArrowTip=true, unsigned short nMode=0/*0=Solid,1=Triangles*/, const float* param = 0);
    float*  GetPathRoadVts()                        {   return m_path_road_vts;         }
    
    unsigned short  GetRenderMode()                 {   return m_nRenderMode;           }
   
    int     ExtractPartialPathWithLevel(vector<vbPath*>*  pNewPathes, float minLevel, float maxLevel);
    
    bool    IsVisible()                             {   return m_bShow;                 }
    void    SetVisibility(bool bShow)               {   m_bShow = bShow;                }

private:
    float  UpdatePathDistance();
    
    
    int    BuildSkelectonPathRoad();
    int    BuildRectPathRoad(bool bThickPath, bool bArrowTip);//Mode=0
    int    BuildTriangleArrayPathRoad(bool bThickPath, bool bArrowTip, const float* param);//Mode=1
    
 
    
};


#endif
