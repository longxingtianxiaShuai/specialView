//
//  VBIPS.m
//  VBIPS
//
//  Created by Justin on 2014. 6. 30..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "VBIPS.h"
#import "SLIPS.h"
#import "BeaconIPS.h"

@implementation VBIPS

- (id) initSLIPSWithMapId:(NSString *)mapId andCustomerId:(NSString *)customerId andDelegate:(id<MAIPSDelegate>)delegate
{
    self = [super init];
    if (self) {
        self.IPSType = eSL;
        self.IPS = [[SLIPS alloc] initWithMapId:mapId andCustomerId:customerId];
        self.IPS.delegate = delegate;
    }
    return self;
}

- (id) initBeaconIPSWithUUID:(NSUUID *)UUID andIdentifier:(NSString *)identifier andThreshold:(int)threshold andRepeat:(int)repeat andDelegate:(id<MAIPSDelegate>)delegate
{
    self = [super init];
    if (self) {
        self.IPSType = eBeacon;
        self.IPS = [[BeaconIPS alloc] initBeaconRegionWithUUID:UUID andIdentifier:identifier];
        self.IPS.delegate = delegate;
        self.IPS.threshold = threshold;
        self.IPS.repeat = repeat;
    }
    return self;
}

- (id) initBeaconIPSWithUUIDs:(NSArray *)UUIDs andIdentifiers:(NSArray *)identifiers andThreshold:(int)threshold andRepeat:(int)repeat andDelegate:(id<MAIPSDelegate>)delegate
{
    self = [super init];
    if (self) {
        self.IPSType = eBeacon;
        self.IPS = [[BeaconIPS alloc] initBeaconRegionWithUUIDs:UUIDs andIdentifiers:identifiers];
        self.IPS.delegate = delegate;
        self.IPS.threshold = threshold;
        self.IPS.repeat = repeat;
    }
    return self;
}

- (void) setBeaconThreshold:(int)threshold andRepeat:(int)repeat
{
    self.IPS.threshold = threshold;
    self.IPS.repeat = repeat;
}

- (void) startUpdatingLocation
{
    [self.IPS startUpdatingLocation];
}

- (void) stopUpdatingLocation
{
    [self.IPS stopUpdatingLocation];
}

- (void) setSLTransform:(float *)matrix centerInput:(CGPoint)centerInput_ scaleInput:(CGPoint)scaleInput_ centerOutput:(CGPoint)centerOutput_ scaleOutput:(CGPoint)scaleOutput_ azimuthOffset:(double)azimuthOffset_
{
    [self.IPS setSLTransform:matrix centerInput:centerInput_ scaleInput:scaleInput_ centerOutput:centerOutput_ scaleOutput:scaleOutput_ azimuthOffset:azimuthOffset_];
}

- (void) resetBeaconUpdatingLocation
{
    [self.IPS resetUpdatingLocation];
}

- (void) setSLSimulationData:(NSString *)filePath
{
    [self.IPS setSLSimulationData:filePath];
}

- (void) startSLSimulation
{
    [self.IPS startSLSimulation];
}

@end
