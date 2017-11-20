//
//  VBIPS.h
//  VBIPS
//
//  Created by Justin on 2014. 6. 30..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>


#import "BaseIPS.h"

typedef enum { eSL = 0, eBeacon } eIPSType;

@interface VBIPS : NSObject

@property (nonatomic) eIPSType IPSType;
@property (nonatomic, strong) BaseIPS *IPS;


- (id) initSLIPSWithMapId:(NSString *)mapId andCustomerId:(NSString *)customerId andDelegate:(id<MAIPSDelegate>)delegate;
- (void) setBeaconThreshold:(int)threshold andRepeat:(int)repeat;
- (id) initBeaconIPSWithUUID:(NSUUID *)UUID andIdentifier:(NSString *)identifier andThreshold:(int)threshold andRepeat:(int)repeat andDelegate:(id<MAIPSDelegate>)delegate;
- (id) initBeaconIPSWithUUIDs:(NSArray *)UUIDs andIdentifiers:(NSArray *)identifiers andThreshold:(int)threshold andRepeat:(int)repeat andDelegate:(id<MAIPSDelegate>)delegate;
- (void) startUpdatingLocation;
- (void) stopUpdatingLocation;
- (void) setSLTransform:(float *)matrix centerInput:(CGPoint)centerInput_ scaleInput:(CGPoint)scaleInput_ centerOutput:(CGPoint)centerOutput_ scaleOutput:(CGPoint)scaleOutput_ azimuthOffset:(double)azimuthOffset_;
- (void) resetBeaconUpdatingLocation;

- (void) setSLSimulationData:(NSString *)filePath;
- (void) startSLSimulation;

@end
