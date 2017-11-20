#include "vbPath.h"
#include <iostream>
#include <float.h>

vbPath::vbPath()
{
    m_vt_path.clear();
    m_segment_count = 0;    
    m_fWholeDistanceCm = 0.f;
    
    m_path_road_vts = NULL;
    m_road_vts_num = 0;
    m_gndOffset = 0.f;
    m_road_width_cm = 400.f;
    
    m_pathID = 0;
    m_mapID = -1;
    m_floorID = INT16_MAX;//0-all
    
    m_fProgress = 0.f;
    
    m_RGBA[0] = m_RGBA[1] = m_RGBA[2] = m_RGBA[3] = -1.f;   //정의 안된 상태는 음수 ==> 기본색 적용
    
    m_nRenderMode = 0;//Solide
    m_groupName = "";
    
    m_bShow = true;
    
}

vbPath::~vbPath()
{
    ClearData();   
}

void vbPath::ClearData()
{
    m_vt_path.clear();
    m_segment_count = 0;   
    m_fWholeDistanceCm = 0.f;
    

    if(m_path_road_vts)
        delete  [] m_path_road_vts;
    
    m_path_road_vts = NULL;
    m_road_vts_num = 0;
    
    m_fProgress = 0.f;
    m_groupName = "";
    m_floorID=INT16_MAX;
    
    m_minBound = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_maxBound = vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
  
}

float* vbPath::GetVtArray()
{
    if(m_vt_path.size()==0) return NULL;
    
    return (float*)(m_vt_path[0].Pointer());   
}



void vbPath::SetData(vector<float>& vt_list)
{
    ClearData();
    
    //2점 이상이라야 한다.
    if(vt_list.size()<6)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "--------------------- Lack of Path data in vector ------------------------\r\n";
#endif
        return;
    }

    int vector_size = (int)vt_list.size();
        
    m_segment_count = vector_size/3 - 1;
        
    //입력은 3차원 좌표.
    vec3 vt;
    for (int i=0; i<=m_segment_count; i++)
    {
        vt.x = vt_list.at(i*3+0);
        vt.y = vt_list.at(i*3+1);
        vt.z = vt_list.at(i*3+2);
        
        if (m_minBound.x > vt.x) m_minBound.x = vt.x;
        if (m_minBound.y > vt.y) m_minBound.y = vt.y;
        if (m_minBound.z > vt.z) m_minBound.z = vt.z;
        
        if (m_maxBound.x < vt.x) m_maxBound.x = vt.x;
        if (m_maxBound.y < vt.y) m_maxBound.y = vt.y;
        if (m_maxBound.z < vt.z) m_maxBound.z = vt.z;
        
        m_vt_path.push_back(vt);
    }
    
    UpdatePathDistance();
}

float vbPath::UpdatePathDistance()
{
    float total_dist = 0.f;
    
    vector<vec3>* vts = GetPathVts();
    if (vts->size()<2) {
        return 0.f;
    }
    
    vec3 start, end;
    for (int i=0; i<vts->size()-1; i++)
    {
        start = vts->at(i);
        end = vts->at(i+1);
        
        total_dist += (start-end).size();
    }
    
    m_fPathDistance = total_dist;
    
    return total_dist;
}


int  vbPath::BuildPathRoadVts(bool bThickPath, bool bArrowTip, unsigned short nMode/*0=Solid,1=Triangles*/, const float* param)
{
    //BuildSkelectonPathRoad();
    switch(nMode)
    {
        case 0://Solid
            return BuildRectPathRoad(bThickPath,bArrowTip);
            break;
            
        case 1://Triangles
            return BuildTriangleArrayPathRoad(bThickPath, bArrowTip, param);
            break;
            
        default:
            return -1;
    }
    return -1;
}

