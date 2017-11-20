#include "vbViewInfo.hpp"

#include <iostream>

bool vbViewInfo::SingleFingerUp(ivec2 oPos, int tapCount, vec3& pos3d)
{
   // std::cout << "Single finger Up\r\n";
    
    ivec2 tPos = ConvertToRotatedDeviceCoord(oPos);
    
    switch (m_eViewControlSet)
    {
        //Yaw & Pitch mode
        case vbViewControlSetYawPitch:
        {
            return SingleFingerUp_YawPitch(tPos, tapCount, pos3d);
        }
        break;
            
        case vbViewControlSetStandardI:
        {
            return SingleFingerUp_STD1(tPos, tapCount, pos3d);
        }
        break;
            
        case vbViewControlSetGongVueTypeA:
        {
            return SingleFingerUp_GongVueA(tPos, tapCount, pos3d);
        }
        break;
            
        case vbViewControlSetZoneView://Zone view(4)
        {
            return SingleFingerUp_ZoneView(tPos, tapCount, pos3d);
        }
        break;
            
        case vbViewControlSetPalladiON://PalladiON(5)
        {
            return SingleFingerUp_PalladiON(tPos, tapCount, pos3d);
        }
        break;
            
        case vbViewControlSetPlanar3D://Planar3D(6)
        {
            return SingleFingerUp_Planar3D(tPos, tapCount, pos3d);
        }
        break;
            
        case vbViewControlSetFixedFPS://자유롭게 움직이되 API로만 제어 할 수 있다.
            break;

        //Default LBS mode
        case vbViewControlSetDefaultLBS:
        {
            return SingleFingerUp_Default(tPos, tapCount, pos3d);
        }
        break;
        default:
        {
        }
        break;
    }
      
    return false;
}

