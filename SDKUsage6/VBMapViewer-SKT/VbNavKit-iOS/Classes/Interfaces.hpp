#pragma once
#include "Vector.hpp"
#include "Quaternion.hpp"
#include "vbViewInfo.hpp"
#include "vbConfiguration.hpp"
#include "vbMeshManager.hpp"
#include "vbMaterialManager.hpp"
#include "vbPOIManager.h"
#include "vbMe.h"
#include "vbPathManager.h"
#include "vbEnum.hpp"
#include "tess/vbPolygon.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

struct IApplicationEngine {
    //Destructor
    virtual ~IApplicationEngine() {}
    
    //Initialize
    //지정 된 화면 크기로 프레임 버퍼를 만들고, 각종 초기화를 한다.
    virtual void Initialize(int width, int height) = 0;
    //초기화가 되었는지 여부를 나타낸다. 초기화가 되어 있지 않을때 초기화를 하도록 한다.
    virtual bool IsInitialted() = 0;
    
    //Debug
    // 디버그용 로그를 남길지 여부를 결정한다.
    virtual void EnableDebug(bool bEnableDebug) = 0;
    
    //Mesh & Texture
    virtual void SetMeshPath(int legn, const char* path) = 0;
    virtual void SetTexturePath(int legn, const char* path) = 0;
    virtual VBMapErrorCode  LoadMesh(bool bAsStatic) = 0;            //Load and view control
    virtual VBMapErrorCode  ImportInstance(const char* path) = 0;
    virtual int  AddInstance(const char* sbm_name_without_extension, unsigned short mesh_id_in_sbm, const float* transform_mat) = 0;
    virtual void ClearData(bool bClearRender=true, bool bClearMesh=true, bool bClearMaterial=true, bool bClearView=true, bool bClearPath=true) = 0;
    
    //Rendering
    virtual void Render() const = 0;
    
    virtual void SetSolidRendering(bool bOnSolidRendering = true) = 0;
    virtual void SetRenderingMode(vbGLRenderMode eRenderMode) = 0;
    
    //Animation
    virtual void UpdateAnimation(float timeStep) = 0;
    
    //Touch
    virtual void InvalidateManipulation() = 0;
    virtual void OnSingleFingerUp(int posX, int posY, int tapCount) = 0;
    virtual void OnSingleFingerDown(int posX, int posY, int tapCount) = 0;
    virtual void OnSingleFingerMove(int newposX, int newposY, int tapCount) = 0;
    virtual void OnDoubleFingerUp(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2) = 0;
    virtual void OnDoubleFingerDown(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2) = 0;
    virtual void OnDoubleFingerMove(int pos1X, int pos1Y, int tapCount1, int pos2X, int pos2Y, int tapCount2) = 0;
    virtual void SetViewControlSet(vbViewInfo::vbViewControlSet eViewControlSet) = 0;
    
    //Camera
    virtual void SetCameraToSee3DPoint(float newCenterX,float newCenterY,float newCenterZ) = 0;
    virtual vec3 GetCameraPosition() = 0;
    virtual bool IsViewUpdated() = 0;
    virtual void SetViewUpdated(bool bUpdated) = 0;
    virtual void SetCameraPitchRatio(float pitch_ratio) = 0;
    virtual void SetCamera(float cx, float cy, float cz, float yaw_deg_from_nZ, float pitch_deg_from_xz_plane, float fDist) = 0;
    virtual void SetOrthoCameraWithPosOffset(VBOrthoCameraDirection mode, vec3 pos, float offset) = 0;
    virtual bool SetCameraOnPath(float px2D, float py2D, int nFloor, int nIndexOfFromVt, float fProgress, float fPitchRatio, float fDistance, bool bFitDir) = 0;
    virtual bool SetCameraOnPath(float px2D, float py2D, int nFloor, bool bFitDirection) = 0;
    
    virtual void SetHRotationTouchRange(float fVerticalRatioFromBottom) = 0;
    virtual vec3 GetOrbitCenter() = 0;
    
