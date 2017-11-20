#include "Interfaces.hpp"
#include "vbMeshManager.hpp"
#include "vbMaterialManager.hpp"
#include "vbMeshFactory.hpp"
#include "vbMaterialFactory.hpp"
#include "vbConfiguration.hpp"
#include "vbViewInfo.hpp"
#include "vbPOIFactory.h"
#include "vbMe.h"
#include "vbPathManager.h"
#include "vbInstanceManager.h"
#include "vbInstance/vbInstanceFactory.h"
#include "tess/vbPolyManager.h"
#include "vbMeshGroup.h"
#include "vbMeshGroupManager.h"


#include <iostream>

//#import "GLView.h"

using namespace std;


class ApplicationEngine : public IApplicationEngine
{
public:
    ApplicationEngine(IRenderingEngine* renderingEngine);
    ~ApplicationEngine();
    
    void Initialize(int width, int height);
    bool IsInitialted()     {   return m_bIsInitiated; };
 
    //Mesh & Texture
    //Contents path
    void SetMeshPath(int length_with_null, const char* path);
    void SetTexturePath(int length_with_null_and_slash, const char* path);
    VBMapErrorCode  LoadMesh(bool bAsStatic);
    VBMapErrorCode  ImportInstance(const char* path);
    int             AddInstance(const char* sbm_name_without_extension, unsigned short mesh_id_in_sbm, const float* transform_mat);
    
    void ClearData(bool bClearRender=true, bool bClearMesh=true, bool bClearMaterial=true, bool bClearView=true, bool bClearPath=true);
   
    
    //Rendering
    void Render() const;//Return true for update POI
    void SetSolidRendering(bool bOnSolidRendering = true);
    void SetRenderingMode(vbGLRenderMode render_mode);
  
    //Picking
    bool GetPickedMeshID(int tx, int ty, unsigned short* mesh_ID);
    void SetPickingMode(bool bPickingOn);
    bool IsPickingMode()        {   return m_bPickingMode;  }
    //Animation
    void UpdateAnimation(float dt);
    
    //Touch
    void InvalidateManipulation();
    void OnSingleFingerUp(int posX, int posY, int tapCount);
    void OnSingleFingerDown(int posX, int posY, int tapCount);
    void OnSingleFingerMove(int newposX, int newposY, int tapCount);
    
    void OnDoubleFingerUp(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2);
    void OnDoubleFingerDown(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2);
    void OnDoubleFingerMove(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2);
    void SetViewControlSet(vbViewInfo::vbViewControlSet eViewControlSet);
 
    
    //Camera
    //Camera to any point
    void SetCameraToSee3DPoint(float newCenterX,float newCenterY,float newCenterZ);
    vec3 GetCameraPosition();
    void SetCameraPitchRatio(float pitch_ratio);
    
    void SetCamera(float cx, float cy, float cz, float yaw_deg_from_nZ, float pitch_deg_from_xz_plane, float fDist);
    void SetOrthoCameraWithPosOffset(VBOrthoCameraDirection mode, vec3 pos, float offset);
    bool SetCameraOnPath(float px2D, float py2D, int nFloor, int nIndexOfFromVt, float fProgress, float fPitchRatio, float fDistance, bool bFitDirection);
    bool SetCameraOnPath(float px2D, float py2D, int nFloor, bool bFitDirection);
    void SetHRotationTouchRange(float fVerticalRatioFromBottom);
    vec3 GetOrbitCenter();
    void SetCameraMinMaxOffset(float fMinOffset, float fMaxOffset);
   
    float GetPitchRatio();
    float GetPitchDegree();
    float GetYawDegree();
    void GetCemeraParam(float* params);//Yaw - Pitch - PosX - PosY - PosZ - Offset
    void FitAABBtoScreen(bool bUpdateAABB=false, bool bBoundMinMaxDist=false);
    void FitAABB2VisibleGroup(bool bBoundMinMaxDist=false);
    void SetGroundLevel(float fGoundLevel);
    void SetCameraPostion2VisibleGroupCenter(float fCameraDistance);
    void SetHRotationAnimation(bool bStart, float fYawStep);//
    bool IsOnHRotatingAnimation();
    
    void BrokeControl(bool bBrokePan, bool bBrokeZoom, bool bBrokeRotate);
    void UpdatePanBoundAsAABB();
    void UpdatePanBound(vec3 min, vec3 max);
    
    
    //Path
    int SetPathData(float* pts, unsigned short ptNum, float total_dist, int mapIndex=0, bool bArrowTip=true);
    int  SetPathDataWithGroupName(std::string group_name, float* pts, unsigned short ptNum, float total_dist, int mapIndex=0, bool bArrowTip=0);
    int  SetPathDataWithFloorID(int floorID, float* pts, unsigned short ptNum, float total_dist, int mapIndex=0, bool bArrowTip=0);
    
    int AddPathData(float* pts, unsigned short ptNum, float* fRGBA, float fPathWidth, bool bArrowTip = true, unsigned short nMode=0, const float* param = 0);
    int  AddPathDataWithGroupName(std::string group_name, float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip=0, unsigned short nMode=0, const float* param = 0); /*Mode 0=Solid, 1=Triangles*/
    int  AddPathDataWithFloorID(int floorID, float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip=0, unsigned short nMode=0, const float* param = 0); /*Mode 0=Solid, 1=Triangles*/
    
    
//    void showPath(int pathID);
    void showPath(bool bShow);
    void showPathAtFloor(int nFloor);
    vbPath*  UpdateVisiblePath(float px2D, float py2D, int nFloor);
    int SetPathData2D(float* pts, int* floors, unsigned short ptNum, bool bArrowTip=true);
    
    void SetMyPosition(float org_x, float org_py, float org_pz,float corrected_x, float correctedpy, float corrected_pz);
    void ShowMyPosition(bool bShowMyPos);
    void SetMyPosDiscState(bool bShow, const float* rgba, const float* pos, float rad);

    
    
    //World2Window
    //World2Window
    mat4*           GetViewProjectionMatrix();
    ivec2           GetScreenSize();
   
    //Device orientation
    void SetDeviceDirection(DeviceDirection device_direction, bool bAnimate);
   
    //Runtime config
    //DO NOT MODIFY bUseVBO on RENDERING. IF YOU WANT TO CHANGE THIS, USE AS 'CLEAR-CHANGE-LOAD'
    void SetRuntimeConfig(RunTimeConfig* pConfig);
    void GetRuntimeConfig(RunTimeConfig* pConfig);
   
    
   
    void UseLocalTexture(bool bUseLocalTexture=false)   { m_bUseLocalTexture = bUseLocalTexture; }//true - downloaded texture, false - resource texture
	    
    //Navigation
    void InitView();
    
    bool IsUpdatedImageButtonState();
    
    vec3 GetGroundPositionOnScreenXY(ivec2 pos);
    
    bool IsViewUpdated();
    void SetViewUpdated(bool bUpdated);
    
    // 디버그용 로그를 남길지 여부를 결정한다.
    void EnableDebug(bool bEnableDebug) {   m_bDebugLog = bEnableDebug; }
    
