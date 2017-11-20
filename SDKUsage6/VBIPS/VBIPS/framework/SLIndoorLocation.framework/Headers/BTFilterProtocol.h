//
//  BTFilterProtocol.h
//  SLIndoorLocation
//
//  Created by Per on 5/27/13.
//
//

#import <Foundation/Foundation.h>

@protocol BTFilterProtocol <NSObject>

- (id)initWithUuid:(NSString*)uuid andRssi:(int)rssi andMaxRssiThreshold:(int)maxRssiThreshold;
- (void)updateRssi:(int)rssi;
- (int)getLastRssi;
- (double)getFilteredRssi;
- (int)getMissedCounter;
- (NSString*)getUuid;
- (void)updateFinished;
- (double)getMean;
- (double)getStdDev;

@end
