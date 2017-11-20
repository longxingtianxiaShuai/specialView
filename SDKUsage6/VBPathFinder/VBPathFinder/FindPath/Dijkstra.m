//
//  Dijkstra.m
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import "Dijkstra.h"
#import "VBNode.h"
#import "VBEdge.h"
#import "PriorityQueueARC.h"

@implementation Dijkstra

+ (NSArray *) findBestPathFrom:(VBNode *)startNode To:(VBNode *)endNode option:(int)option
{
    NSArray *edgePath = [self findBestEdgePathFrom:startNode To:endNode option:option];
    
    if (!edgePath)
        return nil;
    
    NSMutableArray *nodePath = [[NSMutableArray alloc] init];
    
    [nodePath addObject:startNode];
    VBNode *curr = startNode;
    for (VBEdge *e in edgePath)
    {
        curr = [curr otherNode:e];
        [nodePath addObject:curr];
        
    }
    return nodePath;
}

+ (NSArray *) findBestEdgePathFrom:(VBNode *)startNode To:(VBNode *)endNode option:(int)option
{
    if ([endNode isReachableFrom:startNode] == NO)
    {
        return nil;
    }
    
    NSMutableArray *edgePath = [[NSMutableArray alloc] init];
    
    PriorityQueueARC *pq = [[PriorityQueueARC alloc] init];
    NSMutableDictionary *distance = [[NSMutableDictionary alloc] init];
    
    NSMutableDictionary *previousEdge = [[NSMutableDictionary alloc] init];
    
    [pq addObject:startNode withWeight:0];
    [distance setObject:[NSNumber numberWithDouble:0] forKey:startNode.nodeId];
    
    BOOL filtered = NO;
    int rest = 0;
    while (![pq isEmpty])
    {
        ObjectWeightWrapper *pair = [pq poll];
        
        VBNode *minNode = pair.object;
        if ([minNode isEqual:endNode]){
            break;
        }

        
        double dist = pair.weight;
        
        for (VBEdge *e in minNode.edges)
        {
            double alt = dist + e.weight;
            VBNode *otherNode = [minNode otherNode:e];
            if (otherNode == nil){
                continue;
            }
            
            rest = e.rest;
            
            switch (option) {
                case 0:
                    filtered = NO;
                    break;
                case 2:
                    if (rest == 4) {
                        filtered = YES;
                    } else {
                        filtered = NO;
                    }
                    break;
                case 4:
                    if (rest == 2) {
                        filtered = YES;
                    } else {
                        filtered = NO;
                    }
                    break;
                case 6:
                    filtered = NO;
                    break;
                default:
                    break;
            }
            
            if (filtered == YES) {
                continue;
            }
            
            if (![distance objectForKey:otherNode.nodeId] || alt < [[distance objectForKey:otherNode.nodeId] doubleValue])
            {
                [distance setObject:[NSNumber numberWithDouble:alt] forKey:otherNode.nodeId];
                [previousEdge setObject:e forKey:otherNode.nodeId];
                
                [pq removeObject:otherNode];
                [pq addObject:otherNode withWeight:alt];
            }
            
        }
    }
    
    VBNode *n = endNode;

    
    //VBEdge *e = [previousEdge objectForKey:n.nodeId];
    //for (VBEdge *e in n.edges) {
    //    [edgePath insertObject:e atIndex:0];
    //}
    
    
    while ([previousEdge objectForKey:n.nodeId] != nil)
    {
        VBEdge *e = [previousEdge objectForKey:n.nodeId];
        [edgePath insertObject:e atIndex:0];
        n = [n otherNode:e];
    }
    
    if ([edgePath count] == 0 || ![[edgePath objectAtIndex:0] hasNode:startNode])
    {
        return nil;
    }
    
    return edgePath;
}

@end
