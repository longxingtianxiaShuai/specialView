//
//  RadioMap.h
//  SLIndoorLocation
//
//  Created by Per on 2011-04-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RadioLink.h"
#import "BTinfo.h"
#import "NavFilterSettings.h"
#import "NetPos.h"

@interface RadioMap : NSObject {
	// Date of the map data.
	NSString *dataDate;
	
	// The name of the map.
	NSString *name;
	
	// Array storing all unique bssids in the map
    NSMutableArray *allBssidArray;
    
    NSMutableArray *radioLinkArray;

	double radioMeasurementVariance;
    double minRadioExpectedLevel;
    double minRadioLikelihood;
    double defaultLevelFloor;
    bool useRelativeRadio;
}

- (RadioMap*) initWithJsonDictionary:(NSDictionary*) json andSettings:(NavFilterSettings*)settings;
- (void) computeRadioLikelihood:(double*)likelihoods forMeasurement:(NSMutableDictionary*)btInfo atPositions:(NSArray*)pos;
- (double) getExpectedLevelAtPosition:(NetPos*) pos forBssid:(NSString*)bssid;
- (double) getRadioLikelihoodForMeasurement:(BTinfo*) btInfo atPosition:(NetPos*) pos;
- (NSMutableArray*) getRadioLikelihoodOnGrid:(NSMutableDictionary*)btInMap;
- (Boolean) isBssidInMap:(NSString*)bssid;

@end
	

