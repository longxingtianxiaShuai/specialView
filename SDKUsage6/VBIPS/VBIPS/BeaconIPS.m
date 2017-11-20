//
//  BeaconIPS.m
//  VBIPS
//
//  Created by Justin on 2014. 7. 8..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "BeaconIPS.h"

@implementation BeaconIPS {
    NSMutableArray *arrayRepeat;
    BOOL isFirst;
    int count;
    CLBeacon *prevBeacon;
    BOOL leaveState;
    NSArray *UUIDs;
    NSArray *identifiers;
}

- (id)initBeaconRegionWithUUID:(NSUUID *)proximityUUID andIdentifier:(NSString*)identifier
{
    self = [super init];
    if (self) {
        isFirst = YES;
        arrayRepeat = [[NSMutableArray alloc] initWithCapacity:5];
        self.beaconRegion = [[CLBeaconRegion alloc]
                            initWithProximityUUID:proximityUUID
                                identifier:identifier];
        
        // Register the beacon region with the location manager.
        self.locManager = [[CLLocationManager alloc] init];
        self.locManager.delegate = self;
        //[self.locManager startMonitoringForRegion:self.beaconRegion];
        prevBeacon = nil;
        leaveState = NO;
    }
    return  self;
}

- (id)initBeaconRegionWithUUIDs:(NSArray *)proximityUUIDs andIdentifiers:(NSArray *)identifiers_
{
    self = [super init];
    if (self) {
        isFirst = YES;
        arrayRepeat = [[NSMutableArray alloc] initWithCapacity:5];
        
        self.locManager = [[CLLocationManager alloc] init];
        self.locManager.delegate = self;
        
        UUIDs = proximityUUIDs;
        identifiers = identifiers_;
        prevBeacon = nil;
        leaveState = NO;
    }
    return self;
}

- (void) startUpdatingLocation
{
    if ([UUIDs count] > 0) {
        for (NSInteger idx = 0; idx < [UUIDs count]; idx++) {
            NSUUID *proximityUUID = [UUIDs objectAtIndex:idx];
            NSString *regionID = [identifiers objectAtIndex:idx];
            CLBeaconRegion *region = [[CLBeaconRegion alloc] initWithProximityUUID:proximityUUID identifier:regionID];
        
            [self.locManager startMonitoringForRegion:region];
            [self.locManager startRangingBeaconsInRegion:region];
        }
    } else {
        [self.locManager startMonitoringForRegion:self.beaconRegion];
    }
}

- (void)locationManager:(CLLocationManager *)manager didRangeBeacons:(NSArray *)beacons inRegion:(CLBeaconRegion *)region
{
    if ([beacons count] > 0) {
        CLBeacon *nearestExhibit = [self getNearestBeacon:beacons];
        
        if (isFirst) {
            count = 1;
            [arrayRepeat addObject:nearestExhibit];
            isFirst = NO;
        }
        if (nearestExhibit.rssi > self.threshold) {
           [arrayRepeat addObject:nearestExhibit];
            int index = (int)[arrayRepeat count] - 1;
            if ([nearestExhibit.major isEqualToNumber:((CLBeacon *)[arrayRepeat objectAtIndex:index]).major]
                && [nearestExhibit.minor isEqualToNumber:((CLBeacon *)[arrayRepeat objectAtIndex:index]).minor]
                && nearestExhibit.proximity != CLProximityUnknown) {
                count++;
                
                NSLog(@"rssi -> %@, count = %d", nearestExhibit.minor, count);
                
                if ([arrayRepeat count] >= self.repeat) {
                    [arrayRepeat removeObjectAtIndex:0];
                }
                if (count >= self.repeat) {
                    if ([self.delegate respondsToSelector:@selector(didEnterBeaconRegion:)]) {
                        [self.delegate didEnterBeaconRegion:nearestExhibit];
                    }
                    
                    if ([self isEqualToBeacon:nearestExhibit another:prevBeacon] == NO) {
                        if ([self.delegate respondsToSelector:@selector(didUpdateBeaconLocation:)]) {
                            [self.delegate didUpdateBeaconLocation:nearestExhibit];
                        }
                        leaveState = NO;
                        prevBeacon = nearestExhibit;
                    } else {
                        isFirst = YES;
                        [arrayRepeat removeAllObjects];
                    }
                    count = 0;
                }
            }

        } else {
            if ([self isEqualToBeacon:nearestExhibit another:prevBeacon] == NO && prevBeacon != nil && leaveState == NO) {
                if ([self.delegate respondsToSelector:@selector(didLeaveBeaconRegion)]) {
                    [self.delegate didLeaveBeaconRegion];
                }
                leaveState = YES;
            }
        }
    }
}

- (BOOL) isEqualToBeacon:(CLBeacon *)beacon1 another:(CLBeacon *)beacon2
{
    if (beacon1 == nil || beacon2 == nil) {
        return NO;
    }
    if ([beacon1.major isEqualToNumber:beacon2.major] && [beacon1.minor isEqualToNumber:beacon2.minor]
        && [beacon1.proximityUUID isEqual:beacon2.proximityUUID]) {
        return YES;
    } else {
        return NO;
    }
}

- (CLBeacon *) getNearestBeacon:(NSArray *)beacons
{
    NSInteger compare = -300;
    int index = 0;
    int found = 0;
    for (CLBeacon *bc in beacons) {
        if (compare < bc.rssi && bc.rssi != 0) {
            compare = bc.rssi;
            found = index;
        }
        index++;
    }
    return [beacons objectAtIndex:found];
}

- (void) resetUpdatingLocation
{
    prevBeacon = nil;
    [arrayRepeat removeAllObjects];
}


@end
