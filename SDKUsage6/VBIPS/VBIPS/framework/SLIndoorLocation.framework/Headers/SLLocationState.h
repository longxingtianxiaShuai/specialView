//
//  SLLocationState.h
//  SLIndoorLocation
//
//  Copyright (c) 2014 SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SLIndoorLocationEnums.h"

@class SLCoordinate3D;

@interface SLLocationState : NSObject

- (id)initWithLocation:(SLCoordinate3D*)location andLocationUncertainty:(double)locationUncertaintyRadius andLocationStatus:(SLLocationStatus)locationStatus;

@property (nonatomic, strong) SLCoordinate3D* location; /**< Current location */
@property (nonatomic) double locationUncertaintyRadius; /**< Current uncertainty in [m] */
@property (nonatomic) SLLocationStatus locationStatus; /**< Current location status. YES means that the location is confirmed by a radio measurement. */ // TODO: use enum instead

@end
