//
//  MAStateManager.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 22..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MAStateManager.h"

@implementation MAStateManager

-(id)init
{
    self = [super init];
    
    if (self) {
        self.isZone = NO;
#if TARGET_IPHONE_SIMULATOR
        self.downloadPath = @"/Users/VB";
#else
        self.downloadPath = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
#endif

    }
    
    return self;
}

+(MAStateManager*)Instance
{
    static dispatch_once_t dispatchOnce;
    static MAStateManager* instance = nil;
    
    dispatch_once(&dispatchOnce, ^{
        instance=[[MAStateManager alloc] init];
    });
    
    return instance;
}

@end
