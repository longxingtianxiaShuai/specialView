//
//  SLPixelPoint3D.h
//  SLIndoorLocation
//
//  Created by Tim Bjärengren on 05/11/14.
//  Copyright (c) 2014 SenionLab AB. All rights reserved.
//

#import "SLPixelPoint2D.h"
#import "FloorNr.h"

/**
 This class contains a pixel coordinate in 3D
 */
@interface SLPixelPoint3D : SLPixelPoint2D

/**
 Constructor.
 @param x point in pixels
 @param y point in pixels
 @param floorNr of point
 */
- (id)initWithX:(double)x andY:(double)y andFloorNr:(FloorNr)floorNr;

/**
 FloorNr
 */
@property (nonatomic) FloorNr floorNr;

@end
