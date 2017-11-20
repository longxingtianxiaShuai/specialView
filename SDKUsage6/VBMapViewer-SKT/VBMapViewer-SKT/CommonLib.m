//
//  CommonObjects.m
//  nGinVideo
//
//  Created by 헌주 황 on 12. 3. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "CommonLib.h"

@implementation CommonLib
+(NSString*)GetTimeTextBySec:(NSInteger) duration
{
    int hour=duration/(60*60);
    int min=duration%(60*60)/60;
    int sec=duration%60;
    
    if(sec>=60)
    {
        sec-=60;
        min++;
    }
    if(min>=60)
    {
        min-=60;
        hour++;
    }
    
    return [NSString stringWithFormat:@"%02d:%02d:%02d",hour,min,sec];
}

+(NSString*)GetFileSizeTextByByte:(unsigned long long) fileSize
{
    NSString* fileSizeText;
    
    if(fileSize<1024)
        fileSizeText=[NSString stringWithFormat:@"%dBytes",(int)fileSize];    
    else if(fileSize<1024*1024)
        fileSizeText=[NSString stringWithFormat:@"%dKB",(int)(fileSize/1024)];    
    else if(fileSize<1024*1024*1024)
        fileSizeText=[NSString stringWithFormat:@"%dMB",(int)(fileSize/(1024*1024))];  
    else
        fileSizeText=[NSString stringWithFormat:@"%.2fGB",fileSize/(float)(1024*1024*1024)];
    
    return fileSizeText;
}

+(NSArray*)GetFileList:(NSString*)path withFileExt:(NSString*)fileExt
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSArray *dirContents = [fm contentsOfDirectoryAtPath:path error:nil];
    
    NSString* filterFormat=[NSString stringWithFormat:@"self ENDSWITH '.%@'",fileExt];
    NSPredicate *fltr = [NSPredicate predicateWithFormat:filterFormat];
    NSArray *onlyJPGs = [dirContents filteredArrayUsingPredicate:fltr];
    
    return onlyJPGs;    
}

+(NSMutableArray*)GetFolderList:(NSString*)path
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSArray *dirContents = [fm contentsOfDirectoryAtPath:path error:nil];
    
    if(!dirContents)
        return nil;
    
    NSMutableArray* arr=[[NSMutableArray alloc]init];
    
    for(int i=0;i<dirContents.count;++i)
    {
        NSString* newPath=[path stringByAppendingPathComponent:[dirContents objectAtIndex:i]];
        
        NSDictionary* dir=[fm attributesOfItemAtPath:newPath error:nil];
        
        if(NSFileTypeDirectory==[dir objectForKey:NSFileType])
        {
            [arr addObject:[dirContents objectAtIndex:i]];
        }
    }
    
    return arr;
}

+(void)CreateFolder:(NSString*)path
{
    if(![[NSFileManager defaultManager] fileExistsAtPath:path])
    {
        [[NSFileManager defaultManager] createDirectoryAtPath:path
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
}

+(unsigned long long)GetFileSize:(NSString*)filePathName
{
    NSDictionary *fileAttributes = [[NSFileManager defaultManager] attributesOfItemAtPath:filePathName error:nil];
    
    return [[fileAttributes objectForKey:NSFileSize] unsignedLongLongValue];
}

+(unsigned long long)GetFreeSpace
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);  
    NSDictionary *dictionary = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[paths lastObject] error: nil];  
    
    NSNumber *freeFileSystemSizeInBytes = [dictionary objectForKey:NSFileSystemFreeSize];
    
    return freeFileSystemSizeInBytes.unsignedLongLongValue;
}

+(NSString*)ReplaceExtension:(NSString*)fileName withExt:(NSString*)extension
{
    return [[fileName stringByDeletingPathExtension]stringByAppendingPathExtension:extension];
}

+(void)DeleteFile:(NSString*)fileName
{
    [[NSFileManager defaultManager]removeItemAtPath:fileName error:nil];
}

+(NSString*)CreateUUID
{
    CFUUIDRef uuid=CFUUIDCreate(kCFAllocatorDefault);
    CFStringRef uuidStr=CFUUIDCreateString(kCFAllocatorDefault,uuid);
    CFRelease(uuid);
    
    return [CommonLib ConvertCFString:uuidStr];
}

+(NSString*)ConvertCFString:(CFStringRef)str
{
    NSString* ret;
    
    if(str)
    {
        ret=[NSString stringWithString:(__bridge NSString*)str];
        CFRelease(str);
    }
    else 
    {
        ret=@"";
    }
    
    return ret;
}

+(NSString*)CStringToNSString:(const char*)charPtr
{
    return charPtr?[NSString stringWithUTF8String:charPtr]:@"";
}

+(NSString*)InternalIdValueToExternalIdValue:(int)idnum type:(NSString*)type
{
    
    NSString *strZoneId = [NSString stringWithFormat:@"%d", idnum];
    
    int strSize = (int)[strZoneId length];
    int zeroCount = 0;
    
    if (strSize == 1) {
        zeroCount = 3;
    }
    else if (strSize == 2) {
        zeroCount = 2;
    }
    else if (strSize == 3) {
        zeroCount = 1;
    }
    else if (strSize == 4) {
        zeroCount = 0;
    }
    else
    {
        NSAssert(NO, @"ID는 1~4자리입니다");
    }
    
    for (int i=0; i<zeroCount; i++) {
        strZoneId = [NSString stringWithFormat:@"0%@",strZoneId];
    }
    
    strZoneId = [NSString stringWithFormat:@"%@%@",type,strZoneId];
    
    //NSLog(@"strZoneId : %@", strZoneId);
    
    return strZoneId;
}

+(int)ExternalIdValueToInternalIdValue:(NSString*)idValue
{
    NSString *strId = [idValue substringWithRange:NSMakeRange(1, [idValue length]-1)];
    return [strId intValue];
}

+(NSString*)removeDashInVersionString:(NSString*)versionString
{
    int dashLocation = (int)[versionString rangeOfString:@"-"].location;
    
    NSString *strFirst = [versionString substringWithRange:NSMakeRange(0, dashLocation)];
    NSString *strLast = [versionString substringWithRange:NSMakeRange(dashLocation+1, [versionString length]-(dashLocation+1))];
    
    return [NSString stringWithFormat:@"%@%@",strFirst,strLast];
}

+(UIButton*)createButton:(UIButtonType)style size:(CGRect)rect action:(SEL)action normalImageName:(NSString*)normalName highlightImageName:(NSString*)highlightName target:(NSObject*)targetObj {
    UIButton *btn = [UIButton buttonWithType:style];
    btn.frame = rect;
    [btn addTarget:targetObj action:action forControlEvents:UIControlEventTouchUpInside];
    [btn setImage:[UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:normalName ofType:nil]] forState:UIControlStateNormal];
    [btn setImage:[UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:highlightName ofType:nil]] forState:UIControlStateHighlighted];
    //[btn setImage:[UIImage imageWithContentsOfFile:normalName] forState:UIControlStateNormal];
    //[btn setImage:[UIImage imageWithContentsOfFile:highlightName] forState:UIControlStateHighlighted];
    
    return btn;
}

+(UIImageView*)createUIImgeView:(CGRect)rect PngImageName:(NSString*)name {
    UIImageView *iv = [[UIImageView alloc] initWithFrame:rect];
    iv.image = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:name ofType:nil]];
    //iv.image = [UIImage imageWithContentsOfFile:name];
    return iv;
}
@end