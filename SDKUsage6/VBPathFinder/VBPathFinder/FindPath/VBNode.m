//
//  Node.m
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All* rights reserved.
//

#import "VBNode.h"
#import "VBEdge.h"

@interface VBNode()

@end

@implementation VBNode

- (id) initWithNodeId: (int) nodeId name:(NSString *) name x: (double) x y: (double) y z: (int) z level:(int)level zone: (int) zone type: (int) type
{
    if (self = [super init])
    {
        self.nodeId = [NSNumber numberWithInt:nodeId];
        self.name = name;
        self.x = x;
        self.y = y;
        self.z = z;
        self.level = level;
        self.zone = zone;
        self.type = type;
        self.edges = [[NSMutableArray alloc] initWithCapacity:1];
    }
    return self;
}

- (BOOL) isReachableFrom: (VBNode *) otherNode
{
    return YES;
}

- (VBNode *) otherNode: (VBEdge *) edge
{
    if ([self isEqual:edge.firstNode]){
        return edge.secondNode;
    } else {
        return edge.firstNode;
    }
        
    
    //if ([self isEqual:edge.secondNode]){
    //    return edge.firstNode;
   // }
    
    
   // @throw [NSException exceptionWithName:@"NSInvalidArgumentException" reason:[NSString stringWithFormat:@"Edge # %@ invalid for Node # %@", edge.edgeId, self.nodeId] userInfo:nil];
    
  //  return nil; /* to please the compiler */
    
}

- (NSComparisonResult) compare: (VBNode *) otherNode
{
    
    if (self.nodeId == otherNode.nodeId)
        return NSOrderedSame;
    if (self.nodeId < otherNode.nodeId)
        return NSOrderedAscending;
    
    return NSOrderedDescending;
}

- (NSUInteger)hash
{
    return [self.nodeId unsignedIntegerValue];
}

- (BOOL) isEqual:(id)object
{
    return self.nodeId == ((VBNode *)object).nodeId;
}

- (NSString *) description
{
    return [NSString stringWithFormat:@"Node #%d",[self.nodeId intValue]];
}

/*
- (void) dealloc
{
    for (Edge *e in self.edges)
    {
        if ([e.firstNode isEqual:self])
        {
            e.firstNode = nil;
        }
        if ([e.secondNode isEqual:self])
        {
            e.secondNode = nil;
        }
    }
}*/

@end
