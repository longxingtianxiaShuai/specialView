//
//  StepDetect.h
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Step.h"
#import "HeadingEstDelegate.h"
#import "HeadingEst.h"
#import "SensorManager.h"
#import "DataLoggerProtocol.h"
#import "NavFilterSettings.h"

#define MAXBUF 8

@protocol HeadingEstDelegate;

@protocol StepDetectDelegate
- (void) didStep:(Step*)step;
- (void) didUpdateHeading:(double)heading withStatus:(BOOL)status;
- (double) getHeadingOfMeanParticleLink;
@end

@interface StepDetect : NSObject <SensorManagerDelegate, HeadingEstDelegate> {
    bool useCompassMeas;
	double stepLength;
	double lpLambda;
	double lpHeading;
	int headingIterCounter;
	int nHeadingIter;
	double buffer[MAXBUF];
	int bufferind;
	int timeToStep;
	int stepInterval;
	bool bufferFull;
	int stepNr;
	bool stepDetected;
	bool stepStart;
	bool maxMinOrder;
	double buffer_min;
	double buffer_max;
	NSDate *time;
    double currentTime;
    double lastPlotTime;
	Step *step;
    HeadingEst *headingEst;
    
    int stepCountDown;
    int stepIntervalFake;
    NavFilterSettings *settings;
    id<DataLoggerProtocol> logger;
}

- (StepDetect*) initWithIsSubpath:(BOOL)isSubpath andSettings:(NavFilterSettings*)settings;
- (void) MaxMinBuffer;
- (void) initBuffer;
- (void) addToBuffer: (AccelerometerData*) acceleration;
- (void) printBufferData;
- (void) dealloc;
- (void) passLongLat:(SLCoordinate2D*) location;
- (void) detectStep;
- (void) stop;
- (void) prepareForSubpathLoggingWithInitHeading:(double)initHeading;

@property (nonatomic) NSTimeInterval accFreq;
@property (nonatomic) bool stepStart;
@property (nonatomic) int stepNr;
@property (nonatomic) double stepLength;
@property (nonatomic, weak) id <StepDetectDelegate> delegate;
@property (retain, nonatomic) Step *step;

@end
