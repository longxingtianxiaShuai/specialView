//
//  HttpComm.m
//  nGinVideo
//
//  Created by 헌주 황 on 12. 3. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import "HttpComm.h"
#import <netinet/in.h>
#import <CoreFoundation/CoreFoundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import "CommonLib.h"
#import "MAUpdate.h"

@implementation HttpComm

+(void)HTTPRequest:(NSString*)url
   withJobContinue:(BOOL*)jobContinue
    afterTaskBlock:(void (^)(NSData *data, NSInteger code))afterBlock
{
    dispatch_async( dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ), ^(void)
                   {
                       NSOutputStream* outStream=[NSOutputStream outputStreamToMemory];
                       [outStream open];
                       
                       long long writtenSize=0;
                       
                       CFHTTPMessageRef response=[HttpComm HTTPRequestSync:url
                                                           withJobContinue:jobContinue
                                                             withOutStream:outStream
                                                           withWrittenSize:&writtenSize
                                                                  update:nil
                                                  ];
                       
                       NSInteger code=0;
                       
                       if(response!=nil)
                       {
                           code=CFHTTPMessageGetResponseStatusCode(response);
                           CFRelease(response);
                       }
                       
                       NSData* newData=[[outStream propertyForKey:NSStreamDataWrittenToMemoryStreamKey]copy];
                       
                       [outStream close];
                       
                       if((jobContinue&&*jobContinue)||jobContinue==nil)
                           if(afterBlock)
                           {
                               dispatch_async(dispatch_get_main_queue(), ^(void)
                                              {
                                                  afterBlock(newData, code);
                                              });
                           }
                   });
}

+(CFHTTPMessageRef)HTTPRequestSync:(NSString*)url
                   withJobContinue:(BOOL*)jobContinue
{
    NSOutputStream* outStream=[NSOutputStream outputStreamToMemory];
    [outStream open];
    
    long long writtenSize=0;
    
    CFHTTPMessageRef response=[HttpComm HTTPRequestSync:url
                                        withJobContinue:jobContinue
                                          withOutStream:outStream
                                        withWrittenSize:&writtenSize
                                            update:nil
                               ];
    
    if(response)
    {
        NSData* data=[outStream propertyForKey:NSStreamDataWrittenToMemoryStreamKey];
        
        CFHTTPMessageSetBody(response, (__bridge CFDataRef)data);
    }
    
    [outStream close];
    
    return response;
}

+(CFHTTPMessageRef)HTTPRequestSync:(NSString*)url
                   withJobContinue:(BOOL*)jobContinue
                     withOutStream:(NSOutputStream*)outStream
                   withWrittenSize:(long long*)writtenSize
                          update:(id)update
{
    NSURL *myURL = [NSURL URLWithString:url];
    
    CFHTTPMessageRef request = CFHTTPMessageCreateRequest(NULL, CFSTR("GET"), (__bridge CFURLRef)myURL, kCFHTTPVersion1_0);
    CFReadStreamRef requestStream = CFReadStreamCreateForHTTPRequest(NULL, request);
    CFReadStreamOpen(requestStream);
    
    @try
    {
        //60초 정도 접속 시도를 해보자
        for(int i=0;i<30*10;i++)
        {
            //데이터가 없는데도 YES를 리턴하는 경우가 있다.서버에서 소켓을 close하는 등등 그렇다.
            if(YES==CFReadStreamHasBytesAvailable(requestStream))
                break;
            
            if(jobContinue)
                if(*jobContinue==NO)
                    return nil;
            
            usleep(100000);
        }
        
        CFHTTPMessageRef response=(CFHTTPMessageRef)CFReadStreamCopyProperty(requestStream, kCFStreamPropertyHTTPResponseHeader);
        
        if(response)
        {
            NSString* contentLength=[CommonLib ConvertCFString:CFHTTPMessageCopyHeaderFieldValue(response, CFSTR("Content-Length"))];
            
            BOOL isHaveContentLength=contentLength.length>0;
            
            NSInteger remainedLength=isHaveContentLength?contentLength.integerValue:-1;
            
            CFIndex numBytesRead=0;
            
            do{
                UInt8 buf[4*1024];
                
                numBytesRead = CFReadStreamRead(requestStream, buf, sizeof(buf));
                
                if(numBytesRead==-1)
                    return nil;
                
                if(numBytesRead > 0)
                {
                    *writtenSize+=[outStream write:buf maxLength:numBytesRead];
                    if ([update respondsToSelector:@selector(setCurrentFileSize:)]) {
                        [(MAUpdate *)update setCurrentFileSize:numBytesRead];
                    }
                }
                
                if(isHaveContentLength)
                    remainedLength-=numBytesRead;
                
                if(jobContinue)
                    if(*jobContinue==NO)
                        return nil;
                
            } while(remainedLength>0||(isHaveContentLength==NO&&numBytesRead>0));
            
            NSAssert(isHaveContentLength==NO||contentLength.integerValue==*writtenSize, nil);
        }
        
        return response;
    }
    @finally
    {
        CFReadStreamClose(requestStream);
        CFRelease(requestStream); 
        CFRelease(request);
    }
}

@end
