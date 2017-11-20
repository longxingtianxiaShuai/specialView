//
//  Map.h
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreLocation/CoreLocation.h>
#import "Node.h"
#import "Link.h"
#import "SLCoordinate3D.h"
#import "NetPos.h"
#import "MapPos.h"
#import "MapPosEst.h"

@interface NetworkMap : NSObject { 
	// Date of the map data.
	NSString *dataDate;
	
	// The name of the map.
	NSString *name;
    
    NSString *mapId;
    
    NSString *mapVersionId;
    
	// Lat/long coordinates for for the local network origin
	//CLLocationCoordinate2D networkLocation;
	SLCoordinate2D* networkLocation;
    
    SLCoordinate3D* startPosition;
    SLCoordinate3D* endPosition;
	
	// Orientation of the x-axis (up) in the network-system relative north expressed in NED
	double networkOrientation; 
	
}

- (NetworkMap*) initWithJsonDictionary:(NSDictionary*) json;
- (void) createNetworkWithNodesX:(double*)nodesX
                       andNodesY:(double*)nodesY 
              andNodeDefsFloorNr:(int*)nodeDefsFloorNr 
                 andNodeDefsType:(int*)nodeDefsType 
                        ofLength:(NSUInteger)nNodes 
                       andLinks1:(int*)links1 
                       andLinks2:(int*)links2 
                       andWidths:(double*)widths
                    andLinksType:(int*)linksType 
                        ofLength:(NSUInteger)nLinks;
- (void) printMapInfo;
- (void) printNetwork;
- (void) printLinks;
- (void) printNodes;
- (SLCoordinate2D*) getLongLat;
- (bool) isSubpath;

- (MapPos*) netPos2MapPos:(NetPos*) netPos;
- (NetPos*) mapPoint2NetPos:(SLPoint3D*) mapPoint;
- (MapPosEst*) netPosArray2MapPosEst:(NSArray*) netPosArray;
- (MapPosEst*) netPosArray2MapPosEst:(NSArray*) netPosArray withWeights:(double*)weights;
- (double) magHeading2MapHeading:(double) magHeading;
- (double) mapHeading2MagHeading:(double) mapHeading;

- (void) makeNetPosFeasible:(NetPos*)netPos withHeading:(double)heading;
- (void) makeNetPosFeasible1:(NetPos*)netPos withHeading:(double)heading;
- (void) makeNetPosFeasible2b:(NetPos*)netPos withHeading:(double)heading;

- (Link*) getNewLinkFromNodeID:(int)nodeID andCurrentHeading:(double)heading andCurrentLinkID:(int)linkID;
- (int) getNodeIDForNetPosOutsideLink:(NetPos*)netPos;
- (void) updateNetPos:(NetPos*)netPos forNewLink:(Link*)newLink;

- (void) makeNetPosFeasible2:(NetPos*)netPos withHeading:(double)heading;
- (int) getNewLinkFromNode:(int)nodeID usingHeadingMeasurement:(double)heading andOldLinkID:(int) linkID;

- (void) makeNetPosFeasible:(NetPos*)netPos;

- (SLCoordinate3D*) point2LongLat:(SLPoint3D*)point;
+ (SLCoordinate3D*) point2LongLat:(SLPoint3D*)point withOrigin:(SLCoordinate2D*)origin andOrientation:(double)orientation;
- (SLPoint3D*) longLat2Point:(SLCoordinate3D*)location;
+ (SLPoint3D*) longLat2Point:(SLCoordinate3D*) location withOrigin:(SLCoordinate2D*)origin andOrientation:(double)orientation;
- (NSMutableArray*) netPosArray2LongLatArray:(NSArray*)netPosArray;

- (double) getStartHeadingBasedOnStartLinks;
- (double) getStartHeadingBasedOnLinkID:(int)startLinkID andLinkId:(int)secondLinkID;
- (double) getHeadingOfLinkID:(int) linkID;
- (double) getInitialLogHeading;
- (double) getHeadingInNetPos:(NetPos*)startNetPos towardsDistance:(double)distance;

- (NetPos*) getClosestNetPos:(SLCoordinate3D*)loc;
- (SLCoordinate3D*) projectLocationToNetwork:(SLCoordinate3D*)loc;
- (void) setEndPositionByProjection:(SLCoordinate3D*)endPosCandidate;
- (void) setStartPosition:(SLCoordinate3D*)newStartPos;

- (NSArray*) getNodeCoordinateArray;

//@property (nonatomic) Network network;
@property (nonatomic, strong) NSMutableArray *links;
@property (nonatomic, strong) NSMutableArray *nodes;
@property (readonly, strong) SLCoordinate3D* startPosition;
@property (readonly, strong) SLCoordinate3D* endPosition;
@property (readonly, strong) NSString *dataDate;
@property (readonly, strong) NSString *name;
@property (readonly, strong) NSString *mapId;
@property (readonly, strong) NSString *mapVersionId;

@end