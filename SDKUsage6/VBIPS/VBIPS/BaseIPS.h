//
//  BaseIPS.h
//  VBIPS
//
//  Created by Justin on 2014. 6. 30..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "VBSimulation.h"
#import "MAVector.h"


@protocol MAIPSDelegate <NSObject>
@optional
- (void) didUpdateSLLocation:(MAVector3D *)location withUncertainty:(double)radius;
- (void) didUpdateSLHeading:(double)heading;
- (void) didUpdateBeaconLocation:(CLBeacon *)beacon;
- (void) didEnterBeaconRegion:(CLBeacon *)beacon;
- (void) didLeaveBeaconRegion;

@end

@interface BaseIPS : NSObject {
    double azimuthOffset;
    VBSimulation *simulation;
}

@property(nonatomic,assign) id<MAIPSDelegate> delegate;

@property (nonatomic) int threshold;
@property (nonatomic) int repeat;

- (void) startUpdatingLocation;
- (void) stopUpdatingLocation;
- (void) setSLTransform:(float *)matrix centerInput:(CGPoint)centerInput_ scaleInput:(CGPoint)scaleInput_ centerOutput:(CGPoint)centerOutput_ scaleOutput:(CGPoint)scaleOutput_ azimuthOffset:(double)azimuthOffset_;
- (CGPoint) convertToVBPositionFromX:(float)x andY:(float)y;
- (void) resetUpdatingLocation;
- (void) setSLSimulationData:(NSString *)filePath;
- (void) startSLSimulation;
@end
