//
//  Edge.m
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import "VBEdge.h"
#import "VBNode.h"

@interface VBEdge()

@end

@implementation VBEdge


static int determineEdgeType(VBNode *node1, VBNode *node2)
{
    if (node1.z != node2.z) {
        return EDGE_TYPE_ELEVATOR;
    }
    /*
    switch (node1.type) {
        case NODE_TYPE_ELEVATOR:
            if (node2.type == NODE_TYPE_ELEVATOR && node1.z != node2.z){
                return EDGE_TYPE_ELEVATOR;
            }
            break;
        case NODE_TYPE_STAIR:
            if (node2.type == NODE_TYPE_STAIR && node1.z != node2.z)
            {
                return EDGE_TYPE_STAIR;
            }
            break;
        case NODE_TYPE_SHUTTLE:
            if (node2.type == NODE_TYPE_SHUTTLE)
            {
                return EDGE_TYPE_SHUTTLE;
            }
            break;
        case NODE_TYPE_GATE:
            if (node2.type == NODE_TYPE_GATE && node1.zone != node2.zone)
            {
                return EDGE_TYPE_GATE;
            }
            break;
        default:
            break;
    }*/
    return EDGE_TYPE_OTHER;
}


- (id) initWithEdgeId: (int) edgeId weight:(int) weight
           firstNode: (VBNode *) firstNode secondNode: (VBNode *) secondNode isOneWay: (BOOL) isOneWay rest:(int)rest
{
    if (self = [super init]) {
        self.edgeId = [NSNumber numberWithInt:edgeId];
        self.weight = weight;
        self.firstNode = firstNode;
        self.secondNode = secondNode;
        self.type = determineEdgeType(firstNode,secondNode);
        self.isOneWay = isOneWay;
        self.rest = rest;
    }
    return self;
}

- (NSUInteger) hash
{
    return [self.edgeId unsignedIntegerValue];
}

- (BOOL) isEqual:(id)object
{
    return self.edgeId == ((VBEdge *)object).edgeId;
}
	
- (NSComparisonResult) compare: (VBEdge *)otherEdge
{
    if (self.weight < otherEdge.weight) {
        return NSOrderedAscending;
    }
    if (self.weight == otherEdge.weight) {
        return NSOrderedSame;
    }
    return NSOrderedDescending;
}

- (BOOL) hasNode: (VBNode *) node
{
    if ([self.firstNode isEqual:node])
        return YES;
    if ([self.secondNode isEqual:node])
        return YES;
    
    return NO;
}


@end
