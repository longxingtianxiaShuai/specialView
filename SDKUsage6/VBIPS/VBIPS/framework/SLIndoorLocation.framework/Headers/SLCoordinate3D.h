//
//  SLCoordinate3D.h
//  SLIndoorLocation
//
//  Copyright (c) 2010-2014, SenionLab AB. All rights reserved.
//
//

#import "SLCoordinate2D.h"
#import "FloorNr.h"

/**
 This class contains a geographical coordinate using the WGS 84 reference frame including floor number.
 */
@interface SLCoordinate3D : SLCoordinate2D

/**
 Constructor.
 @param latitude_ Latitude in degrees. Positive value is north of the equator, negative south.
 @param longitude_ Longitude in degrees. Positive value is east of the meridian, negative west.
 @param floorNr_ Floor number.
 */
- (id) initWithLatitude:(double)latitude_ andLongitude:(double)longitude_ andFloorNr:(FloorNr)floorNr_;

- (id) initWithSLCoordinate3D:(SLCoordinate3D*)c;

-(NSDictionary*) writeObjectToDictionary;

+(SLCoordinate3D*) readObjectFromDictionary:(NSDictionary*) dict;

- (BOOL) isEqualToSLCoordinate3D:(SLCoordinate3D *)p withAccuracy:(double)tol;

@property (nonatomic) FloorNr floorNr; /**< Floor number*/

@end
