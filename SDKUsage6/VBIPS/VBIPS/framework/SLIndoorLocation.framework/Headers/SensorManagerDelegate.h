//
//  SensorManagerDelegate.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/28/13.
//
//

#import <Foundation/Foundation.h>
#import "AccelerometerData.h"
#import "GyroData.h"
#import "HeadingData.h"

@protocol SensorManagerDelegate <NSObject>
@optional
- (void) didUpdateAccelerometer:(AccelerometerData*)data;
- (void) didUpdateGyro:(GyroData*)data;
- (void) didUpdateHeading:(HeadingData*)data;
@end
