//
//  BluetoothDelegate.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/29/13.
//
//

#import <Foundation/Foundation.h>

@protocol BluetoothDelegate <NSObject>
@required
- (void)didUpdateBT:(NSMutableDictionary*)btScan;
- (void)didUpdateiBeacon:(NSMutableDictionary*)btScan;
@optional
- (void)didFailScanningBluetooth;
@end