    //2D Draw
    void AddPolygon(std::string name, unsigned char* rgba, vbRing* pNewRing);
    VBMapErrorCode AddHoleToLastPolygon(vbRing* pNewRing);
    void ConvertPolygonToMesh();

    //Group
    std::string IsExistingGroupContaining(std::string group_name);
    
    //각 그룹의 Show/Hide 설정(단, 메쉬까지 적용을 하는 것을 별도임.) 그룹 이름이  nil이면 전체.
    void    SetVisibilityFlagWithGroupName(bool bVisible, std::string group_name);
    //Visibility flag를 각 메쉬에 적용한다.
    void    ApplyVisibilityFlagsToMesh();

    //경로의 Visibility 설정
    void    SetPathVisibilityWithGroupName(std::string group_name, bool bShow, bool bExclusive);
    void    SetPathVisibilityWithFloorID(int floorID, bool bShow, bool bExclusive);
    
private:
    //Configuration
    vbConfiguration     m_AppConfiguration;     //App. Configuration information
    bool                m_bIsInitiated;
    
    //Mesh file
    int                 m_mesh_path_count;
    char*               m_mesh_path;
    int                 m_texture_path_count;
    char*               m_texture_path;
    
 	//Mesh
 	vbMeshManager       m_MeshManager;          //Mesh manager
    //Material          
    vbMaterialManager   m_MatlManger;           //Material manager
    
    //View information
    vbViewInfo          m_view_info;            //View info.
    
    //Path
    vbPathManager       m_PathManager;         //Path
    
    IRenderingEngine*   m_renderingEngine;
    
    //Mesh Instance
    vbInstanceManager   m_InstanceManager;
    
    //Polygon
    vbPolygon*          m_pCurrentPolygon;
    vbPolyManager       m_PolyManager;
    
    //Group
    vbMeshGroupManager  m_GroupManager;
    
    //
    bool                m_bUseLocalTexture;
    char                m_bSupportDeviceDirection[7];
    bool                m_bDebugLog;
    bool                m_bPickingMode;
	
	

};
    
IApplicationEngine* CreateApplicationEngine(IRenderingEngine* renderingEngine)
{
    return new ApplicationEngine(renderingEngine);
}

ApplicationEngine::ApplicationEngine(IRenderingEngine* renderingEngine) :
    m_renderingEngine(renderingEngine)
{
    //Set View info
    m_renderingEngine->SetViewInfo(&m_view_info);
    //Set Configuration
    m_renderingEngine->SetConfiguration(&m_AppConfiguration);
    //Set MeshManager
    m_renderingEngine->SetMeshManager(&m_MeshManager);
    //Set MaterialManager
    m_renderingEngine->SetMaterialManager(&m_MatlManger);
    //Set PathManager
    m_renderingEngine->SetPathManager(&m_PathManager);
   
    m_mesh_path_count = 0;
    m_mesh_path = NULL;
    m_texture_path_count = 0;
    m_texture_path = NULL;
    
    m_bIsInitiated = false;
    m_bDebugLog = false;
    
    m_bUseLocalTexture = true;
    
    memset(m_bSupportDeviceDirection,0,sizeof(char)*7);
    m_bSupportDeviceDirection[1]=1;//Portrait만 지원하도록 기본 설정함.
    
    m_bPickingMode = false;
    
    m_pCurrentPolygon = NULL;
    m_PolyManager.Clear();
    
    m_GroupManager.Clear();
}

ApplicationEngine::~ApplicationEngine()
{
    m_renderingEngine->SetViewInfo(NULL);
    m_renderingEngine->SetConfiguration(NULL);

    if(m_mesh_path) delete [] m_mesh_path;
    m_mesh_path = NULL;
    
    if(m_texture_path) delete [] m_texture_path;
    m_texture_path = NULL;
    
    this->ClearData();
    
    m_renderingEngine->DeleteBaseData();
    
    
}

void ApplicationEngine::SetMeshPath(int length_with_null, const char* path)
{
    if(m_mesh_path) delete [] m_mesh_path;
    m_mesh_path = new char[length_with_null];
    memcpy(m_mesh_path, path, sizeof(char)*length_with_null);
    m_mesh_path_count = length_with_null;
}

void ApplicationEngine::SetTexturePath(int length_with_null_and_slash, const char* path)
{
    if(m_texture_path) delete [] m_texture_path;
    m_texture_path = new char[length_with_null_and_slash];
    memcpy(m_texture_path, path, sizeof(char)*length_with_null_and_slash);
    //Add slash
    m_texture_path[length_with_null_and_slash-2]='/';
    m_texture_path[length_with_null_and_slash-1]=0;     //설정하지 않으면 이상한 문자들이 끼어들 수 있음.
    
    m_texture_path_count = length_with_null_and_slash;
    m_renderingEngine->SetTextureBasePath(string(m_texture_path));
}

int  ApplicationEngine::SetPathDataWithGroupName(std::string group_name, float* pts, unsigned short ptNum, float total_dist, int mapIndex, bool bArrowTip)
{
    if (pts==NULL && ptNum==0 && total_dist<0.f)
    {
        m_PathManager.Clean();
        return 0;
    }
 
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0 || total_dist<0.f)
    {
        std::cout << "Abnormal path data input.\r\n";
        return -1;
    }
    
    if (ptNum<2)
    {
        std::cout << "Lack of path data.\r\n";
        return -1;
    }
#endif
    
    m_PathManager.Clean();
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //Y(좌표 변환이 된 상태로 들어와야 함.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*3+0]);   //X
        path_3d_coords.push_back(pts[i*3+1]);   //Y
        path_3d_coords.push_back(pts[i*3+2]);   //Z
    }
    
    vbPath* newPath = m_PathManager.CreatePath();
    
    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(total_dist);
    int result = newPath->BuildPathRoadVts(false,bArrowTip);
    newPath->SetPathID(1);
    newPath->SetMapID(mapIndex);
    newPath->SetGroupName(group_name);
    m_PathManager.SetVisiblePath(1);
    
    return result;//음수면 에러다.
   
}

int  ApplicationEngine::SetPathDataWithFloorID(int floorID, float* pts, unsigned short ptNum, float total_dist, int mapIndex, bool bArrowTip)
{
    if (pts==NULL && ptNum==0 && total_dist<0.f)
    {
        m_PathManager.Clean();
        return 0;
    }
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0 || total_dist<0.f)
    {
        std::cout << "Abnormal path data input.\r\n";
        return -1;
    }
    
    if (ptNum<2)
    {
        std::cout << "Lack of path data.\r\n";
        return -1;
    }
#endif
    
    m_PathManager.Clean();
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //Y(좌표 변환이 된 상태로 들어와야 함.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*3+0]);   //X
        path_3d_coords.push_back(pts[i*3+1]);   //Y
        path_3d_coords.push_back(pts[i*3+2]);   //Z
    }
    
    vbPath* newPath = m_PathManager.CreatePath();
    
    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(total_dist);
    int result = newPath->BuildPathRoadVts(false,bArrowTip);
    newPath->SetPathID(1);
    newPath->SetMapID(mapIndex);
    newPath->SetFloorID(floorID);
    m_PathManager.SetVisiblePath(1);
    
    return result;//음수면 에러다.
   
}