    virtual void SetHRotationAnimation(bool bStart, float fYawStep) = 0;//
    virtual bool IsOnHRotatingAnimation() = 0;
    
    virtual void BrokeControl(bool bBrokePan, bool bBrokeZoom, bool bBrokeRotate) = 0;
   
    virtual float GetPitchRatio() = 0;
    virtual float GetPitchDegree() = 0;
    virtual float GetYawDegree() = 0;
    virtual void  GetCemeraParam(float* params) = 0;//Yaw - Pitch - PosX - PosY - PosZ - Offset
    virtual void  SetCameraMinMaxOffset(float fMinOffset, float fMaxOffset) = 0;
    virtual vec3 GetGroundPositionOnScreenXY(ivec2 pos) = 0;
    
    virtual void FitAABBtoScreen(bool bUpdateAABB=false, bool bBoundMinMaxDist=false) = 0;
    virtual void FitAABB2VisibleGroup(bool bBoundMinMaxDist=false) = 0;
    virtual void SetCameraPostion2VisibleGroupCenter(float fCameraDistance) = 0;
    
    virtual void SetGroundLevel(float fGoundLevel) = 0;
    
    virtual void UpdatePanBoundAsAABB() = 0;
    virtual void UpdatePanBound(vec3 min, vec3 max) = 0;
    
    
    //Path
    virtual int  SetPathData(float* pts, unsigned short ptNum, float total_dist, int mapIndex=0, bool bArrowTip=0) = 0;
    virtual int  SetPathDataWithGroupName(std::string group_name, float* pts, unsigned short ptNum, float total_dist, int mapIndex=0, bool bArrowTip=0) = 0;
    virtual int  SetPathDataWithFloorID(int floorID, float* pts, unsigned short ptNum, float total_dist, int mapIndex=0, bool bArrowTip=0) = 0;

    virtual int  AddPathData(float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip=0, unsigned short nMode=0, const float* param = 0) = 0; /*Mode 0=Solid, 1=Triangles*/
    virtual int  AddPathDataWithGroupName(std::string group_name, float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip=0, unsigned short nMode=0, const float* param = 0) = 0; /*Mode 0=Solid, 1=Triangles*/
    virtual int  AddPathDataWithFloorID(int floorID, float* pts, unsigned short ptNum,  float* fRGBA, float fPathWidth, bool bArrowTip=0, unsigned short nMode=0, const float* param = 0) = 0; /*Mode 0=Solid, 1=Triangles*/
    
//    virtual void showPath(int pathID) = 0;
    virtual void showPath(bool bShowPath) = 0;
    virtual void showPathAtFloor(int nFloor) = 0;
    virtual vbPath*  UpdateVisiblePath(float px2D, float py2D, int nFloor) = 0;
    
    virtual int  SetPathData2D(float* pts, int* floors, unsigned short ptNum, bool bArrowTip=true) = 0;
    
    virtual void SetMyPosition(float org_x, float org_py, float org_pz,float corrected_x, float correctedpy, float corrected_pz) = 0;

    virtual void ShowMyPosition(bool bShowMyPos) = 0;
    
    virtual void SetMyPosDiscState(bool bShow, const float* rgba, const float* pos, float rad) = 0;
    
    
    //World2Window
    virtual mat4*  GetViewProjectionMatrix() = 0;
    virtual ivec2  GetScreenSize() = 0;
    
    //View orientation
    virtual void SetDeviceDirection(DeviceDirection device_direction, bool bAnimate) = 0;
    
    //Runtime config
    virtual void SetRuntimeConfig(RunTimeConfig* pConfig) = 0;
    virtual void GetRuntimeConfig(RunTimeConfig* pConfig) = 0;
    
    //Picking
    virtual void SetPickingMode(bool bPickingOn) = 0;
    virtual bool IsPickingMode() = 0;
    virtual bool GetPickedMeshID(int tx, int ty, unsigned short* mesh_ID) = 0;
    
