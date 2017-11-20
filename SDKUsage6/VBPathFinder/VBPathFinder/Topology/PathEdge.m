//
//  PathEdge.m
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 12. 7..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import "PathEdge.h"
#import "VBEdge.h"
#import "VBNode.h"

@implementation PathEdge
-(id)initWithEdgeInfo:(VBEdge*)info;
{
    self=[super init];
    
    if(self)
    {
        //900027,90052,90054,1,1498.489938
        _edgeId=[info.edgeId intValue];
        _startNode=[info.firstNode.nodeId intValue];
        _endNode=[info.secondNode.nodeId intValue];
        _isBidirection=!info.isOneWay;
        _length=info.weight;
    }
    
    return self;
}

-(id)initWithTextInfo:(NSString*)textInfo;
{
    self=[super init];
    
    if(self)
    {
        //900027,90052,90054,1,1498.489938
        NSArray* nodeInfo=[textInfo componentsSeparatedByString:@","];
        
        _edgeId=[[nodeInfo objectAtIndex:0]intValue];
        _startNode=[[nodeInfo objectAtIndex:1]intValue];
        _endNode=[[nodeInfo objectAtIndex:2]intValue];
        _isBidirection=[[nodeInfo objectAtIndex:3]intValue];
        _length=[[nodeInfo objectAtIndex:4]floatValue];
    }
    
    return self;
}

@end