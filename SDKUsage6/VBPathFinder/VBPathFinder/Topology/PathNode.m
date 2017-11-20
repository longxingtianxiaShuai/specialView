//
//  PathNode.m
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 11. 28..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import "PathNode.h"
#import "VBNode.h"

@implementation PathNode

-(id)initWithNodeInfo:(VBNode*)info
{
    self=[super init];
    
    if(self)
    {
        _nodeId=[info.nodeId intValue];
        _x=info.x;
        _y=info.y;
        _level = info.level;
        _floor=info.z;
        _type=info.type;
    }
    
    return self;
}

-(id)initWithTextInfo:(NSString*)textInfo
{
    self=[super init];
    
    if(self)
    {
        NSArray* nodeInfo=[textInfo componentsSeparatedByString:@","];
        
        _nodeId=[[nodeInfo objectAtIndex:0]intValue];
        _x=[[nodeInfo objectAtIndex:2]doubleValue];
        _y=[[nodeInfo objectAtIndex:3]doubleValue];
        _floor=[[nodeInfo objectAtIndex:4]intValue];
        _type=[[nodeInfo objectAtIndex:6]intValue];
    }
    
    return self;
}

@end
