//
//  PathTopology.m
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 11. 28..
//  Copyright (c) 2012년 dev4. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "PathTopology.h"
#import "PathNode.h"
#import "PathEdge.h"
#import "MathLinear.h"
#import "Dijkstra.h"
#import "VBNode.h"
#import "MAPoi.h"
#import "MAVector.h"
#import "TopologyBuilder.h"
#import "VBEdge.h"

@implementation PathTopology
{
    TopologyBuilder *builder;
}

-(id)initWithFile:(NSString *)filePath withType:(int)type
{
    self=[super init];
    if(self)
    {
        builder = [[TopologyBuilder alloc] initWithFile:filePath withType:type nodeFileList:nil edgeFileList:nil];
        self.nodeList=[self LoadNodes:type];
        self.edgeList=[self LoadEdges:type];
        self.restrictBit = builder.restrict_bit;
        
    }
    return self;
}



-(NSArray*)LoadNodes:(int)type
{
    NSMutableArray* contentsList=[[NSMutableArray alloc]initWithCapacity:10];
    
    if (type == 0) {     //text
        NSString *fileContents = [NSString stringWithContentsOfFile:[builder.filePath stringByAppendingString:@"node.txt"] encoding:NSASCIIStringEncoding error:nil];    // Justin
        
        NSArray* textNodeList = [fileContents componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
        
        for(NSString* textNodeInfo in textNodeList)
        {
            if(textNodeInfo.length==0)
                continue;
            
            [contentsList addObject:[[PathNode alloc] initWithTextInfo:textNodeInfo]];
        }
        return contentsList;
    } else if (type == 1) {  // gml
        for(VBNode* info in [[builder getNodeCache] allValues])
        {
            [contentsList addObject:[[PathNode alloc] initWithNodeInfo:info]];
            
        }
        return contentsList;
    }
    return nil;
}

-(NSArray*)LoadEdges:(int)type
{
    NSMutableArray* contentsList=[[NSMutableArray alloc]initWithCapacity:10];
    if (type == 0) {    // txt
        NSString *fileContents = [NSString stringWithContentsOfFile:[builder.filePath stringByAppendingString:@"edge.txt"] encoding:NSASCIIStringEncoding error:nil];      // Justin
        
        NSArray* textNodeList = [fileContents componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
        
        for(NSString* textNodeInfo in textNodeList)
        {
            if(textNodeInfo.length==0)
                continue;
            
            [contentsList addObject:[[PathEdge alloc]initWithTextInfo:textNodeInfo]];
        }
        
        return contentsList;
    } else if (type == 1) {    // gml
        for(VBEdge* info in [[builder getEdgeCache] allValues])
        {
            [contentsList addObject:[[PathEdge alloc]initWithEdgeInfo:info]];
        }
        return contentsList;
    }
    return nil;
}

-(float)DistanceFromNode:(PathNode*)from To:(MAVector *)to
{
    Point3D start;
    start.X=from.x;
    start.Y=from.y;
    start.Z=from.floor;
    
    Point3D end;
    end.X=to.x;
    end.Y=to.y;
    end.Z=to.z;
    
    return [MathLinear Distance:&start Point2:&end];
}


-(PathNode*)ClosestNode:(MAVector*)position WithNodeList:(NSArray*)mynodeList
{
    PathNode* closestNode=nil;
    double closestNodeDistance=0;
    
    for(PathNode* node in self.nodeList)
    {
        if(node.floor!=(int)position.z)
            continue;
        
        double nodeDistance=[self NodeDistance:node withPosition:position];
        
        if(closestNode==nil)
        {
            closestNode=node;
            closestNodeDistance=nodeDistance;
            
            continue;
        }
        
        if(closestNodeDistance>nodeDistance)
        {
            closestNode=node;
            closestNodeDistance=nodeDistance;
        }
    }
    
    return closestNode;
}
-(PathNode*)ClosestNode:(MAVector*)position
{
    return [self ClosestNode:position WithNodeList:nil];
}

-(PathNode*)ClosestNode:(MAVector*)position WithNodeList:(NSArray*)mynodeList WithNodeList:(NSArray*)myedgeList
{
    PathNode* closestNode=nil;
    double closestNodeDistance=0;
    
    for(PathNode* node in mynodeList)
    {
        if(node.floor!=(int)position.z)
            continue;
        
        double nodeDistance=[self NodeDistance:node withPosition:position];
        
        if(closestNode==nil)
        {
            closestNode=node;
            closestNodeDistance=nodeDistance;
            
            continue;
        }
        
        if(closestNodeDistance>nodeDistance)
        {
            for(PathEdge* edge in myedgeList)
            {
                if(edge.startNode==node.nodeId||edge.endNode==node.nodeId)
                {
                    closestNode=node;
                    closestNodeDistance=nodeDistance;
                    
                    break;
                }
            }
        }
    }
    
    return closestNode;
    
}

-(double)NodeDistance:(PathNode*)node withPosition:(MAVector*)position
{
    float a= ABS(position.x-node.x);
    float b= ABS(position.y-node.y);

    return sqrt(a*a+b*b);
}

-(PathNode*)NodeWithId:(int)nodeId
{
    for(PathNode* node in self.nodeList)
    {
        if(node.nodeId==nodeId)
            return node;
    }
    
    NSAssert(NO,@"반드시 하나는 있어야 한다. 없다면 node.txt에서 오류다.");
    
    return nil;
}

//입력된 위치(position)에서 제일 가까운 노드를 구하고 그와 연결된 엣지를 구한 후,
//position에서 제일 가까운 엣지 상의 위치를 구한다.
-(MAVector*)ClosestPositionOnEdge:(MAVector*)position withEdgeList:(NSArray*)myEdgeList
{
    PathNode* sourceNode=[self ClosestNode:position WithNodeList:self.nodeList WithNodeList:myEdgeList];
    
    Point3D closestPoint;
    {
        closestPoint.X=0;
        closestPoint.Y=0;
        closestPoint.Z=0;
        
        float minDistance=MAXFLOAT;
        
        Point3D orgPoint=[self Point3DFromIDPFPosition:position];
        Point3D sourcePoint=[self Point3DFromPathNode:sourceNode];
        
        if(orgPoint.Z!=sourcePoint.Z)
            return nil;
        
        NSArray* myNodeList=[self LinkedNodesFromNode:sourceNode WithEdge:myEdgeList];
        
        for(PathNode* node in myNodeList)
        {
            if(node.floor!=(int)position.z)
                continue;
            
            Point3D targetPoint=[self Point3DFromPathNode:node];
            
            Point3D targetPos=[MathLinear ClosestPointOnLine:&orgPoint LineStart:&sourcePoint LineEnd:&targetPoint];
            
            float distance=[MathLinear Distance:&orgPoint Point2:&targetPos];
            
            if(minDistance>distance)
            {
                closestPoint=targetPos;
                minDistance=distance;
            }
        }
    }
    
    if(closestPoint.Z==0)
        return nil;
    
    MAVector* newPos=[[MAVector alloc]init];
    {
        newPos.x=closestPoint.X;
        newPos.y =closestPoint.Y;
        newPos.z=closestPoint.Z;
    }
    
    return newPos;
}

-(NSArray*)LinkedNodesFromNode:(PathNode*)sourceNode WithEdge:(NSArray*)myEdgeList
{
    NSMutableArray* myNodeList=[[NSMutableArray alloc]init];
    
    for(PathEdge* edge in myEdgeList)
    {
        if(edge.startNode==sourceNode.nodeId)
        {
            [myNodeList addObject:[self NodeWithId:edge.endNode]];
        }
        else if(edge.endNode==sourceNode.nodeId)
        {
            [myNodeList addObject:[self NodeWithId:edge.startNode]];
        }
    }
    
    return myNodeList;
}

-(MAVector*)ClosestPositionOnEdge:(MAVector*)position
{
    return [self ClosestPositionOnEdge:position withEdgeList:self.edgeList];
}

-(Point3D)Point3DFromIDPFPosition:(MAVector*)idpfPosition
{
    Point3D orgPoint;
    orgPoint.X=idpfPosition.x;
    orgPoint.Y=idpfPosition.y;
    orgPoint.Z=idpfPosition.z;
    
    return orgPoint;
}

-(Point3D)Point3DFromPathNode:(PathNode*)node
{
    Point3D point;
    point.X=node.x;
    point.Y=node.y;
    point.Z=(float)node.floor;
    
    return point;
}

-(NSArray*)PathListFrom:(MAVector*)fromPos ToDestination:(MAVector*)toPos option:(int)option
{
    //NSLog(@"PathListFrom!!!");
    
    PathNode* currentNode=[self ClosestNode:fromPos];
    PathNode* destinationNode=[self ClosestNode:toPos];
    
    //!!
    Topology *pair = [builder buildFromSrcId:currentNode.nodeId
                                    toDestId:destinationNode.nodeId];
    
    NSLog(@"%@,%f,%f,%d", pair.src.nodeId, pair.src.x, pair.src.y,pair.src.z);
    for (VBEdge *e in pair.src.edges) {
        NSLog(@"%@,%f", e.edgeId, e.weight);
    }
    
    NSLog(@"%@,%f,%f,%d", pair.dest.nodeId, pair.dest.x, pair.dest.y,pair.dest.z);
    for (VBEdge *e in pair.dest.edges) {
        NSLog(@"%@,%f", e.edgeId, e.weight);
    }
    
    if (!pair)
        return nil;
    
    NSMutableArray* mynodelist=[[NSMutableArray alloc]init];
    {
        NSArray* path= [Dijkstra findBestPathFrom:pair.src To:pair.dest option:option];
        for(int i=0;i<path.count;++i)
        {
            VBNode* node= [path objectAtIndex:i];
            
            [mynodelist addObject:[self NodeWithId:node.nodeId.intValue]];
        }
    }
    
    return mynodelist;
}

-(NSArray*)EdgeListFromNodeList:(NSArray*)mynodeList
{
    NSMutableArray* myEdgeList=[[NSMutableArray alloc]init];
    
    int prevNodeId=-1;
    
    for(PathNode* node in mynodeList)
    {
        if(prevNodeId==-1)
            prevNodeId=node.nodeId;
        else
        {
            bool isFound=NO;
            
            for(PathEdge* edge in self.edgeList)
            {
                if(edge.startNode==prevNodeId&&edge.endNode==node.nodeId)
                {
                    [myEdgeList addObject:edge];
                    prevNodeId=node.nodeId;

                    isFound=YES;
                    break;
                }
                else if(edge.isBidirection)
                {
                    if(edge.startNode==node.nodeId&&edge.endNode==prevNodeId)
                    {
                        int tempId=edge.startNode;
                        edge.startNode=edge.endNode;
                        edge.endNode=tempId;
                        
                        [myEdgeList addObject:edge];
                        prevNodeId=node.nodeId;
                        
                        isFound=YES;
                        break;
                    }
                }
            }
        }
    }
    
    //NSAssert(myEdgeList.count+1==mynodeList.count,@"엣지의 갯수는 노드보다 하나 작아야 합니다.");
    /*
    if (myEdgeList.count+1!=mynodeList.count) {
        if ([[[NSLocale preferredLanguages] objectAtIndex:0] isEqualToString:@"ko"]) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"알림" message:@"엣지의 갯수는 노드보다 하나 작아야 합니다." delegate:self cancelButtonTitle:@"확인" otherButtonTitles: nil];
            alert.tag = 1;
            [alert show];
        }
        else {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Notice" message:@"The # of edge needs to be 1 less than the # of node." delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
            alert.tag = 1;
            [alert show];
        }
        
        return nil;
    }*/
    
    
    return myEdgeList;
}

-(float)TotalDistance:(NSArray*)myedgeList
{
    float totalDistance=0;
    
    for(PathEdge* edge in myedgeList)
    {
        if(edge.length>10000)
            totalDistance+=(edge.length-10000.0);
        else
            totalDistance+=edge.length;
    }
    
    return totalDistance;
}

-(NSArray*)PathListWithPoiIds:(NSArray*)pois withLastPosition:(MAVector*)lastPos option:(int)option;
{
    NSMutableArray* path=[[NSMutableArray alloc]init];
    
    MAVector* prevPos=nil;
    
    //NSString *tempString = @"";
    
    int cnt = 0;

    for(MAPoi* poi in pois)
    {
        MAVector* nextPos=[[MAVector alloc]init];
        {
            
            bool isCurrentPositoinPoi=[[NSNumber numberWithInt:poi.poi_id] isEqualToNumber:[NSNumber numberWithInt:2000000001]];
            
            if (isCurrentPositoinPoi) {
                //nextPos=[IdpfBroadcast Instance].lastPosition;
                
               // NSLog(@"PathListWithPoiIds nextPos - x:%f y:%f z:%f", nextPos.fX, nextPos.fY, nextPos.fZ);
                
               if(prevPos) {
                   [path addObjectsFromArray:[self PathListFrom:prevPos ToDestination:nextPos option:option]];
                }
                
                prevPos=nextPos;
            }
            else {
                // x,z, basefloor
                // start[0], start[2], -start[1]
                // x, floor, -y
                //MAPoi* poi= [[MAPoi alloc] init];
                nextPos.x=[poi.x doubleValue];
                nextPos.y= [poi.z doubleValue] * -1.0;
                nextPos.z=(float)poi.base_floor;    //justin - id를 가지고?????????????????????????????????????????
            }
        }
        
        if(prevPos) {
            [path removeLastObject];
            [path addObjectsFromArray:[self PathListFrom:prevPos ToDestination:nextPos option:option]];
        }
        
        prevPos=nextPos;
        
        //tempString = [NSString stringWithFormat:@"cnt:%lu, %@ %d,",(unsigned long)[poiIds count], tempString,poiId.intValue];
        
        cnt++;
    }
    
    if(lastPos)
    {
        [path removeLastObject];
        [path addObjectsFromArray:[self PathListFrom:prevPos ToDestination:lastPos option:option]];
    }
    
    if(path.count==0)
    {
        return nil;
    }

    return path;
}

- (NSArray *)getEdgeList
{
    return self.edgeList;
}

- (NSArray *)getNodeList
{
    return self.nodeList;
}

@end
