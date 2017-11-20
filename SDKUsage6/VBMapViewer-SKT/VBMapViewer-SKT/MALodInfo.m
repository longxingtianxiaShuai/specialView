//
//  MALod.m
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 6. 3..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MALodInfo.h"
#import "MALodIconInfo.h"

@implementation MALodInfo

- (id)init
{
    self = [super init];
    
    if (self) {
        self.iconInfo = [[NSMutableArray alloc] initWithCapacity:10];
    }
    
    return self;
}

@end