int    vbPath::BuildTriangleArrayPathRoad(bool bThickPath, bool bArrowTip, const float* param)//Mode=1
{
    float path_width = m_road_width_cm;
    if(bThickPath) path_width = m_road_width_cm * 2.f;
    
    if(m_path_road_vts)
        delete  [] m_path_road_vts;
    m_path_road_vts = NULL;
    
#ifdef VB_CORE_DEBUG
    unsigned short segCount = GetSegmentCount();
    if (segCount<1) {
        std::cout << "------------- Lack of Segment data --------------- \r\n";
        return -1;
    }
#endif
    
    vector<vec3>* vts = GetPathVts();
#ifdef VB_CORE_DEBUG
    if (vts->size()<2) {
        std::cout << "------------- Lack of Path data to build Path surface --------------- \r\n";
        return -2;
    }
#endif
    
    float edge_length,half_width, unit_length, travel_length = 0.f;
    half_width = path_width/2.f;
    
    if(param==0) unit_length = half_width;
    else
        unit_length = half_width * param[0]; //param[0] = Unit length

    //seed vertex의 개수 = Path 길이 / 삼각형 간격(일단 폭으로...)
    int numSeedVtx = (int)(m_fPathDistance / unit_length );
    m_road_vts_num = numSeedVtx*3;  //SeedVtNum = TriangleNum,
    //ArrowTip이면 마지막 화살표를 좀 더 크게 강조하도록 함.
    m_path_road_vts = new float[m_road_vts_num * 3];
    
    
    /* v0
     start+left
     |      \
     |        \
     start------v2---------------end
     |         /
     |       /
     start-left
     v1
     */
    
    vec3 seed,v0,v1,v2;
    vec3 start, end,left,edge_dir,up;
    up = VEC3_UNITY;

#ifdef VB_CORE_DEBUG
    std::cout << "[Path Generating] Segment count = " << numSeedVtx << std::endl;
    std::cout << "[Path Generating] vt num = " << m_road_vts_num << std::endl;
#endif
    
    int tIDX = 0;
    for (int i=0; i<vts->size()-1; i++)
    {
        start = vts->at(i);
#ifdef VB_CORE_DEBUG
        std::cout << "Path Pt[" << i << "]" << start.x << ", "  << start.y << ", "  << start.z << std::endl;
#endif
        end = vts->at(i+1);
        if((start-end).size()<0.000001)//Almost same
        {
#ifdef VB_CORE_DEBUG
            std::cout << "------------- Duplicated path data --------------- \r\n";
#endif
            if(i>1 && i<vts->size()-2)      start = vts->at(i-1);
        }
        
        
        //Base length
        edge_length = (end-start).size();
        //travel_length = 0.f;
        
        //Base vector
        edge_dir = (end-start).Normalized();
        left = (up.Cross(edge_dir)).Normalized() * half_width;
        
       // std::cout << "[Path Generating] Segment order = " << i << std::endl;
        
        while (edge_length>travel_length && tIDX<numSeedVtx)
        {
         //   std::cout << "[Path Generating] triangle idx = " << tIDX << std::endl;
            
            seed = start + edge_dir * travel_length;
            
            v0 = seed + left;
            v1 = seed - left;
            v2 = seed + edge_dir * half_width * param[1];  //param[1] = 삼각형 크기
            
            m_path_road_vts[tIDX*9+0] = v0.x;    m_path_road_vts[tIDX*9+1] = v0.y;    m_path_road_vts[tIDX*9+2] = v0.z;
            m_path_road_vts[tIDX*9+3] = v1.x;    m_path_road_vts[tIDX*9+4] = v1.y;    m_path_road_vts[tIDX*9+5] = v1.z;
            m_path_road_vts[tIDX*9+6] = v2.x;    m_path_road_vts[tIDX*9+7] = v2.y;    m_path_road_vts[tIDX*9+8] = v2.z;
            
            travel_length += unit_length;
            tIDX++;
        }
        
        //travel_length가  edge_length보다 커진 경우. - travel_length를 다음 길이에서 차감 할 값을 남겨둠.
        travel_length -= edge_length; // 초과 된 값을 다음에 빼주게 됨.
       
    }
    
    /*
    
    if(bArrowTip)//마지막 삼각형을 좀 더 강조함.
    {
        int start_index = (int)(vts->size()-1)*12;
        
        if(!(start_index-6>=0 && start_index+11<m_path_road_vtsSize))
        {
            std::cout << "------------- Out of path head index --------------- \r\n";
            return -4;
        }
        
        //마지막 두 점을 화살표 머리 만큼 뒤로 조정한다.
        //화살표 머리
        vec3 tip;
        tip = (end-start).Normalized() * path_width * 2.f;
        end = end - tip;
        m_path_road_vts[start_index-6] = end.x + left.x;
        m_path_road_vts[start_index-5] = end.y + left.y;
        m_path_road_vts[start_index-4] = end.z + left.z;
        m_path_road_vts[start_index-3] = end.x - left.x;
        m_path_road_vts[start_index-2] = end.y - left.y;
        m_path_road_vts[start_index-1] = end.z - left.z;
        
        
        v0 = end + tip; //tip
        v1 = end - left * 2.f;//right tip
        v2 = end + left * 2.f;//left tip
        

        
        
        m_path_road_vts[start_index+0] = v0.x;    m_path_road_vts[start_index+1] = v0.y;    m_path_road_vts[start_index+2] = v0.z;
        m_path_road_vts[start_index+3] = v1.x;    m_path_road_vts[start_index+4] = v1.y;    m_path_road_vts[start_index+5] = v1.z;
        m_path_road_vts[start_index+6] = v2.x;    m_path_road_vts[start_index+7] = v2.y;    m_path_road_vts[start_index+8] = v2.z;
        m_path_road_vts[start_index+9] = v0.x;    m_path_road_vts[start_index+10] = v0.y;    m_path_road_vts[start_index+11] = v0.z;
        
    }
     */
        /*          end+left*2
         |\
         | \
         --------end + left
         |    \
         |     \
         end     end+tip
         |     /
         |    /
         ------- end -left
         |  /
         |/
         end-left*2
         */
    
    m_nRenderMode = 1;
    return 0;
    
    
}

