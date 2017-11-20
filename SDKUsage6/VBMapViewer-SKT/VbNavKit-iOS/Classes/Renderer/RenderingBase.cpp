//
//  RenderingBase.cpp
//  PISAiOS
//
//  Created by 버츄얼빌더스  버츄얼 on 11. 12. 6..
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "RenderingBase.h"


void    RenderingBase::InitCommonState(IResourceManager* resourceManager)
{
    
    m_resourceManager = resourceManager;
	
    m_pMeshManager  = NULL;
    m_pViewInfo     = NULL;
    m_pConf         = NULL;
    
    m_bUseLocalTexture = true;
    m_eAniAliasingMode = AAMode_ALWAYS;//  AAMode_DYNAMIC;
    m_eDeviceDirection = DeviceOrientationPortrait;
    
    m_render_mode = RENDERMODE_DEFAULT;
    
    m_fVerticalScale = 1.f;
    m_fScreenScale = 1.f;
    
    m_fGroundLevel = 0.f;
    
    m_sMyPos.pVts = 0;
    m_sMyPos.bShow = false;
    
    InitMyPosDisc(1.f, 0.5f, 0.8f, 1.f, 0.5f, 20);
}

void    RenderingBase::SetMeshManager(vbMeshManager* pMeshMng)
{
	m_pMeshManager = pMeshMng;
    m_ColorCoder.SetMeshManager(pMeshMng);
}

void    RenderingBase::SetMyPosDiscState(bool bShow, const float* rgba, const float* pos, float rad)
{
    m_sMyPos.bShow = bShow;
    m_sMyPos.radius = rad;
    
    memcpy(m_sMyPos.RGBA, rgba, sizeof(float)*4);
    memcpy(m_sMyPos.pos, pos, sizeof(float)*3);
}



void    RenderingBase::SetSolidRendering(bool bOnSolidRendering)
{
    if(bOnSolidRendering)   m_render_mode = RENDERMODE_SOLID_COLOR;
    else                    m_render_mode = RENDERMODE_DEFAULT;
}

void RenderingBase::SetRenderingMode(vbGLRenderMode eRenderMode)
{
    m_render_mode = eRenderMode;
}

void RenderingBase::SetConfiguration(vbConfiguration* pConf)
{
    if(!pConf)  return;
    
    m_pConf     = pConf;
    
    m_sMyPos.bShow = pConf->bShowPositionRange;
}


void RenderingBase::MeshToVBO()
{
    if(m_pConf->bUseVBO)    InitializeVBO();
}

void    RenderingBase::InitMyPosDisc(float rad, float r, float g, float b, float a, unsigned char numTriangle)
{
    m_sMyPos.radius = rad;
    
    m_sMyPos.numFanTriangle = (int)numTriangle;

    m_sMyPos.RGBA[0]= r;
    m_sMyPos.RGBA[1]= g;
    m_sMyPos.RGBA[2]= b;
    m_sMyPos.RGBA[3]= a;
    
    if (m_sMyPos.pVts)  delete [] m_sMyPos.pVts;
    m_sMyPos.pVts = new float[(numTriangle+2) * 3];
    
    //원점
    m_sMyPos.pVts[0] = m_sMyPos.pVts[1] = m_sMyPos.pVts[2] = 0.f;

    
    //원주상의 Vt 생성
    float stepAngle = Pi * 2.f / float(numTriangle);
    for (int i=0; i<=numTriangle; i++)
    {
        m_sMyPos.pVts[(i+1)*3 + 0] = cos(stepAngle * float(i));  //X
        m_sMyPos.pVts[(i+1)*3 + 1] = 0.f;                        //Y
        m_sMyPos.pVts[(i+1)*3 + 2] = -sin(stepAngle * float(i));  //Z
    }

}

void    RenderingBase::DeleteBaseData()
{
    //MyPos
    if (m_sMyPos.pVts)  delete [] m_sMyPos.pVts;
    m_sMyPos.pVts = 0;
}