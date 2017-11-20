//
//  BTinfo.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 3/31/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BTinfo : NSObject {
@private
    NSString *UUID;
    NSNumber *RSSI;
    NSString *name;
    NSString *desc;
    double time;
    NSDictionary *advertisementData;
}

- (id) initWithUUID:(NSString*)UUID_ andRSSI:(NSNumber*)RSSI_ andName:(NSString*)name_ andDescription:(NSString*)desc andAdvertisementData:(NSDictionary *)advertisementData andTime:(double)time_;
- (id) initWithUUID:(NSString*)UUID_ andRSSI:(NSNumber*)RSSI_ andName:(NSString*)name_ andDescription:(NSString*)desc;
- (id) initWithUUID:(NSString*)UUID_ andRSSI:(NSNumber*)RSSI_ andTime:(double) time_;
- (id) initWithUUID:(NSString*)UUID_ andRSSI:(NSNumber*)RSSI_;
- (id) initWithMessage:(NSString*)message;
- (NSNumber*) getChannel;
+ (NSMutableDictionary*) radioScanFromIBeaconScan:(NSMutableDictionary*)iBeaconScan;
-(NSDictionary*) writeObjectToDictionary;
-(NSString*) writeObjectToString;
- (void) setNewRSSI:(NSNumber*)RSSI_;
-(NSDictionary*) writeObjectToDictionaryCompact;
-(NSString*) writeObjectToStringCompact;

@property (nonatomic,readonly) NSString *UUID;
@property (nonatomic,readonly) NSNumber *RSSI;
@property (nonatomic,readonly) NSString *name;
@property (nonatomic,readonly) NSString *desc;
@property (nonatomic,readonly) double time;
@property (nonatomic,readonly) NSDictionary *advertisementData;

@end
