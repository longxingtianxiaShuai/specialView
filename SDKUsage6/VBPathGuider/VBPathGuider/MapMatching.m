//
//  MapMatching.m
//  VBPathGuider
//
//  Created by Justin on 2014. 8. 20..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "MapMatching.h"
#import "VBPathNode.h"
#import "VBPathEdge.h"
#import "MAVector.h"

typedef struct tagPoint3D
{
    float X, Y, Z;
}
Point3D;

@implementation MapMatching {
    NSArray *nodeList;
    NSArray *edgeList;
}

- (void) setNodeList:(NSArray *)list
{
    nodeList = list;
}

- (void) setEdgeList:(NSArray *)list
{
    edgeList = list;
}

- (MAVector3D *) mapMatching:(MAVector3D *)pos
{
    return [self closestPositionOnEdge:pos withEdgeList:edgeList];
}

-(Point3D)Point3DFromPosition:(MAVector3D *)pos
{
    Point3D orgPoint;
    orgPoint.X=pos.x;
    orgPoint.Y=pos.y;
    orgPoint.Z=pos.z;
    
    return orgPoint;
}

-(Point3D)Point3DFromPathNode:(VBPathNode*)node
{
    Point3D point;
    point.X=node.x;
    point.Y=(float)node.floor;
    point.Z=node.y;
    
    return point;
}

-(VBPathNode*)nodeWithId:(int)nodeId
{
    for(VBPathNode* node in nodeList)
    {
        if(node.nodeId==nodeId)
            return node;
    }
    
    NSAssert(NO,@"반드시 하나는 있어야 한다. 없다면 node.txt에서 오류다.");
    
    return nil;
}

-(NSArray*)linkedNodesFromNode:(VBPathNode*)sourceNode WithEdge:(NSArray*)myEdgeList
{
    NSMutableArray* myNodeList=[[NSMutableArray alloc]init];
    
    for(VBPathEdge* edge in myEdgeList)
    {
        if(edge.startNode==sourceNode.nodeId)
        {
            [myNodeList addObject:[self nodeWithId:edge.endNode]];
        }
        else if(edge.endNode==sourceNode.nodeId)
        {
            [myNodeList addObject:[self nodeWithId:edge.startNode]];
        }
    }
    
    return myNodeList;
}

-(MAVector3D *)closestPositionOnEdge:(MAVector3D*)position withEdgeList:(NSArray* )myEdgeList
{
    VBPathNode* sourceNode=[self closestNode:position WithNodeList:nodeList WithNodeList:myEdgeList];
    
    Point3D closestPoint;
    {
        closestPoint.X=0;
        closestPoint.Y=0;
        closestPoint.Z=0;
        
        float minDistance=MAXFLOAT;
        
        Point3D orgPoint=[self Point3DFromPosition:position];
        Point3D sourcePoint=[self Point3DFromPathNode:sourceNode];
        
        if(orgPoint.Y!=sourcePoint.Y)
            return nil;
        
        NSArray* myNodeList=[self linkedNodesFromNode:sourceNode WithEdge:myEdgeList];
        
        for(VBPathNode* node in myNodeList)
        {
            if(node.floor!=(int)position.y)
                continue;
            
            Point3D targetPoint=[self Point3DFromPathNode:node];
            
            Point3D targetPos=[self closestPointOnLine:&orgPoint LineStart:&sourcePoint LineEnd:&targetPoint];
            
            float distance=[self distance:&orgPoint Point2:&targetPos];
            
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

-(float)distance:(Point3D*)point1 Point2:(Point3D*)point2
{
    Point3D Vector;
    
    Vector.X = point2->X - point1->X;
    Vector.Y = point2->Y - point1->Y;
    Vector.Z = point2->Z - point1->Z;
    
    return (float)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z );
}

-(Point3D)closestPointOnLine:(Point3D*)point
                   LineStart:(Point3D*)lineStart
                     LineEnd:(Point3D*)lineEnd;
{
    Point3D closestPoint;
    
    float LineMag=[self distance:lineEnd Point2:lineStart];
    
    float U = ( ( ( point->X -lineStart->X ) * ( lineEnd->X - lineStart->X ) ) +
               ( ( point->Y - lineStart->Y ) * ( lineEnd->Y - lineStart->Y ) ) +
               ( ( point->Z - lineStart->Z ) * ( lineEnd->Z - lineStart->Z ) ) ) /
    ( LineMag * LineMag );
    
    if( U < 0.0f)
    {
        closestPoint=*lineStart;
    }
    else if(U > 1.0f)
    {
        closestPoint=*lineEnd;
    }
    else
    {
        closestPoint.X = lineStart->X + U * ( lineEnd->X - lineStart->X );
        closestPoint.Y = lineStart->Y + U * ( lineEnd->Y - lineStart->Y );
        closestPoint.Z = lineStart->Z + U * ( lineEnd->Z - lineStart->Z );
    }
    
    return closestPoint;
}


-(VBPathNode*)closestNode:(MAVector3D*)position WithNodeList:(NSArray*)mynodeList WithNodeList:(NSArray*)myedgeList
{
    VBPathNode* closestNode=nil;
    double closestNodeDistance=0;
    
    for(VBPathNode* node in mynodeList)
    {
        if(node.floor!=(int)position.y)
            continue;
        
        double nodeDistance=[self nodeDistance:node withPosition:position];
        
        if(closestNode==nil)
        {
            closestNode=node;
            closestNodeDistance=nodeDistance;
            
            continue;
        }
        
        if(closestNodeDistance>nodeDistance)
        {
            for(VBPathEdge* edge in myedgeList)
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

-(double)nodeDistance:(VBPathNode*)node withPosition:(MAVector3D*)position
{
    float a=ABS(position.x-node.x);
    float b=ABS(position.z-node.y);
    
    return sqrt(a*a+b*b);
}



@end
