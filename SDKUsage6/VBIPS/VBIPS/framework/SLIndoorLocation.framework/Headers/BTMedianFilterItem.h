//
//  BTMedianFilterItem.h
//  SLIndoorLocation
//
//  Created by Per on 5/28/13.
//
//

#import <Foundation/Foundation.h>
#import "BTFilterProtocol.h"

@interface BTMedianFilterItem : NSObject <BTFilterProtocol> {
    NSString *uuid;
    double filteredRssi;
    int count;
    int rssiFloor;
    bool updated;
    NSMutableArray *buffer;
    int bufferPointer;
    int bufferSize;
    int defaultIndex;
    int maxRssiThreshold;
}
@end