//기존 데이터 다 삭제하고 하나만 만든다. - 변환 된 3차원 좌표로 받는다.
int ApplicationEngine::SetPathData(float* pts, unsigned short ptNum, float total_dist, int mapIndex, bool bArrowTip)
{
    if (pts==NULL && ptNum==0 && total_dist<0.f)
    {
        m_PathManager.Clean();
        return 0;
    }
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0 || total_dist<0.f)
    {
        std::cout << "Abnormal path data input.\r\n";
        return -1;
    }
    
    if (ptNum<2)
    {
        std::cout << "Lack of path data.\r\n";
        return -1;
    }
#endif
    
    m_PathManager.Clean();
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //Y(좌표 변환이 된 상태로 들어와야 함.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*3+0]);   //X
        path_3d_coords.push_back(pts[i*3+1]);   //Y
        path_3d_coords.push_back(pts[i*3+2]);   //Z
    }
    
    vbPath* newPath = m_PathManager.CreatePath();

    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(total_dist);
    int result = newPath->BuildPathRoadVts(false,bArrowTip);
    newPath->SetPathID(1);
    newPath->SetMapID(mapIndex);
    m_PathManager.SetVisiblePath(1);
    
    return result;//음수면 에러다.
 }

//기존 데이터 다 삭제하고 하나만 만든다. - 입력 되는 좌표의 y좌표는 -z이다.
int ApplicationEngine::SetPathData2D(float* pts, int* floors, unsigned short ptNum, bool bArrowTip)
{
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0)
    {
        std::cout << "*************** Abnormal path data input. ******************\r\n";
        return -1;
    }
    
    if (ptNum<2)
    {
        std::cout << "*************** Lack of Path data ******************\r\n";
        return -1;
    }
#endif
    
    m_PathManager.Clean();
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //좌표 변환 안된 상태로 들어오므로, 여기서 변환한다.
    
    ///////////////////////////////////// 전체 경로 /////////////////////////////////
    //우선 전체 경로를 하나 만들고, 층별로 보여줄 경로는 따로 만들어준다.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*2+0]);           //X - 그대로 입력
        path_3d_coords.push_back(0.f);                  //Y - 전체 경로는 바닥에 있다고 가정
        path_3d_coords.push_back(-pts[i*2+1]);          //Z
    }
    
    //Create and Add
    vbPath* newPath = m_PathManager.CreatePath();
    
    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(0);
    int result = newPath->BuildPathRoadVts(true, bArrowTip);//전체 경로는 두껍게 만든다.
    newPath->SetPathID(0);  //다 지우고 새로 만들기 때문에 첫번째 데이터의 아이디인 0이 된다.
    newPath->SetFloorID(0);
    newPath = NULL;

    ///////////////////////////////////// 층별로 분리 된 경로 제작
    path_3d_coords.clear();
    int curr_floor = floors[0];

    for (int i=0; i<ptNum; i++)
    {
        //같은 층이면 점 추가
        if(floors[i]==curr_floor)
        {
            path_3d_coords.push_back(pts[i*2+0]);           //X - 그대로 입력
            path_3d_coords.push_back(0.f);                  //Y - 전체 경로는 바닥에 있다고 가정
            path_3d_coords.push_back(-pts[i*2+1]);          //Z
        }
        else
        {//다른 층으로 바뀌었으면 그 동안 추가 된 데이터로 경로를 완성한다.
            if(path_3d_coords.size()>0)
            {
                //그 동안 추가 된 데이터의 개수가 2개 이상일때만 경로를 만든다.
                if (path_3d_coords.size()>=2)
                {
                    newPath = m_PathManager.CreatePath();
                    newPath->SetPathID((int)m_PathManager.GetPathCount()-1);  //다 지우고 새로 만들기 때문에 첫번째 데이터의 아이디인 0이 된다.
                    newPath->SetFloorID(curr_floor);
                    newPath->SetData(path_3d_coords);
                    newPath->SetWholeCmDistance(0);
                    newPath->BuildPathRoadVts(false, bArrowTip);
                    newPath->SetGroundOffset(150.f);
                }

                
                newPath = NULL;
                path_3d_coords.clear();
                curr_floor = floors[i];
                path_3d_coords.push_back(pts[i*2+0]);           //X - 그대로 입력
                path_3d_coords.push_back(0.f);                  //Y - 전체 경로는 바닥에 있다고 가정
                path_3d_coords.push_back(-pts[i*2+1]);          //Z
            }
        }
    }
    
    if(path_3d_coords.size()>=2)
    {
        newPath = m_PathManager.CreatePath();
        newPath->SetPathID((int)m_PathManager.GetPathCount()-1);  //다 지우고 새로 만들기 때문에 첫번째 데이터의 아이디인 0이 된다.
        newPath->SetFloorID(curr_floor);
        newPath->SetData(path_3d_coords);
        newPath->SetWholeCmDistance(0);
        newPath->BuildPathRoadVts(false,bArrowTip);
        newPath->SetGroundOffset(200.f);
    }
    
    path_3d_coords.clear();
    return result;
}


vec3 ApplicationEngine::GetOrbitCenter()
{
    return m_view_info.GetOrbitCenter();
}

int  ApplicationEngine::AddPathDataWithGroupName(std::string group_name, float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip, unsigned short nMode, const float* param) /*Mode 0=Solid, 1=Triangles*/
{
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0)
    {
        std::cout << "Abnormal path data input.\r\n";
        return -1;
    }
    
    if (ptNum<2)
    {
        std::cout << "-------------- Lack of path data ----------------.\r\n";
        return -2;
    }
    std::cout << "AddPathWithGroupName - GroupName = " << group_name << std::endl;
#endif
    
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //Y(좌표 변환이 된 상태로 들어와야 함.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*3+0]);   //X
        path_3d_coords.push_back(pts[i*3+1]);   //Y
        path_3d_coords.push_back(pts[i*3+2]);   //Z
        
#ifdef VB_CORE_DEBUG
        std::cout << "PtIdx[[" << i << "] = [ " << pts[i*3+0] << " , "  << pts[i*3+1] << " , "  << pts[i*3+2] << std::endl;
#endif
    }
    
    vbPath* newPath = m_PathManager.CreatePath();
    int pathID = m_PathManager.GetPathCount();
    newPath->SetRoadWidth(fPathWidth);
    
    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(0.f);
    newPath->BuildPathRoadVts(false, bArrowTip, nMode, param);
    newPath->SetPathID(pathID);
    newPath->SetColorRGBA(fRGBA[0], fRGBA[1], fRGBA[2], fRGBA[3]);
    newPath->SetGroupName(group_name);
    
    return pathID;
}
int  ApplicationEngine::AddPathDataWithFloorID(int floorID, float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip, unsigned short nMode, const float* param) /*Mode 0=Solid, 1=Triangles*/
{
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0)
    {
        std::cout << "Abnormal path data input.\r\n";
        return -1;
    }
    if (ptNum<2)
    {
        std::cout << "-------------- Lack of path data ----------------.\r\n";
        return -2;
    }
    std::cout << "AddPathWithFloorID - FloorID = " << floorID << std::endl;
