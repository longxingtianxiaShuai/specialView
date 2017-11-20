//
//  HttpComm.h
//  nGinVideo
//
//  Created by 헌주 황 on 12. 3. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HttpComm : NSObject
+(void)HTTPRequest:(NSString*)url
   withJobContinue:(BOOL*)jobContinue
    afterTaskBlock:(void (^)(NSData *data, NSInteger code))afterBlock;

+(CFHTTPMessageRef)HTTPRequestSync:(NSString*)url
                   withJobContinue:(BOOL*)jobContinue
                     withOutStream:(NSOutputStream*)outStream
                   withWrittenSize:(long long*)writtenSize
                          update:(id)update;

+(CFHTTPMessageRef)HTTPRequestSync:(NSString*)url
                   withJobContinue:(BOOL*)jobContinue;
@end
