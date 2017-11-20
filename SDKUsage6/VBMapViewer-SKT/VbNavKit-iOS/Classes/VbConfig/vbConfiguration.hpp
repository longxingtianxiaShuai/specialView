#pragma once
#include "Quaternion.hpp"
#include "vbGeometry.hpp"

#define DEFAULT_CONF_MOVE_SPEED     1.f
#define DEFAULT_CONF_ENDFPS_ANGLE   -45.f


class vbConfiguration
{
public:
    vbConfiguration();
    ~vbConfiguration();
    
    //Background color
    float   fBgColor[4];            //배경 색상
    
    //Solid rendering
    float   fSolidColor[4];         //단색 랜더링 색상
    
    //Base Ground plane
    vbPlane plnInitGround;
    
    //Pan Plane expand ratio
    float   fPanPlaneExpandRatio;       //Pan을 위한 바닥과의 교점 검사 범위 
    
    //Mesh type
    unsigned char nDefaultVertexFlag;   //기본 vertex 속성
    
    //Camera
    vec3        fvCameraPosition;
    vec3        fvCameraOrbitCenter;
    Quaternion  qCameraQuaternion;

    float       fEyeHeight;
    float       fFPSSpeed;
    Quaternion  qFPSRotation;   //Y축 기준의 회전양 정의
    float       fJumpZoomRatio;
    bool        bJumpPanOnJumpZoom;
    float       fEndFPSDirection;   //아래쪽 몇도를 향하도록 할 것인지 설정 - 반드시 0보다 작아야 한다.
    
    float       fCameraAniStep;
    bool        bUseCameraAnimation;
    float       fCamMinDistance; 
    float       fCamMaxDistanceRatioToBBSize;
    float       fCamMaxPanRatioToBSphereRad;
    
    //Yaw & Pitch 
    float       fPitchLimitMin;
    float       fPitchLimitMax;
    float       fYawPitchOffsetLimitMin;
    float       fYawPitchOffsetLimitMax;
    float       fEnvSphereRadius;
    
    //Forward zooming height limits
    float       fForwardZoomHeightLimitBottomY;
    float       fForwardZoomHeightLimitTopY;
    
    
    
    
    //Camera control
    float       fCamRotateAreaRatio;    //화면의 아래쪽 어느 정도를 회전에 사용 할 것인지 (0 : 가장 아래, 1 : 화면 전체)
    
    //Projection
    float   fInitPerspective[4];    //초기 Persepective parameter
    float   fMinNear;               //최소 Near Clip
    
    //Me
    bool        bShowPositionRange;
    
    //Light
    vec4    flightPosition;
    vec4    fLightAmbient;
    
    
    //SuperSampling
    unsigned char    nAntiAliasingMode;
    bool             bUseVBO;
  
    
public:
    
};