#endif
    
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //Y(좌표 변환이 된 상태로 들어와야 함.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*3+0]);   //X
        path_3d_coords.push_back(pts[i*3+1]);   //Y
        path_3d_coords.push_back(pts[i*3+2]);   //Z

#ifdef VB_CORE_DEBUG
        std::cout << "PtIdx[" << i << "] = [ " << pts[i*3+0] << " , "  << pts[i*3+1] << " , "  << pts[i*3+2] << std::endl;
#endif
    }
    
    vbPath* newPath = m_PathManager.CreatePath();
    int pathID = m_PathManager.GetPathCount();
    newPath->SetRoadWidth(fPathWidth);
    
    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(0.f);
    newPath->BuildPathRoadVts(false, bArrowTip, nMode, param);
    newPath->SetPathID(pathID);
    newPath->SetColorRGBA(fRGBA[0], fRGBA[1], fRGBA[2], fRGBA[3]);
    
    newPath->SetFloorID(floorID);
    return pathID;
}

int ApplicationEngine::AddPathData(float* pts, unsigned short ptNum, float* fRGBA, float fPathWidth, bool bArrowTip, unsigned short nMode, const float* param)
{
#ifdef VB_CORE_DEBUG
    if (pts==NULL || ptNum==0)
    {
        std::cout << "Abnormal path data input.\r\n";
        return -1;
    }
    
    if (ptNum<2)
    {
        std::cout << "-------------- Lack of path data ----------------.\r\n";
        return -2;
    }
#endif
    vector<float>   path_3d_coords;
    path_3d_coords.clear();
    
    //Y(좌표 변환이 된 상태로 들어와야 함.
    for (int i=0; i<ptNum; i++)
    {
        path_3d_coords.push_back(pts[i*3+0]);   //X
        path_3d_coords.push_back(pts[i*3+1]);   //Y
        path_3d_coords.push_back(pts[i*3+2]);   //Z
#ifdef VB_CORE_DEBUG
        std::cout << "Path[" << i << "] = [ " << pts[i*3+0] << " , "  << pts[i*3+1] << " , "  << pts[i*3+2] << std::endl;
#endif
        
    }
    
    vbPath* newPath = m_PathManager.CreatePath();
    int pathID = m_PathManager.GetPathCount();
    newPath->SetRoadWidth(fPathWidth);
    
    newPath->SetData(path_3d_coords);
    newPath->SetWholeCmDistance(0.f);
    newPath->BuildPathRoadVts(false, bArrowTip, nMode, param);
    newPath->SetPathID(pathID);
    newPath->SetColorRGBA(fRGBA[0], fRGBA[1], fRGBA[2], fRGBA[3]);
    return pathID;
}

//void ApplicationEngine::showPath(int pathID)
void ApplicationEngine::showPath(bool bShowPath)
{
//    m_PathManager.SetVisiblePath(pathID);
    m_PathManager.SetPathVisibility(bShowPath);
}

void ApplicationEngine::showPathAtFloor(int nFloor)
{
    m_PathManager.SetVisiblePathAtFloor(nFloor);
}

bool ApplicationEngine::IsViewUpdated()
{
    return m_view_info.IsViewUpdated();
}

void ApplicationEngine::SetViewUpdated(bool bUpdated)
{
    m_view_info.SetViewUpdated(bUpdated);
}

void ApplicationEngine::Initialize(int width, int height)
{
    //std::cout << "Width = " << width << ",  Height = " << height << "\r\n";
    
    //View Init with Configuration
    m_view_info.InitializeViewInfo(width, height, &m_AppConfiguration);
    
    //Init Renderer
    m_renderingEngine->SetMeshManager(&m_MeshManager);
    m_renderingEngine->Initialize();
    
    m_renderingEngine->SetUseLocalTexture(m_bUseLocalTexture);
    
    m_bIsInitiated = true;

}

void ApplicationEngine::ClearData(bool bClearRender, bool bClearMesh, bool bClearMaterial, bool bClearView, bool bClearPath)
{
    //Rendering Engine
    if(bClearRender){
     m_renderingEngine->ClearData();
        bClearRender=false;
    }

    //Mesh
    if(bClearMesh){
        m_MeshManager.ClearData();
        m_InstanceManager.Clear();
        bClearMesh=false;
    }
    
    //Material
    if(bClearMaterial){
          m_MatlManger.DeleteAllMaterials();
        bClearMaterial=false;
    }
    
    //View
    if(bClearView){
        m_view_info.ReloadConfiguration();
        bClearView=false;
    }
    
    //Path
    if(bClearPath){
        m_PathManager.Clean();
        bClearPath=false;
    }
    
    m_PolyManager.Clear();
    m_GroupManager.Clear();
    
    m_pCurrentPolygon = NULL;

}

VBMapErrorCode  ApplicationEngine::ImportInstance(const char* path)
{
#ifdef VB_CORE_DEBUG
    std::cout << "Instance path = " << path << std::endl;
#endif
    FILE* fp = fopen(path, "r");
    if (fp==NULL)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "Instance file pointer is NULL" << std::endl;
#endif
        return VBMapError_MAP_FILE_OPEN_ERROR;
    }
    
    VBMapErrorCode result = vbInstanceFactory::ImportMeshInstanceFromIST(&m_InstanceManager, &m_MeshManager, fp);
    
    fclose(fp);
    
    return result;
}

int  ApplicationEngine::AddInstance(const char* sbm_name_without_extension, unsigned short mesh_id_in_sbm, const float* transform_mat)
{
    int new_inst_id = vbInstanceFactory::AddMeshInstance(sbm_name_without_extension, mesh_id_in_sbm, transform_mat, &m_InstanceManager, &m_MeshManager);
    
    return new_inst_id;
}

VBMapErrorCode  ApplicationEngine::LoadMesh(bool bAsStatic)
{

    //Check mesh path
    if(!m_mesh_path)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "Mesh path isn't defined.\r\n";
#endif
        return VBMapError_MAP_PATH_UNDEFINED;
    }
    
    //Mesh file check
    FILE* fp = fopen(m_mesh_path, "rb");
    if(!fp)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "Mesh file[" << m_mesh_path << "] isn't available.\r\n";