void vbViewInfo::SingleFingerDown(ivec2 oPos, int tapCount)
{
    //std::cout << "Single finger down\r\n";
    ivec2 tPos = ConvertToRotatedDeviceCoord(oPos);
   
    switch (m_eViewControlSet)
    {
        //Yaw & Pitch mode
        case vbViewControlSetYawPitch:
        {
            SingleFingerDown_YawPitch(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetStandardI:
        {
            SingleFingerDown_STD1(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetGongVueTypeA:
        {
            SingleFingerDown_GongVueA(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetZoneView://Zone view(4)
        {
            return SingleFingerDown_ZoneView(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetPalladiON://PalladiON(5)
        {
            return SingleFingerDown_PalladiON(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetPlanar3D://Planar3D(6)
        {
            return SingleFingerDown_Planar3D(tPos, tapCount);
        }
        break;

        case vbViewControlSetFixedFPS://자유롭게 움직이되 API로만 제어 할 수 있다.
            break;
            
        //Default LBS mode
        case vbViewControlSetDefaultLBS:
        {
            SingleFingerDown_Default(tPos, tapCount);
        }
            break;
        default:
        {
        }
            break;
    }
    
}

bool vbViewInfo::SingleFingerMove(ivec2 oPos, int tapCount)
{
    //std::cout << "Single finger mode\r\n";
    ivec2 tPos = ConvertToRotatedDeviceCoord(oPos);

    switch (m_eViewControlSet)
    {
        //Yaw & Pitch mode
        case vbViewControlSetYawPitch:
        {
            return SingleFingerMove_YawPitch(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetStandardI:
        {
            return SingleFingerMove_STD1(tPos, tapCount);
            
        }
        break;
            
        case vbViewControlSetGongVueTypeA:
        {
            return SingleFingerMove_GongVueA(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetZoneView://Zone view(4)
        {
            return SingleFingerMove_ZoneView(tPos, tapCount);
        }
        break;
            
            
        case vbViewControlSetPalladiON://PalladiON(5)
        {
            return SingleFingerMove_PalladiON(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetPlanar3D://Planar3D(6)
        {
            return SingleFingerMove_Planar3D(tPos, tapCount);
        }
        break;
            
        case vbViewControlSetFixedFPS://자유롭게 움직이되 API로만 제어 할 수 있다.
            break;
            
        //Default LBS mode
        case vbViewControlSetDefaultLBS:
        {
            return SingleFingerMove_Default(tPos, tapCount);
        }
            break;
        default:
        {
        }
            break;
    }
  
    return false;    //No Update

}

bool vbViewInfo::DoubleFingerUp(ivec2 oPos1/*pressed*/, int tapCount1,ivec2 oPos2/*unpressed*/, int tapCount2)
{
    //std::cout << "Double finger Up\r\n";
    ivec2 pos1 = ConvertToRotatedDeviceCoord(oPos1);
    ivec2 pos2 = ConvertToRotatedDeviceCoord(oPos2);

    switch (m_eViewControlSet)
    {
            //Yaw & Pitch mode
        case vbViewControlSetYawPitch:
        {
            return DoubleFingerUp_YawPitch(pos1, tapCount1, pos2, tapCount2);
        }
        break;

        case vbViewControlSetStandardI:
        {
            return DoubleFingerUp_STD1(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetGongVueTypeA:
        {
            return DoubleFingerUp_GongVueA(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetZoneView://Zone view(4)
        {
            return DoubleFingerUp_ZoneView(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
            
        case vbViewControlSetPalladiON://PalladiON(5)
        {
            return DoubleFingerUp_PalladiON(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
            
        case vbViewControlSetPlanar3D://Planar3D(6)
        {
            return DoubleFingerUp_Planar3D(pos1, tapCount1, pos2, tapCount2);
        }
        break;
  
        case vbViewControlSetFixedFPS://자유롭게 움직이되 API로만 제어 할 수 있다.
            break;
            
        //Default LBS mode
        case vbViewControlSetDefaultLBS:
        {
            return DoubleFingerUp_Default(pos1, tapCount1, pos2, tapCount2);
        }
            break;
        default:
        {
        }
            break;
    }
           
    return true;
}

void vbViewInfo::DoubleFingerDown(ivec2 oPos1, int tapCount1,ivec2 oPos2, int tapCount2)
{
    //std::cout << "Double finger Down\r\n";
    ivec2 pos1 = ConvertToRotatedDeviceCoord(oPos1);
    ivec2 pos2 = ConvertToRotatedDeviceCoord(oPos2);
    

    switch (m_eViewControlSet)
    {
        //Yaw & Pitch mode
        case vbViewControlSetYawPitch:
        {
            DoubleFingerDown_YawPitch(pos1, tapCount1, pos2, tapCount2);
        }
        break;
        
        case vbViewControlSetStandardI:
        {
            DoubleFingerDown_STD1(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetGongVueTypeA:
        {
            DoubleFingerDown_GongVueA(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetZoneView://Zone view(4)
        {
            DoubleFingerDown_ZoneView(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetPalladiON://PalladiON(5)
        {
            DoubleFingerDown_PalladiON(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
            
        case vbViewControlSetPlanar3D://Planar3D(6)
        {
            DoubleFingerDown_Planar3D(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetFixedFPS://자유롭게 움직이되 API로만 제어 할 수 있다.
            break;
            
        //Default LBS mode
        case vbViewControlSetDefaultLBS:
        {
            DoubleFingerDown_Default(pos1, tapCount1, pos2, tapCount2);
        }
            break;
        default:
        {
            DoubleFingerDown_Default(pos1, tapCount1, pos2, tapCount2);
        }
            break;
    }
}

bool vbViewInfo::DoubleFingerMove(ivec2 oPos1, int tapCount1,ivec2 oPos2, int tapCount2)
{
    //std::cout << "Double finger Move\r\n";
    ivec2 pos1 = ConvertToRotatedDeviceCoord(oPos1);
    ivec2 pos2 = ConvertToRotatedDeviceCoord(oPos2);
    

    switch (m_eViewControlSet)
    {
            //Yaw & Pitch mode
        case vbViewControlSetYawPitch:
        {
            return DoubleFingerMove_YawPitch(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetStandardI:
        {
            return DoubleFingerMove_STD1(pos1, tapCount1, pos2, tapCount2);
        }
        break;

        case vbViewControlSetGongVueTypeA:
        {
            return DoubleFingerMove_GongVueA(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetZoneView://Zone view(4)
        {
            return DoubleFingerMove_ZoneView(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
            
        case vbViewControlSetPalladiON://PalladiON(5)
        {
            return DoubleFingerMove_PalladiON(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
            
            
        case vbViewControlSetPlanar3D://Planar3D(6)
        {
            return DoubleFingerMove_Planar3D(pos1, tapCount1, pos2, tapCount2);
        }
        break;
            
        case vbViewControlSetFixedFPS://자유롭게 움직이되 API로만 제어 할 수 있다.
            break;
            
            //Default LBS mode
        case vbViewControlSetDefaultLBS:
        {
            return DoubleFingerMove_Default(pos1, tapCount1, pos2, tapCount2);
        }
            break;
            //Default LBS mode
        default:
        {
            return DoubleFingerMove_Default(pos1, tapCount1, pos2, tapCount2);
        }
            break;
    }
       
    return true;
}



