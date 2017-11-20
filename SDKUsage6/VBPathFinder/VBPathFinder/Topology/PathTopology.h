//
//  PathTopology.h
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 11. 28..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>


@class PathNode;
@class MAVector;


@interface PathTopology : NSObject

@property (nonatomic, strong) NSArray* nodeList;
@property (nonatomic, strong) NSArray* edgeList;
@property (nonatomic, strong) NSNumber *restrictBit;

-(id)initWithFile:(NSString *)filePath withType:(int)type;
-(PathNode*)ClosestNode:(MAVector*)position WithNodeList:(NSArray*)mynodeList;
-(PathNode*)ClosestNode:(MAVector*)position;
-(MAVector*)ClosestPositionOnEdge:(MAVector*)position;
-(MAVector*)ClosestPositionOnEdge:(MAVector*)position withEdgeList:(NSArray*)myEdgeList;

-(NSArray*)PathListFrom:(MAVector*)fromPos ToDestination:(MAVector*)toPos option:(int)option;
-(NSArray*)EdgeListFromNodeList:(NSArray*)mynodeList;
-(float)TotalDistance:(NSArray*)edgeList;
-(PathNode*)NodeWithId:(int)nodeId;
-(NSArray*)PathListWithPoiIds:(NSArray*)pois withLastPosition:(MAVector*)lastPos option:(int)option;
-(float)DistanceFromNode:(PathNode*)from To:(MAVector *)to;
-(NSArray *)getNodeList;
-(NSArray *)getEdgeList;
@end
