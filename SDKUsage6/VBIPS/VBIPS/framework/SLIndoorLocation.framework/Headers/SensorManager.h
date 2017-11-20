//
//  SensorManager.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/27/13.
//
//


#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "SensorManagerProtocol.h"

@interface SensorManager : NSObject <SensorManagerProtocol, CLLocationManagerDelegate, BluetoothDelegate>

@end
