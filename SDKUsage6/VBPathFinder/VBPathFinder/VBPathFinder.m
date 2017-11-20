//
//  VBPathFinder.m
//  VBPathFinder
//
//  Created by Justin on 2014. 6. 20..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "VBPathFinder.h"
#import "PathTopology.h"
#import "PathNode.h"

@implementation VBPathFinder {
    PathTopology *topology;
}

- (id)initWithFile:(NSString *)filePath withType:(fileType)type
{
    self=[super init];
    if (self) {
        topology = [[PathTopology alloc] initWithFile:filePath withType:type];
    }
    return self;

}
/*
- (NSArray *)getPathListFrom:(MAVector *)fromPos ToDestination:(MAVector *)toPos
{
    if (topology) {
       return [topology PathListFrom:fromPos ToDestination:toPos];
    }
    return nil;
}*/

- (PathNode*)getClosestNode:(MAVector*)position WithNodeList:(NSArray*)NodeList
{
    if (topology) {
        return [topology ClosestNode:position WithNodeList:NodeList];
    }
    return nil;
}
- (PathNode*)getClosestNode:(MAVector*)position
{
    if (topology) {
        return [topology ClosestNode:position];
    }
    return nil;
}
- (MAVector*)getClosestPositionOnEdge:(MAVector*)position
{
    if (topology) {
        return [topology ClosestPositionOnEdge:position];
    }
    return nil;
}
- (MAVector*)getClosestPositionOnEdge:(MAVector*)position withEdgeList:(NSArray*)EdgeList
{
    if (topology) {
        return [topology ClosestPositionOnEdge:position withEdgeList:EdgeList];
    }
    return nil;
}

- (NSNumber *) getRestrictBit
{
    if (topology) {
        return topology.restrictBit;
    }
    
    return [NSNumber numberWithInt:0];
}

- (PathNode*)getNodeWithId:(int)nodeId
{
    if (topology) {
        return [topology NodeWithId:nodeId];
    }
    return nil;
}
- (NSArray*)getPathListWithPoiIds:(NSArray*)pois withLastPosition:(MAVector*)lastPos option:(int)option
{
    if (topology) {
        return [topology PathListWithPoiIds:pois withLastPosition:lastPos option:option];
    }
    return nil;
}

- (NSArray*)getEdgeListFromNodeList:(NSArray*)nodeList
{
    if (topology) {
        return [topology EdgeListFromNodeList:nodeList];
    }
    return nil;
}

- (float)getTotalDistance:(NSArray *)edgeList
{
    if (topology) {
        return [topology TotalDistance:edgeList];
    }
    return 0.0;
}

- (float)getDistanceFromNode:(PathNode*)from To:(MAVector *)to
{
    if (topology) {
        return [topology DistanceFromNode:from To:to];
    }
    return 0.0f;
}

-(NSArray *)getNodeList
{
    if (topology) {
        return [topology getNodeList];
    }
    return nil;
}

-(NSArray *)getEdgeList
{
    if (topology) {
        return [topology getEdgeList];
    }
    return nil;
}

@end
