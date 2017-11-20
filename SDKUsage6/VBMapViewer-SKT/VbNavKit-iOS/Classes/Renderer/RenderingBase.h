#ifndef PISAiOS_RenderingBase_h
#define PISAiOS_RenderingBase_h

#include "Interfaces.hpp"
#include "Matrix.hpp"
#include "vbMesh.hpp"
#include "vbMeshManager.hpp"
#include "vbMaterialManager.hpp"
#include "vbViewInfo.hpp"
#include "vbConfiguration.hpp"
#include "vbColorCoder.hpp"

#include <iostream>
using namespace std;

//RenderingEngine에서 1.1과 2.0의 공통 부분의 구현 클래스임.
class RenderingBase : public IRenderingEngine
{
public:

protected:
    //Resource manager
    IResourceManager*   m_resourceManager;

    //Data pointer
    vbMeshManager*      m_pMeshManager;
    vbMaterialManager*  m_pMtlManager;
    vbViewInfo*         m_pViewInfo;
    vbConfiguration*    m_pConf;
    vbPathManager*      m_pPathManager;
    
    //Render base info
    std::string         m_texture_base_path;
    
    //Rendering flag
    bool                m_bUseLocalTexture;
    //bool                m_bShowPositionRange; vbMyPos 내으 flag로 대체함.
    
    //Color-based picking
    vbColorCoder        m_ColorCoder;
    
    //Super Sampling(Anti-aliasing)
    //Super Sampling
    AntiAliasingMode    m_eAniAliasingMode;
    
    unsigned int        m_offscreenRectVBOArrayID;
    unsigned int        m_offscreenRectVBOTxID;
    unsigned int        m_offscreenRectVBOIndexID;
    
    vbGLRenderMode      m_render_mode;    
    DeviceDirection     m_eDeviceDirection;
    
    //Scale
    float               m_fVerticalScale;
    float               m_fScreenScale; //Retina.....
    
    float               m_fGroundLevel;
    
    vbMyPos             m_sMyPos;
  
    
    struct  vbMyPos
    {
        bool    bShow;                  //가시화 여부
        float   radius;                 //반지름 - 스케일에 사용 됨.
        float   RGBA[4];                //색상
        unsigned char numFanTriangle;   //팬 삼각형 개수
        float*  pVts;                   //Vertex좌표들 (원점에 반지름이 1인 원)
        float   pos[3];                 //위치 - 이동에 사용 됨.
    };
   
public:
    void    InitMyPosDisc(float rad, float r, float g, float b, float a, unsigned char numTriangle);
    void    SetMyPosDiscRadius(float rad) { m_sMyPos.radius = rad;  }
    void    SetMyPosDiscColor(float r, float g, float b, float a)
    {
        m_sMyPos.RGBA[0] = r;
        m_sMyPos.RGBA[1] = g;
        m_sMyPos.RGBA[2] = b;
        m_sMyPos.RGBA[3] = a;
    }
    void    SetMyPosCoord(float x, float y, float z)
    {
        m_sMyPos.pos[0] = x;
        m_sMyPos.pos[1] = y;
        m_sMyPos.pos[2] = z;
    }
    void    SetMyPosDiscState(bool bShow, const float* rgba, const float* pos, float rad);
    
    void    SetGroundLevel(float fGLevel)       {   m_fGroundLevel = fGLevel;       }
    float   GetGroundLevel()                    {   return m_fGroundLevel;          }
    
    //Screen scale
    float   GetScreenScale()                                {   return m_fScreenScale;          }
    void    SetScreenScale(float fScreenScale)              {   m_fScreenScale = fScreenScale;  }
  
    //Vertical scale
    float   GetVerticalScale()                              {   return m_fVerticalScale;    }
    void    SetVerticalScale(float fVScale)                 {   m_fVerticalScale = fVScale; }
    
    //Set Data Pointer
    void    SetMeshManager(vbMeshManager* pMeshMng);
    void    SetMaterialManager(vbMaterialManager* pMtlMng)  {   m_pMtlManager = pMtlMng;    }
    void    SetViewInfo(vbViewInfo* pViewInfo)              {   m_pViewInfo = pViewInfo;    }
    void    SetConfiguration(vbConfiguration* pConf);
    void    SetPathManager(vbPathManager* pPathManager)     {   m_pPathManager = pPathManager;            }
    
    
    //Base path
    void    SetTextureBasePath(const string& base_path)     {   m_texture_base_path = base_path;    }

    
    //Rendering flag
    void    ShowMyPosDisc(bool bShow)  {    m_sMyPos.bShow = bShow; }
    void    ShowPositionRange(bool bShow)
    {
        ShowMyPosDisc(bShow);
    }
    bool    IsShowPositionRange()           {   return m_sMyPos.bShow;    };
    
    void    SetUseLocalTexture(bool bUseLocalTexture=false)
                                            { m_bUseLocalTexture =  bUseLocalTexture;}
    
    void    SetSolidRendering(bool bOnSolidRendering = true);
    void    SetRenderingMode(vbGLRenderMode eRenderMode);
 
    //VBO init
    void    MeshToVBO();
   
protected:
    //Initialize
    void    InitCommonState(IResourceManager* resourceManager);
    
    void    DeleteBaseData();
   
    //Super Sampling
    AntiAliasingMode    GetAntiAliasingMode()              {   return m_eAniAliasingMode; };
    void                SetAntiAliasingMode(AntiAliasingMode eAAMode)     {   m_eAniAliasingMode = eAAMode; };

    //Rendering
    virtual void    PreGLState(vbMaterial::VBMESH_TYPE mesh_type) const = 0;
    virtual void    PostGLState(vbMaterial::VBMESH_TYPE mesh_type) const = 0;
    
    
    virtual void    RenderWorld() const = 0;
    virtual void    pkRenderWorld() = 0;
    
    virtual void    RenderOnScreenRect() const = 0;
    virtual void    RenderOnScreenRectVBO() const = 0;
    
    ////Rendering Contents
    virtual void    DrawPathRect() const = 0;
    virtual void    DrawMeshesSeperatedVertexArray() const = 0;
    virtual void    pkDrawMeshesSeperatedVertexArray()= 0;
    
    virtual void    DrawMeshesWithVBO() const = 0;
    virtual void    pkDrawMeshesWithVBO()= 0;
    virtual void    DrawInstancedMeshVBO(const vbMesh* pMesh) const = 0;
 //   virtual void    DrawStaticMeshVBO(const vbMesh* pMesh) const = 0;
    
//    virtual void    DrawInstancedMeshVTArray(const vbMesh* pMesh) const = 0;

    
    virtual void    DrawMeshesSeperatedVertexArraySolid() const = 0;
    virtual void    DrawMeshesWithVBOSolid() const = 0;

    virtual void    DrawMeshesSeperatedVertexArrayColor() const = 0;
    virtual void    DrawMeshesWithVBOColor() const = 0;
    
    virtual void    DrawEdges() const = 0;
    
    virtual void    DrawGroundGrid() const = 0;
    virtual void    DrawMyPosDisc() const = 0;
    


    
    //VBO
    virtual void    DeleteAllVBO() = 0;
    virtual bool    InitializeVBO() = 0;
    
    //FBO
    virtual void    InitFrameBuffer() = 0;
    virtual ivec2   GetFBOSize() const = 0;
 
    //GL State
    virtual void    InitGLStates() = 0;
    
    //Offscreen VBO
    virtual void    InitOffscreenVBO() = 0;
    
    //Texture
    virtual bool    SetTexture(const string& name) = 0;
    
    //Error
    virtual void    ChekGLError() = 0;
    
    
  
};


#endif
