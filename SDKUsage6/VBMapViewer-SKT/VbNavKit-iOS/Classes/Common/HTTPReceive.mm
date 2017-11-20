//
//  HTTPReceive.m
//  UITest
//
//  Created by daejin lee on 11. 9. 22..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import "HTTPReceive.h"


@implementation HTTPReceive

@synthesize delegate;				// 매우 중요하다



+(HTTPReceive *)Create
{

	HTTPReceive *pReceive = [[HTTPReceive alloc]init];
	
    return pReceive;
}




- (BOOL)requestUrl:(NSString *)url isBinary:(BOOL)bIsBinary protocol:(int)nProtocol
{
    
    [self ClearAll ];
    
    mProtocol = nProtocol;
    
    mIsBinary = bIsBinary;
    
    mRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url] 
                                                           cachePolicy:NSURLRequestUseProtocolCachePolicy 
                                                       timeoutInterval:15.0];
    
    // GET 방식
    [mRequest setHTTPMethod:@"GET"];
    
    mUrlConnection = [[NSURLConnection alloc] initWithRequest:mRequest delegate:self];
    
    if ( mUrlConnection ) { 
        mReceivedData = [[NSMutableData data] retain]; // 수신할 데이터를 받을 공간을 마련
        
        //Temp-NSLog(@"protocol: %d requestUrl Success!!", mProtocol );
        
        return YES;
    }
    
    //Temp-NSLog(@"HTTPReceive: %d", __LINE__ );        
    
    return NO;
    
}


- (BOOL)requestUrl2:(NSString *)url body:(NSData *)theData isBinary:(BOOL)bIsBinary protocol:(int)nProtocol
{
    
    [self ClearAll ];
    
    
    mProtocol = nProtocol;
    
    mIsBinary = bIsBinary;
    
    mRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url] 
                                       cachePolicy:NSURLRequestUseProtocolCachePolicy 
                                   timeoutInterval:15.0];
    

    [mRequest setHTTPMethod:@"POST"];
    
//     NSString *postLength = [NSString stringWithFormat:@"%d", [theData length]];
    NSString *postLength = [NSString stringWithFormat:@"%d", [ theData length]];
    [mRequest setValue: postLength forHTTPHeaderField:@"Content-Length"];
    
    //Temp-NSLog(@"%d byte string is %s", [theData length], [theData bytes]);
    
    [mRequest setHTTPBody:theData];
    
    mUrlConnection = [[NSURLConnection alloc] initWithRequest:mRequest delegate:self];
    
    if ( mUrlConnection ) { 
        mReceivedData = [[NSMutableData data] retain]; // 수신할 데이터를 받을 공간을 마련
        
        //Temp-NSLog(@"protocol: %d requestUrl2 Success!!", mProtocol );
        
        return YES;
    }
    return NO;
    
    
/*
    NSString *post = @"\r\n bldgId=coex\r\n";
    NSData *postData = [post dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:YES];
    NSString *postLength = [NSString stringWithFormat:@"%d", [postData length]];
    
    [mRequest setHTTPMethod:@"POST"];
    [mRequest setValue: postLength forHTTPHeaderField:@"Content-Length"];
    [mRequest setValue:@"application/x-wwww-form-urlencode" forHTTPHeaderField:@"content-type"];
    
    [mRequest setHTTPBody:postData];
    
    
    mUrlConnection = [[NSURLConnection alloc] initWithRequest:mRequest delegate:self];
    
    if ( mUrlConnection ) { 
        mReceivedData = [[NSMutableData data] retain]; // 수신할 데이터를 받을 공간을 마련
        
        NSLog(@"requestUrl Success!!" );
        
        return YES;
    }
    return NO;
    */
}



- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    [ mReceivedData appendData:data ]; 
    
//    NSLog(@"didReceiveData Success!!" );
}


- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)aResponse {
    [ mReceivedData setLength:0 ]; 
    mNSURLResponse = aResponse;
    
  //  NSLog(@"didReceiveResponse Success!!" );
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    //Temp-NSLog(@"HTTPReceive Error : %@", [error localizedDescription]);
    
    //[self.delegate fireReceiveFinish_fromHttpReceive];
    [self.delegate fireReceiveFinish_fromHttpReceive: mProtocol isSuccess: FALSE ];
}

-(NSString *)GetReceiveString
{
    return mStrData;
}

-(unsigned char*)GetReceiveData
{
    return mReceivedBytes;
}

-(int)GetReceiveSize
{
    return [mReceivedData length];
}



//모든 DATA 를 받았을 때 호출 됩니다.
- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    
    //받은 receivedData 를 UTF8의 TEXT 형태로 바꿔 줍니다.
  //  mStrData = [[NSString alloc] initWithData:mReceivedData encoding:NSUTF8StringEncoding];
    
    if( mIsBinary == FALSE )
    {
        NSString *strTemp = [[NSString alloc] initWithData:mReceivedData encoding:NSUTF8StringEncoding];
        
        mStrData = [strTemp stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        
        [strTemp release];
    }
    
    //파서 를 붙인다면 여기다 붙이세요 receivedData 를 가지고 붙이시면 됩니다.
    
    [mUrlConnection release];
     mUrlConnection = nil;
    
    mReceivedBytes = (unsigned char*)[mReceivedData mutableBytes];
//    int nLength = [mReceivedData length];
    
    //Temp-NSLog(@"Length: %d", nLength );
    
    //[self.delegate fireReceiveFinish_fromHttpReceive];
    [self.delegate fireReceiveFinish_fromHttpReceive:mProtocol isSuccess: TRUE ];
        
    
    //Temp-NSLog(@"connectionDidFinishLoading  Success!!" );
}



-(void)ClearAll
{
    //Temp-NSLog(@"HTTPReceiver -- ClearAll : self:%@", self );
    
    
    if(mReceivedData )
    {
        [mReceivedData release];
        mReceivedData = nil;
    }
    
    //Temp-NSLog(@"Line: %d mReceivedData release", __LINE__ );
        
    /*
    if( mStrData )
    {
        [mStrData release];
        mStrData = nil;
    }
    
    NSLog(@"Line: %d mStrData release", __LINE__ );
     */

    /*
    if( mNSURLResponse )
    {
        [mNSURLResponse release];
        mNSURLResponse = nil;
    }
     */

    if( mUrlConnection )
    {
        [mUrlConnection release];
        mUrlConnection = nil;
    }
    
    
    //Temp-NSLog(@"Line: %d mUrlConnection release", __LINE__ );
    
    /*
    if( mRequest )
    {
        [mRequest release];
        mRequest = nil;
    }
     */
    
}


-(id)init
{
	self = [super init];
    
    //Temp-NSLog(@"HTTPReceiver -- init : self:%@", self );
    
    mReceivedBytes = nil;
    mIsBinary = FALSE;
    
    mProtocol = -1;
    
	return self;
}


-(void)dealloc
{
    //Temp-NSLog(@"HTTPReceive -- Dealloc!!  self:%@", self);
    [self ClearAll];

    [super dealloc];
}



@end