int vbPath::BuildRectPathRoad(bool bThickPath, bool bArrowTip)
{
    float path_width = m_road_width_cm;
    if(bThickPath) path_width = m_road_width_cm * 2.f;
    
    if(m_path_road_vts)
        delete  [] m_path_road_vts;
    m_path_road_vts = NULL;
 
    
    unsigned short segCount = GetSegmentCount();
#ifdef VB_CORE_DEBUG
    if (segCount<1) {
        std::cout << "------------- Lack of Segment data --------------- \r\n";
        return -1;
    }
#endif
    
    vector<vec3>* vts = GetPathVts();
#ifdef VB_CORE_DEBUG
    if (vts->size()<2) {
        std::cout << "------------- Lack of Path data to build Path surface --------------- \r\n";
        return -2;
    }
#endif
    
    m_road_vts_num = segCount*4;
    int count_tip = 0;
    if(bArrowTip)   count_tip = 12;
    
    int m_path_road_vtsSize=m_road_vts_num*3+count_tip;
    m_path_road_vts = new float[m_path_road_vtsSize]; // 3float * 4vt * seg + 12(Arrow)

   
    
    vec3 v0,v1,v2,v3;
    vec3 start, end,left,up;
    up = VEC3_UNITY;
    
    /* v0                           v2
    start+left------------------end+left
        |                           |
        |                           |
     start                          end
        |                           |   
        |                           |
     start-left-----------------end-left
        v1                          v3
     */
    
    for (int i=0; i<vts->size()-1; i++)
    {
        start = vts->at(i);
#ifdef VB_CORE_DEBUG
        std::cout << "Path Pt[" << i << "]" << start.x << ", "  << start.y << ", "  << start.z << std::endl;
#endif
        end = vts->at(i+1);
        if((start-end).size()<0.000001)//Almost same
        {
#ifdef VB_CORE_DEBUG
            std::cout << "------------- Duplicated path data --------------- \r\n";
#endif
            if(i>1 && i<vts->size()-2)      start = vts->at(i-1);
        }
        
        left = (up.Cross(end-start)).Normalized() * path_width / 2.f;
        
        v0 = start + left;
        v1 = start - left;
        v2 = end + left;
        v3 = end - left;
        
        m_path_road_vts[i*12+0] = v0.x;    m_path_road_vts[i*12+1] = v0.y;    m_path_road_vts[i*12+2] = v0.z;
        m_path_road_vts[i*12+3] = v1.x;    m_path_road_vts[i*12+4] = v1.y;    m_path_road_vts[i*12+5] = v1.z;
        m_path_road_vts[i*12+6] = v2.x;    m_path_road_vts[i*12+7] = v2.y;    m_path_road_vts[i*12+8] = v2.z;
        m_path_road_vts[i*12+9] = v3.x;    m_path_road_vts[i*12+10] = v3.y;    m_path_road_vts[i*12+11] = v3.z;

        if(!(i*12+11<m_path_road_vtsSize))
        {
#ifdef VB_CORE_DEBUG
            std::cout << "------------- Out of path body index --------------- \r\n";
#endif
            return -3;
        }
    }
    
    if(bArrowTip)
    {
        int start_index = (int)(vts->size()-1)*12;
        
        if(!(start_index-6>=0 && start_index+11<m_path_road_vtsSize))
        {
#ifdef VB_CORE_DEBUG
            std::cout << "------------- Out of path head index --------------- \r\n";
#endif
            return -4;
        }
        
        //마지막 두 점을 화살표 머리 만큼 뒤로 조정한다.
        //화살표 머리
        vec3 tip;
        tip = (end-start).Normalized() * path_width * 2.f;
        end = end - tip;
        m_path_road_vts[start_index-6] = end.x + left.x;
        m_path_road_vts[start_index-5] = end.y + left.y;
        m_path_road_vts[start_index-4] = end.z + left.z;
        m_path_road_vts[start_index-3] = end.x - left.x;
        m_path_road_vts[start_index-2] = end.y - left.y;
        m_path_road_vts[start_index-1] = end.z - left.z;
        
        
        v0 = end + tip; //tip
        v1 = end - left * 2.f;//right tip
        v2 = end + left * 2.f;//left tip
        
        /*          end+left*2
         |\
         | \
         --------end + left
         |    \
         |     \
         end     end+tip
         |     /
         |    /
         ------- end -left
         |  /
         |/
         end-left*2
         */
        
        
        m_path_road_vts[start_index+0] = v0.x;    m_path_road_vts[start_index+1] = v0.y;    m_path_road_vts[start_index+2] = v0.z;
        m_path_road_vts[start_index+3] = v1.x;    m_path_road_vts[start_index+4] = v1.y;    m_path_road_vts[start_index+5] = v1.z;
        m_path_road_vts[start_index+6] = v2.x;    m_path_road_vts[start_index+7] = v2.y;    m_path_road_vts[start_index+8] = v2.z;
        m_path_road_vts[start_index+9] = v0.x;    m_path_road_vts[start_index+10] = v0.y;    m_path_road_vts[start_index+11] = v0.z;
        
    }
    
    m_nRenderMode = 0;
    
    return 0;
    
    
}

