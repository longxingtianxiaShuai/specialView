//
//  DeviceInfo.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 1/8/13.
//
//

#import <Foundation/Foundation.h>

@interface DeviceInfo : NSObject {
    NSString* userId;
	NSString* customerId;
	NSString* mapId;
	NSInteger mapVersion;
	NSString* mapVersionId;
	NSString* sdkVersion;
	NSInteger sentTime;
	NSString* phoneManufacturer;
	NSString* phoneModel;
	NSString* phoneProduct;
	NSString* phoneFingerprint;
	NSInteger startTime;
}

-(DeviceInfo*) initWithUserId:(NSString*) userId_ andCustomerId:(NSString*) customerId_ andMapId:(NSString*) mapId_
                andMapVersion:(NSInteger) mapVersion_ andMapVersionId:(NSString*) mapVersionId_ andSdkVersion:(NSString*) sdkVersion_;

-(DeviceInfo*) initWithUserId:(NSString*) userId_ andCustomerId:(NSString*) customerId_ andMapId:(NSString*) mapId_
                andMapVersion:(NSInteger) mapVersion_ andMapVersionId:(NSString*) mapVersionId_ andSdkVersion:(NSString*) sdkVersion_
         andPhoneManufacturer:(NSString*) phoneManufacturer_ andPhoneModel:(NSString*) phoneModel_ andPhoneProduct:(NSString*) phoneProduct_
          andPhoneFingerprint:(NSString*) phoneFingerprint_ andStartTime:(NSInteger) startTime_;

-(DeviceInfo*) initWithUserId:(NSString*) userId_ andCustomerId:(NSString*) customerId_ andMapId:(NSString*) mapId_
                andMapVersion:(NSInteger) mapVersion_ andMapVersionId:(NSString*) mapVersionId_ andSdkVersion:(NSString*) sdkVersion_
         andPhoneManufacturer:(NSString*) phoneManufacturer_ andPhoneModel:(NSString*) phoneModel_ andPhoneProduct:(NSString*) phoneProduct_
          andPhoneFingerprint:(NSString*) phoneFingerprint_ andStartTime:(NSInteger) startTime_ andSentTime:(NSInteger) sentTime_;

-(NSDictionary*) writeObjectToDictionary;

-(NSString*) writeObjectToString;

@property (nonatomic, retain) NSString* userId;
@property (nonatomic, retain) NSString* customerId;
@property (nonatomic, retain) NSString* mapId;
@property (nonatomic) NSInteger mapVersion;
@property (nonatomic, retain) NSString* mapVersionId;
@property (nonatomic, retain) NSString* sdkVersion;
@property (nonatomic) NSInteger sentTime;
@property (nonatomic, retain) NSString* phoneManufacturer;
@property (nonatomic, retain) NSString* phoneModel;
@property (nonatomic, retain) NSString* phoneProduct;
@property (nonatomic, retain) NSString* phoneFingerprint;
@property (nonatomic) NSInteger startTime;

@end
