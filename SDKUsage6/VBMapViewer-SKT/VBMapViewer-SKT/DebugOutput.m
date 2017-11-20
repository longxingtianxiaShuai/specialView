//
//  DebugOutput.m
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 5. 30..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "DebugOutput.h"

@implementation DebugOutput

- (id)init
{
    self = [super init];
    
    if (self) {
        self.enable = NO;
    }
    
    return self;
}

+ (DebugOutput *) instance
{
    static dispatch_once_t dispatchOnce;
    static DebugOutput* instance = nil;
    
    dispatch_once(&dispatchOnce, ^{
        instance=[[DebugOutput alloc] init];
    });
    
    return instance;
}

- (void) output:(id)formatString,...
{
    if (self.enable) {
        va_list arglist;
        va_start(arglist, formatString);
        NSString *output = [[NSString alloc] initWithFormat:formatString arguments:arglist];
        va_end(arglist);
        NSLog(output);
    }
}
@end
