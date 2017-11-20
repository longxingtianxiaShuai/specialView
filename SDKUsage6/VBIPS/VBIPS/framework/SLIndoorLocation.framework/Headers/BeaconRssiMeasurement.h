//
//  BeaconRssiMeasurement.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/8/13.
//
//

#import <Foundation/Foundation.h>
#import "BeaconId.h"

@interface BeaconRssiMeasurement : NSObject {
    @private
    double time;
    BeaconId *beaconId;
    NSInteger rssi;
}

- (id) initWithTime:(double)time_ andBeaconId:(BeaconId*)beaconId_ andRssi:(NSInteger)rssi_;
-(NSDictionary*) writeObjectToDictionary;
-(NSString*) writeObjectToString;

@property (readonly) BeaconId *beaconId;
@property (readonly) NSInteger rssi;

@end
