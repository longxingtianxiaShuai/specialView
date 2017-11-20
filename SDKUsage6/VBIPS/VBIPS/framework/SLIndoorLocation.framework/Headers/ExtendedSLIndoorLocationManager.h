//
//  ExtendedSLIndoorLocationManager.h
//  SLIndoorLocation
//
//  Created by Per on 2011-02-22.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "SLIndoorLocationManagerDelegate.h"
#import "NavFilter.h"
#import "SLFilterDelegate.h"
#import "MapHandlerDelegate.h"
#import "DataLoggerProtocol.h"
#import "NavFilterSettings.h"
#import "SLGeofencingEngine.h"

@interface ExtendedSLIndoorLocationManager : NSObject <SLFilterDelegate, MapHandlerDelegate> {
    @private
    NSString *customerId;
    id<SensorManagerProtocol> sensorManager;
    id<DataLoggerProtocol> logger;
    NavFilterSettings *settings;
}

/**
 Init location using map ID and customer ID, these ID:s are obtained
 for a specific venue from SenionLab.
 */
- (id) initWithMapId:(NSString*)mapId andCustomerId:(NSString*)customerId;

/**
 Set delegate class that will receive updates from SLIndoorLocationManager.
 */
- (void) setDelegate:(id<SLIndoorLocationManagerDelegate>)delegate_;

/**
 Start location updates if they have previously been stopped with
 SLIndoorLocationManager::stopUpdatingLocation. This function is
 automatically called after initialization.
 */
- (void) startUpdatingLocation;

/**
 Stops location updates. To start positioning again, use SLIndoorLocationManager::startUpdatingLocation.
 */
- (void) stopUpdatingLocation;

- (void) startMockupLocationWithLocation:(SLCoordinate3D*) location_ andUncertaintyRadius:(double) uncertaintyRadius andLocationStatus:(SLLocationStatusType) status andTimeInterval:(double) timeInterval;

- (void) stopUpdatingMockupLocation;

/**
 Step length in [m] is specified using this function. Since different users have different
 steplengths, this should be possible to set.
 */
- (void) setStepLength:(double)stepLength;

/**
 Returns current steplength in [m].
 */
- (double) stepLength;

/**
 Force calls to didUpdateFilter and didUpdateHeading of the delegate.
 */
- (void) reportLocation;

- (double) getTargetRadius;
- (SLPoint3D*) getTargetLocation;

/**
 If a new position has been specified by the user, using for example double
 tap on a map position, SLIndoorLocationManager::moveToLocation: is used to
 update the current estimated position to the specified one. It has one input
 argument which is the new location specified as an SLCoordinate2D struct.
 */
- (void) moveToLocation:(SLCoordinate3D*)location_;

- (bool) isLogging;

- (void) startLogging;

- (void) pauseLogging;

- (void) stopLogging;

- (void) logNavigationComment:(NSString*)answer;
- (void) logNavigationSurvey:(NSString*)answer;

- (NSString*) getUserId;

- (bool) isLogApp;

- (NSString*) getCustomerId;
- (NSString*) getMapId;
- (NSString*) getMapVersionId;
- (NSString*) getMapName;
- (NSString*) getMapDate;

- (BOOL) isBluetoothEnabled;

- (void) shouldDisplayHeadingCalibration:(BOOL)displayCalibration;

- (void) setUseCompass:(BOOL)state;
- (BOOL) getUseCompass;
- (void) setUseRadio:(BOOL)state;
- (BOOL) getUseRadio;
- (void) setUseStep:(BOOL)state;
- (BOOL) getUseStep;

- (SLCoordinate3D*) getEndLocation;
- (SLCoordinate3D*) getStartLocation;
- (void) moveEndLocationTo:(SLCoordinate3D*)newEndLocation;
- (double) getInitHeadingForSubpath;

- (NavFilterSettings*) getSettings;
- (NavFilter*) getNavFilter;

- (id<SLGeofencing>) getGeofencingManager;

+ (NSString*) getSdkVersion;

@end
