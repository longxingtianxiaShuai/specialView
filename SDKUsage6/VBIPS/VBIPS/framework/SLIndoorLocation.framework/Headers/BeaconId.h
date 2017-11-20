//
//  MonitoredRegion.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 10/21/13.
//
//

#import <Foundation/Foundation.h>

@interface BeaconId : NSObject <NSCopying> {
    @private
    NSString *uuid;
    NSNumber* major;
    NSNumber* minor;
}

- (id) initWithUuid:(NSString*)uuid_;
- (id) initWithUuid:(NSString*)uuid_ andMajor:(NSNumber*)major_;
- (id) initWithUuid:(NSString*)uuid_ andMajor:(NSNumber*)major_ andMinor:(NSNumber*)minor_;
-(NSString*) getBTString;
-(NSDictionary*) writeObjectToDictionary;
-(NSString*) writeObjectToString;
+(BeaconId*) readObjectFromDictionary:(NSDictionary*) dict;

@property(readonly) NSString *uuid;
@property(readonly) NSNumber* major;
@property(readonly) NSNumber* minor;

@end
