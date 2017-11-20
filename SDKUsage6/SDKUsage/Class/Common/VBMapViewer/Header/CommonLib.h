//
//  CommonObjects.h
//  nGinVideo
//
//  Created by 헌주 황 on 12. 3. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface CommonLib:NSObject
+(NSString*)GetTimeTextBySec:(NSInteger) duration;
+(NSString*)GetFileSizeTextByByte:(unsigned long long) fileSize;
+(NSArray*)GetFileList:(NSString*)path withFileExt:(NSString*)fileExt;
+(NSMutableArray*)GetFolderList:(NSString*)path;
+(void)CreateFolder:(NSString*)path;
+(unsigned long long)GetFileSize:(NSString*)filePathName;
+(unsigned long long)GetFreeSpace;
+(NSString*)ReplaceExtension:(NSString*)fileName withExt:(NSString*)extension;
+(void)DeleteFile:(NSString*)fileName;
+(NSString*)CreateUUID;
+(NSString*)ConvertCFString:(CFStringRef)str;
+(NSString*)CStringToNSString:(const char*)charPtr;

+(NSString*)InternalIdValueToExternalIdValue:(int)idnum type:(NSString*)type;
+(int)ExternalIdValueToInternalIdValue:(NSString*)idValue;
+(NSString*)removeDashInVersionString:(NSString*)versionString;

// UI
+(UIButton*)createButton:(UIButtonType)style size:(CGRect)rect action:(SEL)action normalImageName:(NSString*)normalName highlightImageName:(NSString*)highlightName target:(NSObject*)targetObj;
+(UIImageView*)createUIImgeView:(CGRect)rect PngImageName:(NSString*)name;
@end