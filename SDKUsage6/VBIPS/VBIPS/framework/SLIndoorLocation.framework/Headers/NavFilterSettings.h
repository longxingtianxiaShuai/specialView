//
//  NavFilterSettings.h
//  SLIndoorLocation
//
//  Created by Per on 4/5/13.
//
//

#import <Foundation/Foundation.h>
#import "SLPoint3DPrivate.h"

@interface NavFilterSettings : NSObject {
    
}

- (NavFilterSettings*) init;

- (NavFilterSettings*) initWithJsonDictionary:(NSDictionary*) json;

- (void) readParametersFromJsonDictionary:(NSDictionary*) json;

- (NSDictionary*) writeObjectToDictionary;

- (NSString*) writeObjectToString;

- (void) writeObjectToFile:(NSString*)fileName;

- (void) printParameters;

/*! @property resampleInterval Maximum number of time steps without a resampling. (A resamling might be performed earlier if the effective number of particle threshold is fulfilled.)*/
@property(readonly) int resampleInterval;

/*! @property numParticles Number of particles.*/
@property(readonly) int numParticles;

@property(readonly) double stepThreshold;

/*! @property stepLengthStdDev Standard deviation of step length noise. */
@property(readonly) double stepLengthStdDev;

/*! @property headingStdDev Standard deviation of heading noise. */
@property(readonly) double headingStdDev;

/*! @property minInitPositionStd Minimum initPositionStd (standard deviation of the initial position). Note that initPositionStd is also dependent on the link width.*/
@property(readonly) double minInitPositionStd;

/*! @property minEffectiveNumberOfParticles Minimum measure of effectiveness for the particles before resampling is done. */
@property(readonly) double minEffectiveNumberOfParticles;

/*! @property minNodeDistThreshold Minimum nodeDistThreshold (nodeDistThreshold is used to modify the weigths of particles near a node). Note that nodeDistThreshold is also dependent on the link width.*/
@property(readonly) double minNodeDistThreshold;

/*! @property stdToUncertaintyRadiusFactor A factor that the location standard deviation is multiplied with before sending to the delegate object.*/
@property(readonly) double stdToUncertaintyRadiusFactor;

/*! @property maxLocationUncertaintyRadius Ceiling of location uncertainty in meter, if unceratinty from the nav filter is larger than this threshold this value is sent instead.*/
@property(readonly) double maxLocationUncertaintyRadius;

/*! @property minLocationUncertaintyRadius Floor of location uncertainty in meter, if unceratinty from the nav filter is less than this threshold this value is sent instead.*/
@property(readonly) double minLocationUncertaintyRadius;

@property(readonly) double stepLengthStdDevBTSim;

/*! @property radioReinitThresh Threshold for reinit. */
@property(readonly) double radioReinitThresh;


@property(readonly) int reinitThreshold;
@property(readonly) int nrRadioForReinit;
@property(readonly) double maxPosStdDevForReinit;
@property(readonly) double maxRssiThreshold;
@property(readonly) double maxDiffRssiThreshold;
@property(readonly) int maxMissedDetectionForUseInReinit;
@property(readonly) int stepsToUnconfirmed;

@property(readonly) double targetRadius;
@property(readonly) SLPoint3D* targetLocation;

@property(readonly) int btMeasToUnconfirmed;
@property(readonly) double stdDevMaxUnconfirmed;
@property(readonly) bool useFiltRssiForReinit;
@property(readonly) double radioReinitThreshFactor;
@property(readonly) double reinitMeasPfGridDiffForReinit;
@property(readonly) bool useLambdaFilter;
@property(readonly) int nMaxMissedDisturbanceIndicatorCalc;
@property(readonly) double disturbanceIndicatorThreshold;

@property(readonly) double floorNrPropabilityThresholdForPosObtained;
@property(readonly) double floorNrPropabilityThresholdForConfirmed;
@property(readonly) bool useFiltRssiInPFUpdate;

@property(readonly) double floorNrPropabilityThresholdForUpdateDelegate;

@property(readonly) bool useHeadingInTimeUpdate;
@property(readonly) bool useHeadingInMeasUpdate;

@property(readonly) NSMutableArray *monitoredRegions;
@property(readonly) NSMutableArray *rangedRegions;

@property(readonly) double initPosStdDevForSubpath;

@property(readonly) double radioMeasurementVariance;
@property(readonly) double minRadioExpectedLevel;
@property(readonly) double minRadioLikelihood;
@property(readonly) double defaultLevelFloor;
@property(readonly) bool useRelativeRadio;
@property(readonly) double btUpdateInterval;

@property(readonly) bool logIBDB;
@property(readonly) bool logIBTI;

@property(readonly) int minNbrOfUsedRadioForConfirmed;

@end
