//
//  DebugOutput.h
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 5. 30..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DebugOutput : NSObject

@property (nonatomic) BOOL enable;

+ (DebugOutput *) instance;
- (void) output:(id)formatString,...;
@end
