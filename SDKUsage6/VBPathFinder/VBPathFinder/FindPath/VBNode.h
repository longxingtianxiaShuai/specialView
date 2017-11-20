//
//  Node.h
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import <Foundation/Foundation.h>

@class VBEdge;

#define NODE_TYPE_OTHER 0
#define NODE_TYPE_ELEVATOR 1
#define NODE_TYPE_STAIR 2
#define NODE_TYPE_SHUTTLE 3
#define NODE_TYPE_GATE 4

#define NODE_ZONE_PARKING_LOT 10
#define NODE_ZONE_LANDSIDE_TERMINAL 30
#define NODE_ZONE_AIRSIDE_TERMINAL 50
#define NODE_ZONE_TRAFFIC_CENTER 70
#define NODE_ZONE_PASSENGER_TERMINAL 90

@interface VBNode : NSObject

@property (strong, nonatomic) NSNumber *nodeId;
@property (strong, nonatomic) NSString *name;
@property (nonatomic) double x;
@property (nonatomic) double y;
@property (nonatomic) int z;
@property (nonatomic) int zone;
@property (nonatomic) int type;
@property (nonatomic) int level;

@property (strong, nonatomic) NSMutableArray *edges;

- (id) initWithNodeId: (int) nodeId name:(NSString *) name x: (double) x y: (double) y z: (int) z level:(int)level zone: (int) zone type: (int) type;

- (BOOL) isReachableFrom: (VBNode *) otherNode;

- (VBNode *) otherNode: (VBEdge *) edge;

- (NSComparisonResult) compare: (VBNode *) otherNode;

@end