    //2D
    virtual void AddPolygon(std::string name, unsigned char* rgba, vbRing* pNewRing) = 0;
    virtual VBMapErrorCode AddHoleToLastPolygon(vbRing* pNewRing) = 0;
    virtual void ConvertPolygonToMesh() = 0;
    
    //Group
    virtual std::string IsExistingGroupContaining(std::string group_name) = 0;
    
    //각 그룹의 Show/Hide 설정(단, 메쉬까지 적용을 하는 것을 별도임.) 그룹 이름이  nil이면 전체.
    virtual void    SetVisibilityFlagWithGroupName(bool bVisible, std::string group_name) = 0;
    //Visibility flag를 각 메쉬에 적용한다.
    virtual void    ApplyVisibilityFlagsToMesh() = 0;
    //경로의 Visibility 설정
    virtual void    SetPathVisibilityWithGroupName(std::string group_name, bool bShow, bool bExclusive) = 0;
    virtual void    SetPathVisibilityWithFloorID(int floorID, bool bShow, bool bExclusive) = 0;
    
   
};



struct TextureDescription
{
    TextureFormat   Format;
    int             BitPerComponent;
    ivec2           Size;
    int             MipCount;
};

struct IRenderingEngine {
    virtual void Initialize() = 0;
    virtual void Render() const = 0;
    virtual void pkColorBasedRender() = 0;
    
    virtual void SetViewInfo(vbViewInfo* pViewInfo) = 0;
    virtual void SetConfiguration(vbConfiguration* pConf) = 0;
    virtual void SetMeshManager(vbMeshManager* pMeshMng) = 0;
    virtual void SetMaterialManager(vbMaterialManager* pMtlMng) = 0;
    virtual void ClearData() = 0;
    virtual void LoadTextures() = 0;
    virtual void MeshToVBO() = 0;
    virtual bool SetTexture(const string& name) = 0;
    virtual void SetTextureBasePath(const string& base_path) = 0;
    virtual void SetUseLocalTexture(bool bUseLocalTexture=false) = 0;
    virtual void SetPathManager(vbPathManager* pPathManager) = 0;
    virtual void DeleteBaseFBOAndVBO() = 0;
    virtual AntiAliasingMode    GetAntiAliasingMode() = 0;
    virtual void SetAntiAliasingMode(AntiAliasingMode eAAMode) = 0;
    virtual void SetSolidRendering(bool bOnSolidRendering = true) = 0;
    virtual void SetRenderingMode(vbGLRenderMode render_mode) = 0;
    virtual bool GetColorcodedMeshIDAt(int tx, int ty, unsigned short* pMesh_ID) = 0;
    virtual void SetVerticalScale(float fVScale) = 0;
    virtual void SetScreenScale(float fScrScale) = 0;
    virtual void SetGroundLevel(float fGLevel) = 0;
    virtual float GetGroundLevel() = 0;
    virtual void DeleteBaseData() = 0;
    virtual void SetMyPosDiscState(bool bShow, const float* rgba, const float* pos, float rad) = 0;
    virtual void ShowMyPosDisc(bool bShow) = 0;
    
    virtual ~IRenderingEngine() {}
};

struct IResourceManager {
    virtual string                GetResourcePath() const = 0;
    virtual TextureDescription    LoadResourcePngImage(const string& filename) = 0;
    virtual TextureDescription    LoadImage(const string& file) = 0;
    
    virtual void*   GetImageData() = 0;
    virtual ivec2   GetImageSize() = 0;
    virtual void    UnloadImage() = 0;
    virtual ~IResourceManager() {}
};

IApplicationEngine* CreateApplicationEngine(IRenderingEngine*);
IResourceManager* CreateResourceManager();

namespace ES1  { IRenderingEngine* CreateRenderingEngine(IResourceManager*); }
namespace ES2  { IRenderingEngine* CreateRenderingEngine(IResourceManager*); }
