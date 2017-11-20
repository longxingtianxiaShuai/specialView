//
//  SLIPS.m
//  VBIPS
//
//  Created by Justin on 2014. 6. 30..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "SLIPS.h"
#import "MAVector.h"
#import "Log.h"
#import "MACoordinate.h"

@implementation SLIPS {
    NSTimer *timerSimulation;
   // Log *log;
}

- (id) initWithMapId:(NSString *)mapId andCustomerId:(NSString *)customerId
{
    self = [super init];
    if (self) {
        self.locationManager = [[SLIndoorLocationManager alloc] initWithMapId:mapId andCustomerId:customerId];
        self.locationManager.delegate = self;
        
       // log = [[Log alloc] init];
        /*
        NSDate *now = [NSDate date];
        NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
        [dateFormat setDateFormat:@"yyyy-MM-dd_HH:mm:ss"];
        NSString *time = [dateFormat stringFromDate:now];
        time = [time stringByAppendingString:@".txt"];*/
        //[log setLogFileName:time];
    }
    return self;
}
/*
- (void) didFinishLoadingLocation
{
    //[self.locationManager startUpdatingLocation];
}
*/
- (void) didFailLocationAuthorizationStatus
{
    
}

- (void) didUpdateMotionType:(SLMotionState)motionState
{
    
}

- (void) startUpdatingLocation
{
    [self.locationManager startUpdatingLocation];
}

- (void) stopUpdatingLocation
{
    [self.locationManager stopUpdatingLocation];
}

- (void) didUpdateLocation:(SLCoordinate3D *)location withUncertainty:(double)radius andStatus:(SLLocationStatus)locationStatus
{
    MAVector *pos = [[MAVector alloc] init];
    CGPoint newPos = [self convertToVBPositionFromX:location.longitude andY:location.latitude];
    pos.x = newPos.x;
    pos.y = newPos.y;
    pos.z = location.floorNr;
    
    //[log saveSLLogWithLonguitude:location.longitude andLatitude:location.latitude andX:newPos.x andY:newPos.y radius:radius];
    
    if ([self.delegate respondsToSelector:@selector(didUpdateSLLocation:withUncertainty:)]) {
        [self.delegate didUpdateSLLocation:pos withUncertainty:radius];
    }
}

- (void) didUpdateHeading:(double)heading withStatus:(BOOL)status
{
   // [log saveSLLogWithHeading:heading];
    
    if ([self.delegate respondsToSelector:@selector(didUpdateSLHeading:)]) {
        [self.delegate didUpdateSLHeading:heading + azimuthOffset];
    }
}

- (void) didFailInternetConnectionWithError:(NSError *)error
{
    
}

- (void) didEnterGeometry:(id<SLGeometry>)geometry
{
    
}

- (void) didLeaveGeometry:(id<SLGeometry>)geometry
{
    
}

- (void) didFailInvalidIds:(NSError *)error
{
    
}


- (void) setSLSimulationData:(NSString *)filePath
{
    simulation = [[VBSimulation alloc] init];
    [simulation setData:filePath];
}

- (void) simulationTimer
{
    MACoordinate *info = [simulation getCurrentInfo];
    if (info) {
        if (info.UpdatePosition) {
            if ([self.delegate respondsToSelector:@selector(didUpdateSLLocation:withUncertainty:)]) {
                MAVector *v = [[MAVector alloc] init];
                v.x = info.x;
                v.y = info.y;
                v.z = info.floor;
                [self.delegate didUpdateSLLocation:v withUncertainty:0.0];
            }
        } else {
            if ([self.delegate respondsToSelector:@selector(didUpdateSLHeading:)]) {
                [self.delegate didUpdateSLHeading:info.Headng];
            }
        }
    } else {
        [timerSimulation invalidate];
        timerSimulation = nil;
    }
}

- (void) startSLSimulation
{
    timerSimulation = [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(simulationTimer) userInfo:nil repeats:YES];
}

@end
