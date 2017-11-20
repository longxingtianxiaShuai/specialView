//
//  FloorInfo.h
//  SLIndoorLocation
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SLCoordinate2D.h"
#import "SLPixelPoint2D.h"

@interface FloorInfo : NSObject

/**
 The name of the floor.
 */
@property(readonly, nonatomic, strong) NSMutableString* floorName; 

/**
 The id of the floor.
 */
@property(readonly, nonatomic) NSInteger floorId; 

/**
 The file name of the bitmap image file. Preferably a PNG file, but not necessary.
 */
@property(readonly, nonatomic, strong) NSMutableString* bitmapFilename; 

/**
 The location of the bitmap expressed in latitude and longitude [deg].
 */
@property(readonly, nonatomic) SLCoordinate2D *bitmapLocation;

/**
 The offset of the reference point \c bitmapLocationbitmap relative the bitmap origin (upper left corner). 
 
 \c bitmapOffset is the location of the reference point \c bitmapLocation
 in the bitmap expressed in pixels relative the upper left corner of the bitmap 
 with x-axis pointing to the right and 
 the y-axis pointing downwards, i.e., the standard computer vision definition.
 For example, if \c bitmapLocation represent the location of the upper left corner
 of the bitmap the offset is (x,y) = (0,0).
 */
@property(readonly, nonatomic) SLPixelPoint2D *bitmapOffset;

/**
 The orientation of the bitmap. The orientation is defined as the 
 angle between true North in the bitmap and the up-direction of the bitmap
 (i.e., the minus y-axis) in the clock-wise direction. 
 For example, if North is pointing to the left in the bitmap, then
 the orientation is 90 degrees.
 */
@property(readonly, nonatomic) double bitmapOrientation;

/**
 The scale of the bitmap. The number of pixels in the bitmap corresponding to one meter.
 */
@property(readonly, nonatomic) double pixelsPerMeter; 


@end
