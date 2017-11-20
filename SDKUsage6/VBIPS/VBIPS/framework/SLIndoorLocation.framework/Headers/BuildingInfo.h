//
//  BuildingInfo.h
//  SLIndoorLocation
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "SLCoordinate3D.h"
#import "SLPixelPoint3D.h"
#import "FloorNr.h"

@class FloorInfo;

/**
 This class keeps information about the building the user is currently in and how the global
 properties of the building relate to the bitmap such as the long/lat location, orientation and scale.
 
 Note that the map information in this class is used only for showing the
 user location on a bitmap image of the building. This class is not used by the navigation filter.
 */
@interface BuildingInfo : NSObject {
}

/**
 Init function where the bitmap properties are stored in a JSON file (http://json.org).
 The structure and names of the JSON fields are the same as the class properties. 
 See below for an example of a JSON file.
 \verbatim
 {
	"name":"Building name",
	"bitmapFilename":"building.png",
	"bitmapOffset":{"x":472,"y":715},
	"bitmapLocation":{"latitude":47.5735,"longitude":-122.171049},
	"bitmapOrientation":-90,
	"pixelsPerMeter":2.877929732
 }
 \endverbatim
 */
- (id)initWithBitmapJsonFilename:(NSString *) bitmapJsonFilename;

- (id)initWithBitmapJsonData:(NSData *) bitmapJsonData;

/**
 This function converts a pixel position to longitude and latitude position. 
 */
- (SLCoordinate3D *)SLPixelPoint3DToSLCoordinate3D:(SLPixelPoint3D *)point;

/**
 Convert a longitude and latitude position to a pixel coordinate.
 */
- (SLPixelPoint3D *)SLCoordinate3DToSLPixelPoint3D:(SLCoordinate3D *) location_;

/**
 Convert compass heading to heading relative the screen reference system.
 */
- (double)heading2PixelHeading:(double)heading andFloorNr:(FloorNr)floorNr_;

/**
 Get the floorInfo of floor floorNr.
 */
- (FloorInfo *)getFloorInfo:(FloorNr)floorNr_;

/**
 Get default floor number.
 */
- (FloorNr)getDefaultFloorNr;

/**
 Get the floorInfo of floor floorNr.
 */
- (NSInteger)getTotalNumberOfFloors;

/**
 The name of the building.
 */
@property(readonly, nonatomic, strong) NSMutableString *name;

/**
 The date of the data.
 */
@property(readonly, nonatomic, strong) NSMutableString *dataDate;

/**
 The list containing all floorInfo
 */
@property (nonatomic, strong) NSDictionary *floorInfoList;

/**
 The list containing all floorInfo
 */
@property (nonatomic, strong) NSMutableArray *floorNrArray;
 
@end
