//
//  vbEnum.hpp
//  VbNavKit-iOS
//
//  Created by MobileTeam on 2014. 7. 28..
//  Copyright (c) 2014년 dev4. All rights reserved.
//

#ifndef VbNavKit_iOS_vbEnum_hpp
#define VbNavKit_iOS_vbEnum_hpp

struct RunTimeConfig
{
    float  fBackgroundColor4f[4];
    char   bUseVBO;
    float  fPerspectiveParam[4];    //0-FOV, 1-Aspect ratio, 2-Near, 3-Far
    float  fRotationAreaRatio;
    float  fSolidColor4f[4];
    float  fCamDistanceRange[2];    //0-Min Distance, 1-MaxDistanceRatioToBBSize
    float  fAmbientColor4f[4];
    float  fPitchMinMax[2];         //0-Min pitch, 1-Max pitch
};

struct  vbRing
{
    int     nPtCount;
    float*  pts2D;      // pts2D = new float[nPtCount*2]
};


struct  vbMyPos
{
    bool    bShow;                  //가시화 여부
    float   radius;                 //반지름 - 스케일에 사용 됨.
    float   RGBA[4];                //색상
    unsigned char numFanTriangle;   //팬 삼각형 개수
    float*  pVts;                   //Vertex좌표들 (원점에 반지름이 1인 원)
    float   pos[3];                 //위치 - 이동에 사용 됨.
};

enum VBMapErrorCode
{
    VBMapError_UNKNOWN = 0,
    VBMapError_NO_ERROR = 1,
    VBMapError_MAP_PATH_UNDEFINED = 2,
    VBMapError_INVALID_CONTEXT = 3,
    VBMapError_MAP_FILE_OPEN_ERROR = 4,
    VBMapError_UNSUPORRTED_VERSION = 5,
    VBMapError_INSTANCING_WITH_STATIC_MESH = 6,
    VBMapError_BAD_INPUT_PARAMETER = 7,
    VBMapError_NOT_READY = 8,
    VBMapError_REFER_IMPORPER_MESH = 9
    
};


enum AntiAliasingMode
{
    AAMode_NONE = 0,
    AAMode_DYNAMIC = 1,
    AAMode_ALWAYS = 2
};

enum    vbGLRenderMode
{
    RENDERMODE_DEFAULT = 0,         //Texture + Light
    RENDERMODE_TEXTURE_LIGHT = 0,   //Texture + Light
    RENDERMODE_SOLID_COLOR = 1,     //No Texture + Light + Single color
    RENDERMODE_NO_TEX_LIGHT = 2,    //Own color + Light
    RENDERMODE_TEXTURE_NO_LIGHT = 3,//For Offscreen rendering
    RENDERMODE_NO_TEX_NO_LIGHT =4
};

enum TextureFormat
{
    TextureFormatGray,
    TextureFormatGrayAlpha,
    TextureFormatRgb,
    TextureFormatRgba,
    TextureFormatPvrtcRbg2,
    TextureFormatPvrtcRgba2,
    TextureFormatPvrtcRgb4,
    TextureFormatPvrtcRgba4,
    TextureFormat565,
    TextureFormat5551,
};

enum DeviceDirection//Named according to the position of the Home button
{
    DeviceOrientationUnknown=0,
    DeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
    DeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
    DeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
    DeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
    DeviceOrientationFaceUp,              // Device oriented flat, face up
    DeviceOrientationFaceDown             // Device oriented flat, face down
};

enum VBOrthoCameraDirection
{
    VBOrthoCameraDirectionNegativeX=0,
    VBOrthoCameraDirectionPositiveX=1,
    VBOrthoCameraDirectionNegativeY=2,
    VBOrthoCameraDirectionPositiveY=3,
    VBOrthoCameraDirectionNegativeZ=4,
    VBOrthoCameraDirectionPositiveZ=5
};



#endif