#endif
        return VBMapError_MAP_FILE_OPEN_ERROR;
    }
    
    //format name
    char    format_name[8];
    fread(format_name,sizeof(char),8,fp);
    
    //version
    unsigned char   version = 0;
    fread(&version,sizeof(unsigned char),1,fp);
    
    switch(version)
    {
        case 1:
        case 2:
        {
            //MESH HEADER
            vbSBMHeader_V2	header;

            fread(&header.material_count,sizeof(unsigned short),1,fp);
            fread(&header.mesh_count,sizeof(unsigned short),1,fp);
            
            
            //Material
            vbMaterialFactory::ImportMaterialFromSBMv2(&m_MatlManger, fp, header.material_count, (char*)m_mesh_path);
            
            //Mesh
            vbMeshFactory::ImportMeshFromSBMv1(&m_MeshManager, &m_MatlManger,fp, header.mesh_count, bAsStatic, ( char*)m_mesh_path, &m_GroupManager);
            
            //Release useless texcoord
            m_MeshManager.FreeUnusedTexCoord(&m_MatlManger);//텍스쳐 재질이 아닌 경우에도 텍스쳐 좌표가 할당된 모델이 있어, 이를 반환한다.
        }
        break;
            
        case 4:
        {
            //Header
            vbSBMHeader_V4_2  header_v4;  //4.2
            fread(&header_v4,sizeof(vbSBMHeader_V4_2),1,fp);  // sizeof(vbSBMHeader_V4) = 32
            
        }
        break;
       
        case 3:// 폐기 된 버젼
        default:
        {
#ifdef VB_CORE_DEBUG
            cout << "This version is " << version << ". But it's not supported.\r\n";
#endif
            fclose(fp);
            return VBMapError_UNSUPORRTED_VERSION;
        }
        break;
    }
    fclose(fp);
    
    //Fit BB
    m_GroupManager.UpdateBB();
    
    //Mesh managerdml BB는 사용하지 않도록 수정 해 나가야 함.
    m_MeshManager.UpdateAABB(false);    //Mesh의 BB는 최신이므로 전체 BB만 계산하면 된다.
    
    //Check Environment Map --- 이후에는 m_view_info 멤버로 이동시켜야 함.
    vec3 minBound, maxBound;
    minBound = m_MeshManager.GetMinBound();
    maxBound = m_MeshManager.GetMaxBound();
     m_view_info.UpdateValidRange(minBound, maxBound);
   

    //Fit Whole Data with AABB
    m_view_info.SetYawDegree(0.f);
    m_view_info.SetCameraPitchRatio(0.01f);
    m_view_info.FitAABBtoScreen();
    
    
    //Update Near-Far clip
    m_view_info.UpdateNearFarClip();
    m_view_info.UpdateViewMatrix();
  
    //Load Texture

    m_renderingEngine->LoadTextures();
    
#ifdef VB_CORE_DEBUG
    //m_MatlManger.ShowInfo();    //텍스쳐 중복 처리 전후 결과를 확인 - VBO말고 그냥 메모리써도 될랑가???
#endif
    
    m_renderingEngine->MeshToVBO();//사용하는 것으로 설정되어 있다면 변환한다.
    
    return VBMapError_NO_ERROR;

}

void ApplicationEngine::Render() const
{
    m_renderingEngine->Render();
}

void ApplicationEngine::UpdateAnimation(float dt)
{
    if(!m_view_info.IsOnAnimating()) return;
    
    //Animation 중
    m_view_info.UpdateAnimationFrame();
 }

void ApplicationEngine::InvalidateManipulation()
{
    m_view_info.InvalidateManipulation();
}

void ApplicationEngine::OnSingleFingerUp(int locationX, int locationY, int tapCount)
{
    if(m_view_info.IsOnAnimating()) return;
    
    vec3 pos3d;
    if(m_view_info.SingleFingerUp(ivec2(locationX,locationY), tapCount,pos3d))
    {
        switch (m_view_info.GetNavigationMode())
        {
            case vbViewInfo::NAVIGATION_ME:
            {
                m_view_info.RecoverNavigationMode();
            }
            break;
            default:    break;
        }
    }
}

void ApplicationEngine::OnSingleFingerDown(int locationX, int locationY, int tapCount)
{
    if(m_view_info.IsOnAnimating()) return;
    
    m_view_info.SingleFingerDown(ivec2(locationX,locationY),tapCount);
}

void ApplicationEngine::SetMyPosition(float org_x, float org_y, float org_z,float corrected_x, float corrected_y, float corrected_z)
{
    vec3    mypos_original(org_x,org_y,org_z);
    vec3    mypos_corrected(corrected_x,corrected_y,corrected_z);
    m_PathManager.SetMyPos(mypos_original, mypos_corrected,m_view_info.GetCameraPosition());
}

void ApplicationEngine::SetMyPosDiscState(bool bShow, const float* rgba, const float* pos, float rad)
{
    m_renderingEngine->SetMyPosDiscState(bShow, rgba, pos, rad);
}


void ApplicationEngine::ShowMyPosition(bool bShowMyPos)
{
    m_renderingEngine->ShowMyPosDisc(bShowMyPos);
    //m_PathManager.ShowMyPos(bShowMyPos);
}

bool ApplicationEngine::IsOnHRotatingAnimation()
{
    if(m_view_info.IsOnAnimating() && 
    m_view_info.GetAniMode() == vbViewInfo::CAMERA_ANI_YAW_ROTATE)
        return true;
    
    return false;
    
}

void ApplicationEngine::OnSingleFingerMove(int locationX, int locationY, int tapCount)
{
  if(m_view_info.IsOnAnimating()) return;
    /*
    if(m_view_info.IsOnAnimating())
    {
        //수평 회전 애니메이션 중에 터치면 중단.
        if (m_view_info.GetAniMode() == vbViewInfo::CAMERA_ANI_YAW_ROTATE)
        {
            m_view_info.EndHRotationAnimation();
        }
        return;
    }
     */
    
    m_view_info.SingleFingerMove(ivec2(locationX,locationY),tapCount);
}

void ApplicationEngine::OnDoubleFingerUp(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2)
{
    m_view_info.DoubleFingerUp(ivec2(pos1X,pos1Y),tapCount1, ivec2(pos2X,pos2Y), tapCount2);
}

void ApplicationEngine::OnDoubleFingerDown(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2)
{
    if(m_view_info.IsOnAnimating()) return;

    m_view_info.DoubleFingerDown(ivec2(pos1X,pos1Y),tapCount1, ivec2(pos2X,pos2Y), tapCount2);
    
}

void ApplicationEngine::OnDoubleFingerMove(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2)
{
    if(m_view_info.IsOnAnimating()) return;

    m_view_info.DoubleFingerMove(ivec2(pos1X,pos1Y),tapCount1, ivec2(pos2X,pos2Y), tapCount2);
}


vec3 ApplicationEngine::GetGroundPositionOnScreenXY(ivec2 pos)
{
   return m_view_info.GetRayIntersectionWithGround(pos);
}


void ApplicationEngine::SetCameraToSee3DPoint(float newCenterX,float newCenterY,float newCenterZ)
{
    m_view_info.JumpPanning(vec3(newCenterX,newCenterY,newCenterZ));
}

vec3 ApplicationEngine::GetCameraPosition()
{
    return m_view_info.GetCameraPosition();
}

mat4* ApplicationEngine::GetViewProjectionMatrix()
{
    return m_view_info.GetViewProjectionMatrix();
}

ivec2 ApplicationEngine::GetScreenSize()
{
    return m_view_info.GetScreenSize();
}

void ApplicationEngine::SetCameraPitchRatio(float pitch_ratio)
{
    m_view_info.SetCameraPitchRatio(pitch_ratio);
}


