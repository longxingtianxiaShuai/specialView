//
//  VBPathFinder.h
//  VBPathFinder
//
//  Created by Justin on 2014. 6. 20..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum { eText = 0, eGml } fileType;

@class MAVector;
@class PathNode;

@interface VBPathFinder : NSObject

- (id)initWithFile:(NSString *)filePath withType:(fileType)type;
//- (NSArray *)getPathListFrom:(MAVector *)fromPos ToDestination:(MAVector *)toPos;
- (PathNode*)getClosestNode:(MAVector*)position WithNodeList:(NSArray*)NodeList;
- (PathNode*)getClosestNode:(MAVector*)position;
- (MAVector*)getClosestPositionOnEdge:(MAVector*)position;
- (MAVector*)getClosestPositionOnEdge:(MAVector*)position withEdgeList:(NSArray*)EdgeList;

- (PathNode*)getNodeWithId:(int)nodeId;
- (NSArray*)getPathListWithPoiIds:(NSArray*)pois withLastPosition:(MAVector*)lastPos option:(int)option;
- (NSArray*)getEdgeListFromNodeList:(NSArray*)nodeList;
- (float)getTotalDistance:(NSArray *)edgeList;
- (float)getDistanceFromNode:(PathNode*)from To:(MAVector *)to;
- (NSArray *)getNodeList;
- (NSArray *)getEdgeList;
- (NSNumber *) getRestrictBit;

@end
