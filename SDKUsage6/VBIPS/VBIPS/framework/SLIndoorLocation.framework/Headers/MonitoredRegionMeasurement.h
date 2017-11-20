//
//  MonitoredRegionMeasurment.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 10/29/13.
//
//

#import <Foundation/Foundation.h>
#import "BeaconId.h"

@interface MonitoredRegionMeasurement : NSObject {
    @private
    double time;
    BeaconId *beaconId;
    int state;
}

- (id) initWithTime:(double)time_ andRegion:(BeaconId*)beaconId_ andState:(int)state_;
-(NSDictionary*) writeObjectToDictionary;
-(NSString*) writeObjectToString;

@end