void ApplicationEngine::SetCamera(float cx, float cy, float cz,
                                    float yaw_deg_from_nZ, float pitch_deg_from_xz_plane,
                                    float fDist)
{
    m_view_info.SetCamera(cx, cy, cz, yaw_deg_from_nZ, pitch_deg_from_xz_plane, fDist);
    
}

void ApplicationEngine::SetOrthoCameraWithPosOffset(VBOrthoCameraDirection mode, vec3 pos, float offset)
{
    Quaternion rot;
    
    switch (mode)
    {
        case VBOrthoCameraDirectionNegativeX:
        {
            rot = Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), vec3(1.f,0.f,0.f));
       }
            break;
        case VBOrthoCameraDirectionPositiveX:
        {
            rot = Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), vec3(-1.f,0.f,0.f));
            
        }
            break;
        case VBOrthoCameraDirectionNegativeY:
        {
            rot = Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), vec3(0.f,1.f,0.f));
        }
            break;
        case VBOrthoCameraDirectionPositiveY:
        {
            rot = Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), vec3(0.f,-1.f,0.f));
        }
            break;
        default:
        {
 #ifdef VB_CORE_DEBUG
            std::cout << "****************** Undefined Orthocamera Direction ************\r\n";
#endif
        }
            break;
    }
    
    m_view_info.SetOrientation(rot);
    m_view_info.SetOrbitCenter(pos);
    m_view_info.SetCameraDistance(offset);
    
    m_view_info.UpdateCameraPositionFromOrbit();
    m_view_info.UpdateViewMatrix();
    
}

vbPath*  ApplicationEngine::UpdateVisiblePath(float px2D, float py2D, int nFloor)
{
    //Check path
    vbPath* pTargetPath = m_PathManager.GetNearestPathAtFloor(px2D, py2D, nFloor); //nFloor=0 - Whole path
    if(!pTargetPath)
    {
#ifdef VB_CORE_DEBUG
        std::cout << "*** Invalid path requested. - No path is near (" << px2D << ", " << py2D << ")" << " at floor " << nFloor << " ************\r\n";
#endif
        return NULL;
    }
    
    //보이는 경로로 설정
    m_PathManager.SetVisiblepath(pTargetPath);
    
    //진행 상태와 방향을 설정함.
    pTargetPath->SetProgress(vec3(px2D,0.f,-py2D));
    
    return pTargetPath;
}

float ApplicationEngine::GetPitchRatio()
{
    return  m_view_info.GetPitchRatio();
}

float ApplicationEngine::GetPitchDegree()
{
    return  m_view_info.GetPitchDegree();
}

float ApplicationEngine::GetYawDegree()
{
    return  m_view_info.GetYawDegree();
}

void  ApplicationEngine::GetCemeraParam(float* params)//Yaw - Pitch - PosX - PosY - PosZ - Offset
{
    params[0] = m_view_info.GetYawDegree();
    params[1] = m_view_info.GetPitchDegree();
    
    vec3 cntr = m_view_info.GetOrbitCenter();
    params[2] = cntr.x;
    params[3] = cntr.y;
    params[4] = cntr.z;
    
    params[5] = m_view_info.GetCameraDistance();
    
}

void ApplicationEngine::SetHRotationAnimation(bool bStart, float fYawStep)
{
    
    if (bStart) {
        m_view_info.StartHRotationAnimation(fYawStep);
    }
    else
        m_view_info.EndHRotationAnimation();
    
}

void ApplicationEngine::BrokeControl(bool bBrokePan, bool bBrokeZoom, bool bBrokeRotate)
{
    m_view_info.BrokeControl(bBrokePan, bBrokeZoom, bBrokeRotate);
}


void ApplicationEngine::FitAABBtoScreen(bool bUpdateAABB/*false*/, bool bBoundMinMaxDist/*false*/)
{
    
    if(bUpdateAABB) m_MeshManager.UpdateAABB(false);    //Mesh의 BB는 달라지지 않으므로 BB에 대해서만 갱신
    
    //현재 카메라의 설정 상태에서 Fitting한다.
    m_view_info.FitAABBtoScreen(); //카메라가 지면을 향하고 있지 않다면 오류가 생길 수도 있다.
    
    
    //Update Near-Far clip
    m_view_info.UpdateNearFarClip();
    m_view_info.UpdateViewMatrix();
}

void ApplicationEngine::FitAABB2VisibleGroup(bool bBoundMinMaxDist/*false*/)
{
    m_GroupManager.UpdateBB();
//    m_GroupManager.ShowInfo();
    
    
    vec3 minBB = m_GroupManager.GetMinVisibleBB();
    vec3 maxBB = m_GroupManager.GetMaxVisibleBB();
    vec3 tmp = minBB;
    
    vector<vec3> vertices;
    vertices.push_back(minBB);
    vertices.push_back(maxBB);
    tmp.x = maxBB.x;  vertices.push_back(tmp);
    tmp.y = maxBB.y;  vertices.push_back(tmp);
    tmp.x = minBB.x;  vertices.push_back(tmp);
    tmp.z = maxBB.z;  vertices.push_back(tmp);
    tmp.y = minBB.y;  vertices.push_back(tmp);
    tmp.x = maxBB.x;  vertices.push_back(tmp);
    

    m_view_info.FitAABBtoVertices(vertices, bBoundMinMaxDist);
    
    //Update Near-Far clip
    m_view_info.UpdateNearFarClip();
    m_view_info.UpdateViewMatrix();
}

void ApplicationEngine::SetCameraPostion2VisibleGroupCenter(float fCameraDistance)
{
    m_GroupManager.UpdateBB();
    vec3 cntr = ( m_GroupManager.GetMinVisibleBB()+m_GroupManager.GetMaxVisibleBB() ) / 2;
    cntr.y = 0.f;
    
#ifdef VB_CORE_DEBUG
//    m_GroupManager.ShowInfo();
    printf("[NEW CENTER] = %f %f %f\r\n", cntr.x, cntr.y, cntr.z);
#endif

    m_view_info.SetCameraDistance(fCameraDistance);
    SetCameraToSee3DPoint(cntr.x, 0.f, cntr.z);  //animation 이동
}


void ApplicationEngine::SetGroundLevel(float fGoundLevel)
{

    m_renderingEngine->SetGroundLevel(fGoundLevel);
}


