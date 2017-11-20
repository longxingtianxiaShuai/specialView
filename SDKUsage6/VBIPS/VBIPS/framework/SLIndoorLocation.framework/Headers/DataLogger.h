//
//  DataLogger.h
//  SLIndoorLocation
//
//  Note! DataLogger is a singleton class!
//
//  Created by Per on 2010-11-28.
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DataLoggerProtocol.h"

@interface DataLogger : NSObject <DataLoggerProtocol> {
	NSFileHandle *fileHandle;
    LogFileInfo* logFileInfo;
    NSString* logFilePath;
}

@end