int vbPath::BuildSkelectonPathRoad()
{
    if(m_path_road_vts)
        delete  [] m_path_road_vts;
    m_path_road_vts = NULL;
    
    unsigned short segCount = GetSegmentCount();
    m_path_road_vts = new float[(segCount+1)*3];
    vector<vec3>* vts = GetPathVts();
    m_road_vts_num = vts->size();
    for (int i=0; i<vts->size(); i++)
    {
        m_path_road_vts[i*3+0] = vts->at(i).x;
        m_path_road_vts[i*3+1] = vts->at(i).y;
        m_path_road_vts[i*3+2] = vts->at(i).z;
    }
    
    return 0;
}

void vbPath::SetProgress(vec3 ptAbout)
{
    if(m_vt_path.size()<2)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "----------------- Abnormal path to make progress ------------------\r\n";
#endif
        return;
    }
    
    //가장 가까운 노드를 찾는다.
    float minDist = FLT_MAX;
    float dist = FLT_MAX;
    int nearest_id = 0;
    for(int i=0;i<m_vt_path.size();i++)
    {
        dist = (m_vt_path.at(i)-ptAbout).size();
        
        if(dist<minDist)
        {
            minDist = dist;
            nearest_id = i;
        }
    }
    
    //시작점과 가장 가까운 경우
    float u=0.f;
    vec3 start, end;
    if (nearest_id==0)//First
    {
        start = m_vt_path.at(0);
        end = m_vt_path.at(1);
        
        u = ( (ptAbout.x - start.x)*(end.x-start.x) +  (ptAbout.y - start.y)*(end.y-start.y) +  (ptAbout.z - start.z)*(end.z-start.z) ) / ( (start.x - end.x)*(start.x - end.x) +(start.y - end.y)*(start.y - end.y) + (start.z - end.z)*(start.z - end.z) );
        
        if(u<0.f)       m_fProgress = 0.f;
        else if(u>1.f)  m_fProgress = 1.f;
        else            m_fProgress = u;
    }
    else if(nearest_id==m_vt_path.size()-1)//Last
    {
        start = m_vt_path.at(nearest_id-1);
        end = m_vt_path.at(nearest_id);
        
        u = ( (ptAbout.x - start.x)*(end.x-start.x) +  (ptAbout.y - start.y)*(end.y-start.y) +  (ptAbout.z - start.z)*(end.z-start.z) ) / ( (start.x - end.x)*(start.x - end.x) +(start.y - end.y)*(start.y - end.y) + (start.z - end.z)*(start.z - end.z) );
        
        if(u<0.f)       m_fProgress = (float)(nearest_id-1);
        else if(u>1.f)  m_fProgress = (float)nearest_id;
        else            m_fProgress = (float)(nearest_id-1) + u;
    }
    else 
    {
        vec3 prjA, prjB;
        
        //앞에꺼
        start = m_vt_path.at(nearest_id-1);
        end = m_vt_path.at(nearest_id);
        
        u = ( (ptAbout.x - start.x)*(end.x-start.x) +  (ptAbout.y - start.y)*(end.y-start.y) +  (ptAbout.z - start.z)*(end.z-start.z) ) / ( (start.x - end.x)*(start.x - end.x) +(start.y - end.y)*(start.y - end.y) + (start.z - end.z)*(start.z - end.z) );
        if (u<0.f)      u = 0.f;
        else if(u>1.f)  u = 1.f;
        
        prjA = start + (end-start)*u;
        
        //다음꺼
        start = m_vt_path.at(nearest_id);
        end = m_vt_path.at(nearest_id+1);
        float u2 = ( (ptAbout.x - start.x)*(end.x-start.x) +  (ptAbout.y - start.y)*(end.y-start.y) +  (ptAbout.z - start.z)*(end.z-start.z) ) / ( (start.x - end.x)*(start.x - end.x) +(start.y - end.y)*(start.y - end.y) + (start.z - end.z)*(start.z - end.z) );

        if (u2<0.f)      u2 = 0.f;
        else if(u2>1.f)  u2 = 1.f;
        
        prjB = start + (end-start)*u2;
        
        if( (ptAbout-prjA).size() > (ptAbout-prjB).size() )//다음 에지가 더 가까움.
        {
            m_fProgress = (float)nearest_id + u2;
        }
        else
            m_fProgress = (float)nearest_id -1.f + u;
        
        
    }
    
    unsigned    short start_idx = (unsigned short)(m_fProgress);
    
    if(m_vt_path.size()>start_idx+1)
    {
        start = m_vt_path.at(start_idx);
        end = m_vt_path.at(start_idx+1);
        m_curr_yaw =(end - start).Normalized();
    }
}


