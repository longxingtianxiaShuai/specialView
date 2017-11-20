//
//  vbConfiguration.cpp
//  PISAiOS
//
//  Created by 버츄얼빌더스 버츄얼 on 11. 8. 25..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "vbConfiguration.hpp"
#include "Interfaces.hpp"

vbConfiguration::vbConfiguration()
{
    //Background color
    fBgColor[0]=0.5f;   fBgColor[1]=0.5f;   fBgColor[2]=0.5f; fBgColor[3]=0.5f;
    
    //Solid color
    fSolidColor[0]=0.6f;   fSolidColor[1]=0.6f;   fSolidColor[2]=0.6f; fSolidColor[3]=1.f;

    //Ground
    plnInitGround.SetPlane(vbPlane::PLN_P_ZX, 0.f);
    
    //Pan Expand Ratio
    fPanPlaneExpandRatio = 0.f;
    
    //Mesh type
    nDefaultVertexFlag  =   VertexFlagsNormals | VertexFlagsTexCoords;   //기본 vertex 속성
    
    //Projection
    fInitPerspective[0] = 60.f;      //fovy(degree)
    fInitPerspective[1] = 0.666f;    //aspect
    fInitPerspective[2] = 100.f;     //near
    fInitPerspective[3] = 10000.f;   //far
    
    //Min. near clip distance
    fMinNear = 100.f;
    fCamMinDistance = 100.f;    //최소 카메라 거리는 NearClip보다 커야 한다.
    fCamMaxDistanceRatioToBBSize = 3.5f;
    fCamMaxPanRatioToBSphereRad  = 3.5f;
    
    
    //LightPosition
    flightPosition  = vec4(2.0, 1.0, 3.0, 0);
    fLightAmbient   = vec4(0.5, 0.5, 0.5, 1.);
    //fLightAmbient   = vec4(1.0, 1.0, 1.0, 1.);
    
    //Camera 
    fvCameraOrbitCenter = vec3(0.f,0.f,0.f);
    //Q = -0.67559, 4.21579e-38, 4.61707e-37, 0.737277 - front max
    qCameraQuaternion = Quaternion(-0.67559,0.f,0.f,0.737277);
    fEyeHeight          = 150.f;
    fFPSSpeed           = 40.f;
    vec3 dir1 = vec3(0.1f,0.f,1.f);
    dir1.Normalize();
    qFPSRotation        = Quaternion::CreateFromVectors(vec3(0.f,0.f,1.f), dir1);
   
    fJumpZoomRatio      = 0.5f;
    bJumpPanOnJumpZoom  = true;
    
    fEndFPSDirection = -45.f;
        
    
    bShowPositionRange = false;
    
    fCamRotateAreaRatio = 0.20f; //20% from bottom
    
    fPitchLimitMin = -85.f;//30.f;//Deg
    fPitchLimitMax = -30.f;//85.f;
    fEnvSphereRadius = 1000.f;
    
    fForwardZoomHeightLimitBottomY = 150.f;     // 1.5 meter
    fForwardZoomHeightLimitTopY = 250000.f;   // Application select

    //Animation
    fCameraAniStep = 0.075f;
    bUseCameraAnimation = true;
    
    
    //Super Sampling
    //nAntiAliasingMode = 1;  //Dynamic mode
    nAntiAliasingMode = 2;  //Always
    bUseVBO = true;
       
}

vbConfiguration::~vbConfiguration()
{
    
}