//위치 변경은 사전에 미리 해둬야 함.
bool ApplicationEngine::SetCameraOnPath(float px2D, float py2D, int nFloor, int nIndexOfFromVt, float fProgress, float fPitchRatio, float fDistance, bool bFitDirection)
{
    vbPath* pTargetPath = UpdateVisiblePath(px2D, py2D, nFloor);
    if(pTargetPath==NULL) return false;
    
    //Check direction
    vector<vec3>* pVts = pTargetPath->GetPathVts();
    if(pVts->size()<2 || pVts->size()-2<nIndexOfFromVt)//100개면 마지막이 99번, 즉 적어도 98이하라야 한다.
    {
#ifdef VB_CORE_DEBUG
        std::cout << "**************** Invalid path requested. (Lack of vt) ***********\r\n";
#endif
        return false;
    }
    
    //Get Direction
    vec3 dir = pVts->at(nIndexOfFromVt+1) - pVts->at(nIndexOfFromVt);
    dir.y = 0;
    dir.Normalize();
    
    //수평 회전 성분
    Quaternion orbit_h = Quaternion::CreateFromVectors(VEC3_UNITZ * (-1.f),dir);
    orbit_h.Normalize();
    
    //////////////////////////////////위로 올릴 쿼터니언.
    float valid_pitch_ratio  = fPitchRatio;
    if(valid_pitch_ratio>1.f)           valid_pitch_ratio = 0.99f;
    else if(valid_pitch_ratio<0.01f)    valid_pitch_ratio = 0.01f;
    m_view_info.SetCameraPitchRatio(valid_pitch_ratio);
   
    float pitch_angle = m_AppConfiguration.fPitchLimitMin + (m_AppConfiguration.fPitchLimitMax - m_AppConfiguration.fPitchLimitMin) * valid_pitch_ratio;
    
    
    /*
    vec3 pitch_dir;
    pitch_dir.x = 0.f;
    pitch_dir.y = sin(DEG2RAD(pitch_angle));
    pitch_dir.z = -cos(DEG2RAD(pitch_angle));
    Quaternion orbit_v = Quaternion::CreateFromVectors(VEC3_UNITZ * (-1.f),pitch_dir);
    orbit_v.Normalize();
     */
    
    Quaternion orbit_v = Quaternion::CreateFromNegativeZtoPitchDegree(pitch_angle);
    
    
    Quaternion cam_dir;
    if(bFitDirection)
    {
        cam_dir = orbit_h.Rotated(orbit_v);
        m_view_info.SetYawDegree(dir);
    }
    else                cam_dir = orbit_v;
    
    //거리
    float valid_distance = fDistance;
    if(valid_distance<m_AppConfiguration.fCamMinDistance)         valid_distance = m_AppConfiguration.fCamMinDistance;
    
    //위치
    vec3 new_pos = pVts->at(nIndexOfFromVt) + (pVts->at(nIndexOfFromVt+1) - pVts->at(nIndexOfFromVt)) * fProgress;
    
    //////////////////////////////////// 파라미터 적용
    m_view_info.SetOrientation(cam_dir);
    m_view_info.SetCameraDistance(valid_distance);
    m_view_info.SetOrbitCenter(new_pos);
    
    m_view_info.UpdateCameraPositionFromOrbit();
    m_view_info.UpdateViewMatrix();
    
    return true;

}

void ApplicationEngine::UpdatePanBoundAsAABB()
{
    m_view_info.SetPanValidRange(m_MeshManager.GetMinBound(), m_MeshManager.GetMaxBound(), 1.f);
}

void ApplicationEngine::UpdatePanBound(vec3 min, vec3 max)
{
    m_view_info.SetPanValidRange(min, max, 1.f);
}

void  ApplicationEngine::SetCameraMinMaxOffset(float fMinOffset, float fMaxOffset)
{
    
#ifdef VB_CORE_DEBUG
    if (fMinOffset>fMaxOffset) {
        float new_max_offset = (m_MeshManager.GetMaxBound() - m_MeshManager.GetMinBound()).size() * 2.f;
        printf("[WATCH!!] Camera Min offset(%f) is larger than Max offset(%f). Max offset would be 2 times of Whole BB size as %f.\r\n",fMinOffset,fMaxOffset,new_max_offset);
        if (new_max_offset > fMinOffset)
            fMaxOffset = new_max_offset;
        else
            fMaxOffset = fMinOffset * 10.f;
    }

    //printf("[WATCH!!] Camera Min-Max = %f - %f\r\n",fMinOffset,fMaxOffset);


#endif
    
    m_view_info.SetCameraMinMaxDistance(fMinOffset, fMaxOffset);
}


//WONIL X-MAS
//위치 변경은 사전에 미리 해둬야 함.
bool ApplicationEngine::SetCameraOnPath(float px2D, float py2D, int nFloor, bool bFitDirection)
{
    vbPath* pTargetPath = UpdateVisiblePath(px2D, py2D, nFloor);
    if(pTargetPath==NULL) return false;
    //Progress 및 현재 방향이 확보되어 있음.
    
    if(m_view_info.IsNavigation())  return false;

    
    vec3 new_pos(px2D,0.f,-py2D);
    Quaternion cam_dir = m_view_info.GetOrientation();
    //Get Direction
    vec3    cam_vec = m_view_info.GetDirection();
    cam_vec.y = 0.f;
    vec3 dir = pTargetPath->GetCurrentYaw();
    dir.y = 0.f;
    
    //
    float angle = acos(cam_vec.Dot(dir) / (cam_vec.size() * dir.size())) * 180.f / 3.141592;

    if(bFitDirection && angle>30.f && angle < 150.f)
    {        
        //수평 회전 성분
        Quaternion orbit_h = Quaternion::CreateFromVectors(VEC3_UNITZ * (-1.f),dir);
        orbit_h.Normalize();
        
        //목표 방향을 Yaw로 설정한다.
        m_view_info.SetYawDegree(dir);
        
        //////////////////////////////////위로 올릴 쿼터니언.
        float pitch_angle = m_view_info.GetPitchDegree();
        
        /*
        vec3 pitch_dir;
        pitch_dir.x = 0.f;
        pitch_dir.y = sin(DEG2RAD(pitch_angle));
        pitch_dir.z = -cos(DEG2RAD(pitch_angle));
        Quaternion orbit_v = Quaternion::CreateFromVectors(VEC3_UNITZ * (-1.f),pitch_dir);
        orbit_v.Normalize();
         */
        
        Quaternion orbit_v = Quaternion::CreateFromNegativeZtoPitchDegree(pitch_angle);
         
        cam_dir = orbit_h.Rotated(orbit_v);
        cam_dir.Normalize();
    }
    
    //위치
    m_view_info.JumpPanRotation(new_pos, cam_dir);
    return true;
    
}

void ApplicationEngine::SetViewControlSet(vbViewInfo::vbViewControlSet eViewControlSet)
{
    m_view_info.SetViewControlSet(eViewControlSet);
}

void ApplicationEngine::SetHRotationTouchRange(float fVerticalRatioFromBottom)
{
    m_view_info.SetHRotationRatio(fVerticalRatioFromBottom);
}

void ApplicationEngine::SetDeviceDirection(DeviceDirection device_direction, bool bAnimate)
{
    m_view_info.SetViewTurnDirection((vbViewDirection)device_direction,bAnimate);
    
#ifdef VB_CORE_DEBUG
    std::cout << "Device direction = " << int(device_direction) << std::endl ;
#endif
}

