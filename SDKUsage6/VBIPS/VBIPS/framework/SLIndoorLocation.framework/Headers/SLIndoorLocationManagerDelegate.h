//
//  SLIndoorLocationManagerDelegate.h
//  SLIndoorLocationManager
//
//  Copyright (c) 2010-2014, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SLIndoorLocationEnums.h"
#import "SLCoordinate3D.h"

@class SLIndoorLocationManager;

/**
 Callback functions for the delegate of the SLIndoorLocationManager.
 */
@protocol SLIndoorLocationManagerDelegate <NSObject>
/** 
 Returns the latitude/longitude of the user with an uncertainty radius (meters) and status (true=confirmed, false=unconfirmed).
 */
- (void) didUpdateLocation:(SLCoordinate3D*)location withUncertainty:(double)radius andStatus:(SLLocationStatus)locationStatus;

/** 
 Returns a heading (can be used to rotate the map) and status indicates if the 
 compass is uncalibrated or there is strong interference from local magnetic fields.
 */
- (void) didUpdateHeading:(double)heading withStatus:(BOOL)status;

/**
 Returns the motion state of the user.
 */
- (void) didUpdateMotionType:(SLMotionState)motionState;

/**
 Called if internet connection is not working.
 */
- (void) didFailInternetConnectionWithError:(NSError *)error;

/**
 Called if invalid customer id or map id is used in the SDK.
 */
- (void) didFailInvalidIds:(NSError *)error;

/**
 Called if user has not authorized use of location status. This is a serious call, since the location might not be available.
 */
- (void) didFailLocationAuthorizationStatus;

@optional
- (void) didFinishLoadingManager;
- (void) didFailScanningBT;

@end
