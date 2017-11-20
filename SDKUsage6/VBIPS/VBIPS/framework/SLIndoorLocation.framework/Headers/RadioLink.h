//
//  RadioLink.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 4/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface RadioLink : NSObject {
    double defaultLevelFloor;
    int numberOfPoints;
    double distanceInterval;
    NSMutableDictionary *radioDataDict;
	int linkID;
}

-(id) initWithlinkID:(int) linkID_ radioDataDictionary:(NSMutableDictionary*)radioDataDict_ andNumberOfPoints:(int)numberOfPoints_ andDistanceInterval:(double)distanceInterval_ andDefaultLevelFloor:(double)defaultLevelFloor_;
-(double) getLevelAtDistance:(double) distance forBssid:(NSString*) bssid;

@property (nonatomic, readonly) NSMutableDictionary *radioDataDict;
@property (nonatomic, readonly) int numberOfPoints;
@property (nonatomic, readonly) double distanceInterval;
@end