//Runtime config
void ApplicationEngine::SetRuntimeConfig(RunTimeConfig* pConfig)
{
    //Background color
    memcpy(m_AppConfiguration.fBgColor, pConfig->fBackgroundColor4f, sizeof(float)*4);
    
    //use VBO - DO NOT MODIFY on RENDERING. IF YOU WANT TO CHANGE THIS, USE AS 'CLEAR-CHANGE-LOAD'
    m_AppConfiguration.bUseVBO = pConfig->bUseVBO;
    
    //Perspective param
    m_view_info.SetPerspectiveParameters(pConfig->fPerspectiveParam);
    
    //Rotation param
    m_AppConfiguration.fCamRotateAreaRatio = pConfig->fRotationAreaRatio;
    
    //Solid Color
    memcpy(m_AppConfiguration.fSolidColor,pConfig->fSolidColor4f,sizeof(float)*4);
    
    //Camera Distance Range
    m_AppConfiguration.fCamMinDistance = pConfig->fCamDistanceRange[0]; //Min Distance
    m_view_info.GetCameraControl_OrbitZoom()->SetDistanceMin(pConfig->fCamDistanceRange[0]);
    
    m_AppConfiguration.fCamMaxDistanceRatioToBBSize = pConfig->fCamDistanceRange[1];
    m_view_info.SetCameraMaxDistanceRatioBBSize(pConfig->fCamDistanceRange[1]);
    
    //Ambient color
    m_AppConfiguration.fLightAmbient.x = pConfig->fAmbientColor4f[0];
    m_AppConfiguration.fLightAmbient.y = pConfig->fAmbientColor4f[1];
    m_AppConfiguration.fLightAmbient.z = pConfig->fAmbientColor4f[2];
    m_AppConfiguration.fLightAmbient.w = pConfig->fAmbientColor4f[3];
    
    //Pitch range
    m_view_info.GetCameraTrackControl()->SetPitchMinDegree(pConfig->fPitchMinMax[0]);
    m_view_info.GetCameraTrackControl()->SetPitchMaxDegree(pConfig->fPitchMinMax[1]);

}

void ApplicationEngine::GetRuntimeConfig(RunTimeConfig* pConfig)
{
    //Background color
    memcpy(pConfig->fBackgroundColor4f, m_AppConfiguration.fBgColor, sizeof(float)*4);

    //use VBO
    pConfig->bUseVBO = m_AppConfiguration.bUseVBO;
    
    //Perspective param
    memcpy(pConfig->fPerspectiveParam, m_view_info.GetPerspectiveParameters(), sizeof(float)*4);

    //Rotation param
    pConfig->fRotationAreaRatio = m_AppConfiguration.fCamRotateAreaRatio;
    
    //Solid Color
    memcpy(pConfig->fSolidColor4f,m_AppConfiguration.fSolidColor,sizeof(float)*4);
    
    //Camera Distance Range
    pConfig->fCamDistanceRange[0] = m_AppConfiguration.fCamMinDistance; //Min Distance
    pConfig->fCamDistanceRange[1] = m_AppConfiguration.fCamMaxDistanceRatioToBBSize;
    
    //Ambient color
    memcpy(pConfig->fAmbientColor4f,m_AppConfiguration.fLightAmbient.Pointer(), sizeof(float)*4);

    //Pitch range
    pConfig->fPitchMinMax[0] = m_view_info.GetCameraTrackControl()->GetPitchMinDegree();
    pConfig->fPitchMinMax[1] = m_view_info.GetCameraTrackControl()->GetPitchMaxDegree();
    
}

void ApplicationEngine::SetSolidRendering(bool bOnSolidRendering)
{
    m_renderingEngine->SetSolidRendering(bOnSolidRendering);
}

void ApplicationEngine::SetRenderingMode(vbGLRenderMode render_mode)
{
    m_renderingEngine->SetRenderingMode(render_mode);
}


bool ApplicationEngine::GetPickedMeshID(int tx, int ty, unsigned short* mesh_ID)
{
    return m_renderingEngine->GetColorcodedMeshIDAt(tx, ty,mesh_ID);
}

void ApplicationEngine::SetPickingMode(bool bPickingOn)
{
    m_bPickingMode = bPickingOn;
}

void ApplicationEngine::AddPolygon(std::string name, unsigned char* rgba, vbRing* pNewRing)
{
    
}

VBMapErrorCode ApplicationEngine::AddHoleToLastPolygon(vbRing* pNewRing)
{
    //현재 폴리곤이 없으면 추가 할 수 없다.
    if(m_pCurrentPolygon==NULL) return VBMapError_NOT_READY;
    
    m_pCurrentPolygon->AddInner(pNewRing);
    return VBMapError_NO_ERROR;
}

void ApplicationEngine::ConvertPolygonToMesh()
{
    //현재 폴리곤이 없으면 변환 할 것이 없으므로 리턴
    if(m_pCurrentPolygon==NULL) return;
    
    //변환
    
    //폴리곤 삭제
    m_PolyManager.Clear();
    
    //변환이 완료 되면 다시 초기화 한다.
    m_pCurrentPolygon = NULL;
}

////////////////////////// Group
//Group
std::string ApplicationEngine::IsExistingGroupContaining(std::string group_name)
{
    //Group_name을 포함하고 있는 그룹이 있는지 검사.
    vbMeshGroup* pGroup = m_GroupManager.GetNamedGroupContaining(group_name);
    
    if (pGroup) {
        return pGroup->GetGroupName();
    }
    
    return std::string("");
}

//각 그룹의 Show/Hide 설정(단, 메쉬까지 적용을 하는 것을 별도임.) 그룹 이름이  nil이면 전체.
void    ApplicationEngine::SetVisibilityFlagWithGroupName(bool bVisible, std::string group_name)
{
    //Group_name을 포함하고 있는 그룹이 있는지 검사.
    if (group_name!=std::string(""))
    {
        vbMeshGroup* pGroup = m_GroupManager.GetNamedGroupContaining(group_name);
        
        if (pGroup) {
            pGroup->SetVisibility(bVisible);
        }
    }
    else
    {
        for (int i=0; i < m_GroupManager.GetGroups()->size(); i++)
        {
            m_GroupManager.GetGroups()->at(i)->SetVisibility(bVisible);
        }
    }

}

//Visibility flag를 각 메쉬에 적용한다.
void    ApplicationEngine::ApplyVisibilityFlagsToMesh()
{
    vbMeshGroup* pGroup = 0;
    for (int i=0; i < m_GroupManager.GetGroups()->size(); i++)
    {
        pGroup = m_GroupManager.GetGroups()->at(i);
        
        m_MeshManager.SetGroupVisibility(pGroup->IsVisible(), pGroup->GetGroupName());
        
        pGroup = 0;
    }
    
    //Visible BB는 갱신 될 수 있으므로.
    m_GroupManager.UpdateBB();
    
//    m_GroupManager.ShowInfo();
}

//경로의 Visibility 설정
void    ApplicationEngine::SetPathVisibilityWithGroupName(std::string group_name, bool bShow, bool bExclusive)
{
    m_PathManager.SetGroupPathVisibility(group_name, bShow, bExclusive);
}

void    ApplicationEngine::SetPathVisibilityWithFloorID(int floorID, bool bShow, bool bExclusive)
{
    m_PathManager.SetVisibilityWithFloorID(floorID, bShow, bExclusive);
}
