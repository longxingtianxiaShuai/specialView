//
//  HeadingEst.h
//  SLIndoorLocation
//
//  Copyright 2011 SenionLab. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ManageHeadingList.h"
#import "TSAGeoMag.h"
#import "HeadingEstDelegate.h"
#import "AccelerometerData.h"
#import "GyroData.h"
#import "HeadingData.h"
#import "SLCoordinate2D.h"

@interface HeadingEst : NSObject {
	
	double accLPX;
	double accLPY;
	double accLPZ;
	
	double headingX;
	double headingY;
	double headingZ;
	
	double declination;
	
	double rotationSum;
	int rotationN;
	double oldHeading;
	
	double headingEst;
	double headingP;
	double headingMeas;
	
	NSTimeInterval gyroFreq;
	NSTimeInterval accFreq;
	
	bool useGyro;
	bool canProjectHeading;
	bool firstHeadingMeas;
	
	double sSum;
	double aLP;
	
	bool magIsCalibrating;
	int magCalibrationCounter;
    
	ManageHeadingList *sManager;
	TSAGeoMag *magComp;
}


- (HeadingEst*) init;
- (void) projectHeading:(HeadingData*) heading;
- (void) testHeading:(NSDate*) timestamp;

- (void) measurementUpdate;
- (void) timeUpdateUsing:(double) rotation andTimeInterval:(double) dT;

- (void) didUpdateMagnetometer:(HeadingData*) heading;
- (void) didUpdateAccelerometer:(AccelerometerData*) acc;
- (void) didUpdateGyro:(GyroData*) gyro;

- (void) calculateHeadingMeasurement;
- (double) fixHeadingSingularityUsingHeadingMeas:(double) heading;

- (void) useAccWithFreq:(NSTimeInterval)accFreq_;
- (void) useGyroWithFreq:(NSTimeInterval)gyroFreq_; 

- (void) computeDeclinationUsing:(SLCoordinate2D*) location;

- (void) setInitialHeading:(double)initHeading;

@property (nonatomic) double headingEst;
@property (nonatomic, weak) id <NSObject, HeadingEstDelegate > delegate;


@end

