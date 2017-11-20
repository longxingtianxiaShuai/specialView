//
//  SensorManagerProtocol.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/28/13.
//
//

#import <Foundation/Foundation.h>
#import "SensorManagerDelegate.h"
#import "BluetoothDelegate.h"

@protocol SensorManagerProtocol <NSObject>
- (void) setSensorDelegate:(id<SensorManagerDelegate>) delegate;
- (void) removeSensorDelegate:(id<SensorManagerDelegate>) delegate;
- (void) setBluetoothDelegate:(id<BluetoothDelegate>) delegate;
- (void) removeBluetoothDelegate:(id<BluetoothDelegate>) delegate;
- (void) setLogFileName:(NSString*)logFileName;
- (void) startSensors;
- (void) stopSensors;
- (void) startScanBluetooth:(double)btUpdateInterval;
- (void) stopScanBluetooth;
- (void) startScaniBeaconWithRangingRegions:(NSArray*)rangingRegions andMonitorRegions:(NSArray*)monitorRegions;
- (void) stopScaniBeacon;
- (bool) isGyroAvailable;
- (bool) isAccelerometerAvailable;
- (bool) isMagnetometerAvailable;
- (bool) supportLEHardware;
- (void) displayHeadingCalibration:(bool)value;
- (void) setIsSubpath:(BOOL)value;
@property(readonly, nonatomic) double accFreq;
@property(readonly, nonatomic) double gyroFreq;
@end
