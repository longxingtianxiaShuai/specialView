//
//  navFilter.h
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "StepDetect.h"
#import "NetworkMap.h"
#import "RadioMap.h"
#import "NavFilterSettings.h"
#import "DataLoggerProtocol.h"
#import "NavFilterEnums.h"


@protocol SLFilterDelegate;

/*!
 @class NavFilter
 @abstract Testing abstract
 @discussion NavFilter discussion
 */

@interface NavFilter : NSObject <StepDetectDelegate, BluetoothDelegate, HeadingEstDelegate> {
	
	/*! @property stepDetect Step detect object.*/
	StepDetect *stepDetect;
	
	/*! @property particles Particle objects.*/
	NSMutableArray* particles;
	
	/*! @property weights Importance weighs of the particles.*/
	double *weights;
    
    /*! @property likelihoods Likelihoods of the particles in the BT update step.*/
    double *likelihoods;
	
	/*! @property numParticles Number of particles.*/
	int numParticles;
	
	/*! @property timeToResample Counter of time steps, when zero a resampling is performed.*/
	int timeToResample;
	
	RadioMap *radioMap;

	/*! @property map Map network.*/
	NetworkMap *map;
	
	/*! @property resamplingInd Resampling indexes. Help variable in the resampling process. */
	int *resamplingInd;
	
	/*! @property headingStatus The status of the magnetometer, if calibration is needed etc.*/
	BOOL headingStatus;
	
    int *particlesOnRadioLink;
    int posObtainedCounter;
    int reinitCounter;
    int stepsSinceStandardUpdate;
    int btMeasSinceStandardUpdate;
    bool positionObtained;
    double disturbanceIndicator;
    
    NSMutableDictionary *iBeaconItems;
    NSMutableDictionary *slBeaconItems;
    
    bool useRadio;
    
    NSInteger lastNbrOfUsedRadio;
    
    NavFilterSettings* settings;
    
    id<SensorManagerProtocol> sensorManager;
    id<DataLoggerProtocol> logger;
}

/*! @method initWithLocation Init with network map and radio map. */
- (NavFilter*) initWithDelegate:(id)delegate_ andNetworkMap:(NetworkMap*)networkMap_ andRadioMap:(RadioMap*)radioMap_ andSettings:(NavFilterSettings*)settings_;

- (void) prepareForStartLogging;
- (void) prepareForStopLogging;

- (void) printParticles;
- (void) predictParticle:(int) i withLength:(float)length andHeading:(float)heading;
- (void) didStep:(Step*)step;
- (void) simulateParticles:(Step*) step;
- (void) printParticlesDebug;
- (void) printWeightsDebug;

/*! @method isRadioMapAvailable Returns true if a radio map is loaded. */
- (bool) isRadioMapAvailable;
- (BOOL) isBluetoothEnabled;
- (BOOL) getUseRadio;
- (void) setUseRadio:(BOOL)state;
- (void) updateToLocation:(SLCoordinate3D*) location;
- (void) updateToLocation:(SLCoordinate3D*) location withUncertainty:(double) std0;
- (void) setStepLength:(double)stepLength;
- (double) getStepLength;
- (NSMutableArray*) getParticles;
- (void) reportLocationWithNavFilterStatus:(NavFilterStatusType) navFilterStatus;
- (bool) isSubpath;
- (void) dealloc;

@property (nonatomic, weak) id <NSObject, SLFilterDelegate > delegate;
@property (readonly, strong) NSMutableArray* particles;
@property (readonly) int numParticles;
@property (readonly) double* weights;
@property (readonly, strong) NetworkMap* map;
@property (nonatomic) bool useStep;
@property (nonatomic) bool useCompass;

@end
