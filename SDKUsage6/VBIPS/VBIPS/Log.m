//
//  Log.m
//  VBIPS
//
//  Created by Justin on 2014. 8. 18..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "Log.h"

@implementation Log {
    NSFileHandle *fileHandle;
}

- (void) setLogFileName:(NSString *)fileName
{
    NSString *filePath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    filePath = [filePath stringByAppendingPathComponent:fileName];
    if(![[NSFileManager defaultManager] fileExistsAtPath:filePath])
    {
        [[NSFileManager defaultManager] createFileAtPath:filePath contents:nil attributes:nil];
        
        fileHandle = [NSFileHandle fileHandleForUpdatingAtPath:filePath];
    }

}

- (void) saveSLLogWithLonguitude:(double)longuitude andLatitude:(double)latitude andX:(float)x andY:(float)y  radius:(double)radius
{
    NSDate *now = [NSDate date];
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd_HH:mm:ss"];

    NSString *time = [dateFormat stringFromDate:now];
    NSString *line = [NSString stringWithFormat:@"%@ %f %f %f %f %f\r\n", time, longuitude, latitude, x, y, radius];
    
    NSData *readData = [fileHandle readDataToEndOfFile];
    NSString *fileContents = [[NSString alloc] initWithData:readData encoding:NSUTF8StringEncoding];
    fileContents = [fileContents stringByAppendingString:line];
    
    [fileHandle writeData:[fileContents dataUsingEncoding:NSUTF8StringEncoding]];
}

- (void) saveSLLogWithHeading:(double)heading
{
    NSDate *now = [NSDate date];
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd_HH:mm:ss"];
    
    NSString *time = [dateFormat stringFromDate:now];
    NSString *line = [NSString stringWithFormat:@"%@ %f\r\n", time, heading];
    
    NSData *readData = [fileHandle readDataToEndOfFile];
    NSString *fileContents = [[NSString alloc] initWithData:readData encoding:NSUTF8StringEncoding];
    fileContents = [fileContents stringByAppendingString:line];
    
    [fileHandle writeData:[fileContents dataUsingEncoding:NSUTF8StringEncoding]];
}

@end
