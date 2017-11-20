//
//  MALodIconInfo.m
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 6. 3..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MALodIconInfo.h"

@implementation MALodIconInfo

- (id)init
{
    self = [super init];
    
    if (self) {
        self.sizeArray = [[NSMutableArray alloc] initWithCapacity:10];
        self.typeArray = [[NSMutableArray alloc] initWithCapacity:10];
    }
    
    return self;
}


@end