float vbPath::GetNearestNodeDistance(vec3 pt3D)
{
    float minDist = FLT_MAX;
    float dist = FLT_MAX;
    for(int i=0;i<m_vt_path.size();i++)
    {
        dist = (m_vt_path.at(i)-pt3D).size();
        
        if(dist<minDist)    minDist = dist;
    }  
    return dist;
}

int  vbPath::ExtractPartialPathWithLevel(vector<vbPath*>*  pNewPathes, float minLevel, float maxLevel)
{
    int extracted_num = 0;
    
    vbPath* pNewPath = 0;
    
    for(int i=0;i<m_vt_path.size();i++)
    {
        //그 점과이 구간 내에 포함되어 있으면 더한다.
        if (m_vt_path.at(i).y>minLevel && m_vt_path.at(i).y < maxLevel)
        {
            if (pNewPath==NULL)//추가 중인 경로가 없으면 경로를 만든다.
            {
                pNewPath = new vbPath;
            }
            pNewPath->GetPathVts()->push_back(m_vt_path.at(i));
        }
        else
        {
            //그 점이 해당 구간에 포함되어 있지 않으면, 그 점은 포함시키지 않는다.
            //만약, 작성 중인  Path가 있었으면, vertex의 개수가 2개이상이면 추가한다.
            if (pNewPath)
            {
                if(pNewPath->GetPathVts()->size()<2)
                {
                    pNewPath->ClearData();
                    delete pNewPath;
                }
                else
                {
                    pNewPathes->push_back(pNewPath);
                    extracted_num++;
                }
                pNewPath = NULL;
            }
        }
    }
 
    //추가 중인 상태로 마친 경우.
    if (pNewPath)
    {
        if(pNewPath->GetPathVts()->size()<2)
        {
            pNewPath->ClearData();
            delete pNewPath;
        }
        else
        {
            pNewPathes->push_back(pNewPath);
            extracted_num++;
        }
        pNewPath = NULL;
    }
    
    
    return extracted_num;
    
}


