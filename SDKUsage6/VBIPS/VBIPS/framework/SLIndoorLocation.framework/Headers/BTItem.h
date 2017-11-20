//
//  BTItem.h
//  SLIndoorLocation
//
//  Created by Per on 1/27/13.
//
//

#import <Foundation/Foundation.h>
#import "BTFilterProtocol.h"

@interface BTItem : NSObject <BTFilterProtocol> {
    NSString *uuid;
    int rssi;
    double filteredRssi;
    int count;
    double defaultLambda;
    double missedLambda;
    double rssiFloor;
    bool updated;
    double stdDev;
    int maxRssiThreshold;
}

@property (nonatomic, strong) NSString *uuid;

@end
