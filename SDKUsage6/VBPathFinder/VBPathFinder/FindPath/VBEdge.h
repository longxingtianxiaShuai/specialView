//
//  Edge.h
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import <Foundation/Foundation.h>

@class VBNode;

#define EDGE_TYPE_OTHER 0
#define EDGE_TYPE_ELEVATOR 1
#define EDGE_TYPE_STAIR 2
#define EDGE_TYPE_SHUTTLE 3
#define EDGE_TYPE_GATE 4

@interface VBEdge : NSObject

@property (nonatomic, strong)NSNumber *edgeId;
@property (nonatomic)double weight;

/* weak: nodes are set to nil when actual objects are dealloc'd */
@property (assign, nonatomic) VBNode *firstNode;
@property (assign, nonatomic) VBNode *secondNode;

@property (nonatomic)int type;
@property (nonatomic)BOOL isOneWay;
@property (nonatomic)int rest;

- (id) initWithEdgeId: (int) edgeId weight:(int) weight
            firstNode: (VBNode *) firstNode secondNode: (VBNode *) secondNode isOneWay: (BOOL) isOneWay rest:(int)rest;

- (NSComparisonResult) compare: (VBEdge *)otherEdge;
- (BOOL) hasNode: (VBNode *) node;


@end
