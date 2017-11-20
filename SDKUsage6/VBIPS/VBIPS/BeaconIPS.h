//
//  BeaconIPS.h
//  VBIPS
//
//  Created by Justin on 2014. 7. 8..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "BaseIPS.h"
#import <CoreLocation/CoreLocation.h>
#import <CoreBluetooth/CoreBluetooth.h>

@interface BeaconIPS : BaseIPS <CLLocationManagerDelegate>

@property (nonatomic, strong) CLLocationManager *locManager;
@property (nonatomic, strong) CLBeaconRegion *beaconRegion;


- (id)initBeaconRegionWithUUID:(NSUUID *)proximityUUID andIdentifier:(NSString*)identifier;
- (id)initBeaconRegionWithUUIDs:(NSArray *)proximityUUIDs andIdentifiers:(NSArray *)identifiers_;

@end
