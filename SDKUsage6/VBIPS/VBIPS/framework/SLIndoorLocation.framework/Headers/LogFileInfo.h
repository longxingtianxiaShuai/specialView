//
//  LogFileInfo.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 1/9/13.
//
//

#import <Foundation/Foundation.h>
#import "DeviceInfo.h"

@interface LogFileInfo : NSObject {
    DeviceInfo* deviceInfo;
	NSInteger userRating;
	NSString* userMessage;
	NSString* logFileId;
	NSString* logFileName;
	NSString* mapName;
	NSString* mapDate;
}

-(LogFileInfo*) initWithDeviceInfo:(DeviceInfo*) deviceInfo_ andLogFileId:(NSString*)logFileId_ andLogFileName:(NSString*)logFileName_
                        andMapName:(NSString*)mapName_ andMapDate:(NSString*)mapDate_;

-(LogFileInfo*) initWithDeviceInfo:(DeviceInfo*) deviceInfo_ andLogFileId:(NSString*)logFileId_ andLogFileName:(NSString*)logFileName_
                        andMapName:(NSString*)mapName_ andMapDate:(NSString*)mapDate_ andUserRating:(NSInteger)userRating_ andUserMessage:(NSString*)userMessage_;

-(NSDictionary*) writeObjectToDictionary;

-(NSString*) writeObjectToString;

-(BOOL) writeObjectToFile:(NSString*)filePath;

@property (nonatomic, retain) DeviceInfo* deviceInfo;
@property (nonatomic, retain) NSString* logFileId;
@property (nonatomic, retain) NSString* logFileName;
@property (nonatomic, retain) NSString* mapName;
@property (nonatomic, retain) NSString* mapDate;
@property (nonatomic) NSInteger userRating;
@property (nonatomic, retain) NSString* userMessage;

@end
