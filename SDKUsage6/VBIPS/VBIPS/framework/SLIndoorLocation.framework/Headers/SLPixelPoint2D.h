//
//  SLPixelPoint2D.h
//  SLIndoorLocation
//
//  Created by Tim Bjärengren on 05/11/14.
//  Copyright (c) 2014 SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 This class contains a pixel coordinate in 2D
 */
@interface SLPixelPoint2D : NSObject

/**
 Constructor.
 @param x point in pixels
 @param y point in pixels
 */
- (id)initWithX:(double)x andY:(double)y;

/**
 x point in pixels
 */
@property (nonatomic) double x;

/**
 y point in pixels
 */
@property (nonatomic) double y;

@end
