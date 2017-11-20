//
//  Link.h
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SLPoint3DPrivate.h"

@interface Link : NSObject {
}

- (Link*) initWithStartPoint:(SLPoint3D*)startPoint_ andEndPoint:(SLPoint3D*)endPoint_ andStartNode:(int)startNode_ andEndNode:(int)endNode_ andLinkID:(int)linkID_ andWidth:(double)width_ andType:(int) type_;
- (double) getDistanceToClosestNode:(double) distance;
- (double) getHeadingFromNode:(int) n;

/*! @property startPoint The position of the start node.*/
@property (nonatomic, strong) SLPoint3D* startPoint;

/*! @property startPoint The position of the end node.*/
@property (nonatomic, strong) SLPoint3D* endPoint;

/*! @property startNode Start node ID.*/
@property (nonatomic) int startNode;

/*! @property endNode End node ID.*/
@property (nonatomic) int endNode;

/*! @property distance The Eucledian distances from the start node to the end node.*/
@property (nonatomic) double distance;

/*! @property heading Heading of the link.*/
@property (nonatomic) double heading;

/*! @property width Width of the link (in the corridor sense).*/
@property (nonatomic) double width;

/*! @property type of link.*/
@property (nonatomic) int type;

/*! @property linkID ID of this link. */
@property (nonatomic) int linkID;

@end
