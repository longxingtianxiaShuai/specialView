//
//  HTTPReceive.h
//  UITest
//
//  Created by daejin lee on 11. 9. 22..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol EventHTTPDelegate;

@interface HTTPReceive : NSObject {
    
    NSMutableURLRequest *mRequest;
    
    NSURLConnection *mUrlConnection;
    NSString  *mStrData;
    NSMutableData *mReceivedData;
    NSURLResponse *mNSURLResponse;

    BOOL mIsBinary;
    unsigned char *mReceivedBytes;
    
    id <EventHTTPDelegate> delegate;
    
    int mProtocol;
}

+(HTTPReceive *)Create;

- (BOOL)requestUrl:(NSString *)url isBinary:(BOOL)bIsBinary protocol:(int)nProtocol;

// const char *szString = "Good Morning~!";
// NSData *data = [NSData dataWithBytes: szString length: strlen(szString) + 1];
// NSLog(@"data is %@", data);
// NSLog(@"%d byte string is %s", [data length], [data bytes]);
- (BOOL)requestUrl2:(NSString *)url body:(NSData *)theData isBinary:(BOOL)bIsBinary protocol:(int)nProtocol;


-(NSString *)GetReceiveString;

-(unsigned char*)GetReceiveData;

-(int)GetReceiveSize;

-(void)ClearAll;

@property (nonatomic, assign) id <EventHTTPDelegate> delegate;

@end


@protocol EventHTTPDelegate
- (void) fireReceiveFinish_fromHttpReceive:(int)nProtocol isSuccess:(BOOL)bIsSuccess;
@end




