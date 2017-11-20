//
//  FileParser.m
//  DijkstraApp
//
//  Created by Junwoo Park on 11/19/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import "FileParser.h"
#import "VBNode.h"
#import "VBEdge.h"
#import "TBXML.h"

@implementation FileParser
+ (void) loadNodesFromTextFilePath: (NSString *)filePath toNodeDict: (NSMutableDictionary *)nodes
{
    NSError *err;
    NSString *fileContent = [NSString stringWithContentsOfFile:filePath encoding:NSASCIIStringEncoding error:&err];
    
    unsigned length = [fileContent length];
    unsigned lineStart = 0, lineEnd = 0, contentsEnd = 0;
    
    NSRange currentRange;
    
    while (lineEnd < length)
    {
        [fileContent getLineStart:&lineStart end:&lineEnd contentsEnd:&contentsEnd forRange:NSMakeRange(lineEnd, 0)];
        currentRange = NSMakeRange(lineStart, contentsEnd - lineStart);
        
        NSString *line = [fileContent substringWithRange:currentRange];
        
        NSArray *tokens = [line componentsSeparatedByString:@","];
        if ([tokens count] < 7)
        {
            NSException *exception = [NSException exceptionWithName:@"NSInvalidArgumentException" reason:@"Invalid File Format" userInfo:nil];
            @throw exception;
        }
        
        int nodeId;
        NSString *name;
        double x, y;
        int z, zone, type;
        
        nodeId = [[tokens objectAtIndex:0] intValue];
        name = [tokens objectAtIndex:1];
        x = [[tokens objectAtIndex:2] doubleValue];
        y = [[tokens objectAtIndex:3] doubleValue];
        
        z = [[tokens objectAtIndex:4] intValue];
        zone = [[tokens objectAtIndex:5] intValue];
        type = [[tokens objectAtIndex:6] intValue];
        
        if ([nodes objectForKey:[NSNumber numberWithInt:nodeId]] == nil)
        {
            VBNode *newNode = [[VBNode alloc] initWithNodeId:nodeId name:name x:x y:y z:z level:0 zone:zone type:type];
            [nodes setObject:newNode forKey:newNode.nodeId];
        }

    }
}

+ (void) loadEdgesFromTextFilePath: (NSString *)filePath andNodeDict: (NSDictionary *)nodes toEdgeDict: (NSMutableDictionary *)edges
{
    NSError *err;
    NSString *fileContent = [NSString stringWithContentsOfFile:filePath encoding:NSASCIIStringEncoding error:&err];
    
    unsigned length = [fileContent length];
    unsigned lineStart = 0, lineEnd = 0, contentsEnd = 0;
    
    NSRange currentRange;
    
    while (lineEnd < length)
    {
        [fileContent getLineStart:&lineStart end:&lineEnd contentsEnd:&contentsEnd forRange:NSMakeRange(lineEnd, 0)];
        currentRange = NSMakeRange(lineStart, contentsEnd - lineStart);
        
        NSString *line = [fileContent substringWithRange:currentRange];
        NSArray *tokens = [line componentsSeparatedByString:@","];
        if ([tokens count] < 5)
        {
            NSException *exception = [NSException exceptionWithName:@"NSInvalidArgumentException" reason:@"Invalid File Format" userInfo:nil];
            @throw exception;
        }
        
        int edgeId, node1Id, node2Id;
        BOOL isOneWay;
        double weight;
        
        edgeId = [[tokens objectAtIndex:0] intValue];
        
        node1Id = [[tokens objectAtIndex:1] intValue];
        node2Id = [[tokens objectAtIndex:2] intValue];
        
        VBNode *firstNode = [nodes objectForKey:[NSNumber numberWithInt:node1Id]];
        VBNode *secondNode = [nodes objectForKey:[NSNumber numberWithInt:node2Id]];
        
        if (firstNode == nil || secondNode == nil)
        {
            NSException *exception = [NSException exceptionWithName:@"NSInvalidArgumentException" reason:@"Invalid nodeid given" userInfo:nil];
            @throw exception;
        }
        
        isOneWay = ([[tokens objectAtIndex:3] intValue] == 0);
        weight = [[tokens objectAtIndex:4] doubleValue];
        
        if ([edges objectForKey:[NSNumber numberWithInt:edgeId]] == nil)
        {
            VBEdge *newEdge = [[VBEdge alloc] initWithEdgeId:edgeId weight:weight firstNode:firstNode   secondNode:secondNode isOneWay:isOneWay rest:0];
            [firstNode.edges addObject:newEdge];
            if (!isOneWay)
            {
                [secondNode.edges addObject:newEdge];
            }
            [edges setObject:newEdge forKey:newEdge.edgeId];
        }
        else
        {
            NSException *exception = [NSException exceptionWithName:@"NSInvalidArgumentException" reason:@"Duplicate Edge" userInfo:nil];
            @throw exception;
        }
    }
}

+ (NSNumber *) loadNodesAndEdgesFromGmlFilePath: (NSString *)filePath toNodeDict: (NSMutableDictionary *)nodes toEdgeDict: (NSMutableDictionary *)edges;
{
    NSNumber *resBit;
    TBXML *gml = [[TBXML alloc] initWithXMLFile:filePath error:nil];
    TBXMLElement *rootElement = gml.rootXMLElement;
    if (rootElement) {
        TBXMLElement *xDefinitionElement = [TBXML childElementNamed:@"X_definition" parentElement:rootElement];
        if (xDefinitionElement) {
            TBXMLElement *restrictElement = [TBXML childElementNamed:@"restrict" parentElement:xDefinitionElement];
            if (restrictElement) {
                TBXMLElement *restrictBitElement = [TBXML childElementNamed:@"restrict_bit" parentElement:restrictElement];
                int restrict_bit =  [[TBXML textForElement:restrictBitElement] intValue];
                resBit = [NSNumber numberWithInt:restrict_bit];
            }
        }
        
        TBXMLElement *spaceMemberElement = [TBXML childElementNamed:@"SpaceLayerMember" parentElement:rootElement];
        if (spaceMemberElement) {
            TBXMLElement *spaceLayerElement = [TBXML childElementNamed:@"SpaceLayer" parentElement:spaceMemberElement];
            
            if (spaceLayerElement) {
                TBXMLElement *XFloorElement = [TBXML childElementNamed:@"X_floor" parentElement:spaceLayerElement];
                if (XFloorElement) {
                    do {
                        NSString *floorLevel = [TBXML valueOfAttributeNamed:@"level" forElement:XFloorElement];
                        //NSLog(@"Floor = %@", floorLevel);
                        int level = [floorLevel intValue];
                        TBXMLElement *stateElement = [TBXML childElementNamed:@"state" parentElement:XFloorElement];
                        if (stateElement) {
                            do {
                                TBXMLElement *StateElement = [TBXML childElementNamed:@"State" parentElement:stateElement];
                                if (StateElement) {
                                    int nodeId = [[TBXML valueOfAttributeNamed:@"serialid" forElement:StateElement] intValue];
                                    TBXMLElement *topoNodeElement = [TBXML childElementNamed:@"topoNode" parentElement:StateElement];
                                    
                                    if (topoNodeElement) {
                                        TBXMLElement *gmlNodeElement = [TBXML childElementNamed:@"gml:Node" parentElement:topoNodeElement];
                                        if (gmlNodeElement) {
                                           
                                            
                                            TBXMLElement *floorElement = [TBXML childElementNamed:@"floor" parentElement:gmlNodeElement];
                                            int floor = [[TBXML textForElement:floorElement] intValue];
                                            
                                            TBXMLElement *nameElement = [TBXML childElementNamed:@"node_name" parentElement:gmlNodeElement];
                                            NSString *name = [TBXML textForElement:nameElement];
                                            
                                            TBXMLElement *typeElement = [TBXML childElementNamed:@"type" parentElement:gmlNodeElement];
                                            int type = [[TBXML textForElement:typeElement] intValue];
                                            
                                            TBXMLElement *xCoordElement = [TBXML childElementNamed:@"xcoord" parentElement:gmlNodeElement];
                                            double x = [[TBXML textForElement:xCoordElement] doubleValue];
                                            
                                            TBXMLElement *yCoordElement = [TBXML childElementNamed:@"ycoord" parentElement:gmlNodeElement];
                                            double y = [[TBXML textForElement:yCoordElement] doubleValue];
                                            
                                            TBXMLElement *zCoordElement = [TBXML childElementNamed:@"zcoord" parentElement:gmlNodeElement];
                                            int z = [[TBXML textForElement:zCoordElement] intValue];
                                            
                                            TBXMLElement *linkCountElement = [TBXML childElementNamed:@"link_cnt" parentElement:gmlNodeElement];
                                            NSString *linkCount = [TBXML textForElement:linkCountElement];
                                            
                                            TBXMLElement *adjLinkElement = [TBXML childElementNamed:@"adjacency_link" parentElement:gmlNodeElement];
                                            NSString *adjLink = [TBXML textForElement:adjLinkElement];
                                            
                                            //NSLog(@"Id:%d, floor:%@, name:%@, type:%d, x:%f, y:%f ,z:%d, count:%@, adj_link:%@", nodeId, f, n, t, x, y, z, l, a);
                                            //if (nodeId == 206) {
                                            //    int a= 0;
                                           // }
                                            if ([nodes objectForKey:[NSNumber numberWithInt:nodeId]] == nil)
                                            {
                                                VBNode *newNode = [[VBNode alloc] initWithNodeId:nodeId name:name x:x y:y z:floor level:level zone:0 type:type];
                                                [nodes setObject:newNode forKey:newNode.nodeId];
                                            }

                                        }
                                        
                                    }
                                }
                            } while ((stateElement = stateElement->nextSibling));
                        }
                    } while ((XFloorElement = XFloorElement->nextSibling));
                }
            }
        }
    }
    
    if (rootElement) {
        TBXMLElement *spaceMemberElement = [TBXML childElementNamed:@"SpaceLayerMember" parentElement:rootElement];
        if (spaceMemberElement) {
            TBXMLElement *spaceLayerElement = [TBXML childElementNamed:@"SpaceLayer" parentElement:spaceMemberElement];
            
            if (spaceLayerElement) {
                TBXMLElement *XFloorElement = [TBXML childElementNamed:@"X_floor" parentElement:spaceLayerElement];
                if (XFloorElement) {
                    do {
                        TBXMLElement *transitionElement = [TBXML childElementNamed:@"transition" parentElement:XFloorElement];
                        if (transitionElement) {
                            do {
                                TBXMLElement *TransitionElement = [TBXML childElementNamed:@"Transition" parentElement:transitionElement];
                                if (TransitionElement) {
                                    int edgeId = [[TBXML valueOfAttributeNamed:@"serialid" forElement:TransitionElement] intValue];
                                    
                                    TBXMLElement *topoEdgeElement = [TBXML childElementNamed:@"topoEdge" parentElement:TransitionElement];
                                    if (topoEdgeElement) {
                                        TBXMLElement *gmlEdgeElement = [TBXML childElementNamed:@"gml:Edge" parentElement:topoEdgeElement];
                                        if (gmlEdgeElement) {
                                            
                                            TBXMLElement *linkNameElement = [TBXML childElementNamed:@"link_name" parentElement:gmlEdgeElement];
                                            NSString *linkName = [TBXML textForElement:linkNameElement];
                                            
                                            TBXMLElement *sNodeElement = [TBXML childElementNamed:@"snode" parentElement:gmlEdgeElement];
                                            int node1Id = [[TBXML textForElement:sNodeElement] intValue];
                                            
                                            TBXMLElement *eNodeElement = [TBXML childElementNamed:@"enode" parentElement:gmlEdgeElement];
                                            int node2Id = [[TBXML textForElement:eNodeElement] intValue];
                                            
                                            TBXMLElement *lengthElement = [TBXML childElementNamed:@"length" parentElement:gmlEdgeElement];
                                            double weight = [[TBXML textForElement:lengthElement] doubleValue];
                                            
                                            TBXMLElement *typeElement = [TBXML childElementNamed:@"type" parentElement:gmlEdgeElement];
                                            NSString *type = [TBXML textForElement:typeElement];
                                            
                                            TBXMLElement *moveTypeElement = [TBXML childElementNamed:@"move_type" parentElement:gmlEdgeElement];
                                            BOOL isTwoWay = ![[TBXML textForElement:moveTypeElement] boolValue];
                                            
                                            TBXMLElement *passableElement = [TBXML childElementNamed:@"passable" parentElement:gmlEdgeElement];
                                            NSString *passable = [TBXML textForElement:passableElement];
                                            
                                            TBXMLElement *restrictElement = [TBXML childElementNamed:@"restrict" parentElement:gmlEdgeElement];
                                            int rest = [[TBXML textForElement:restrictElement] intValue];
                                            
                                            //NSLog(@"name:%@, snode:%@, enode:%@, type:%@, moveType:%@,passible:%@,restrict:%@",l,s,e,t,m,p,r);
                                            VBNode *firstNode = [nodes objectForKey:[NSNumber numberWithInt:node1Id]];
                                            VBNode *secondNode = [nodes objectForKey:[NSNumber numberWithInt:node2Id]];
                                            
                                            if (firstNode == nil || secondNode == nil)
                                            {
                                                //NSException *exception = [NSException exceptionWithName:@"NSInvalidArgumentException" reason:@"Invalid nodeid given" userInfo:nil];
                                                //@throw exception;
                                                continue;
                                            }
                                            
                                            if ([edges objectForKey:[NSNumber numberWithInt:edgeId]] == nil)
                                            {
                                                VBEdge *newEdge = [[VBEdge alloc] initWithEdgeId:edgeId weight:weight firstNode:firstNode   secondNode:secondNode isOneWay:!isTwoWay rest:rest];
                                            //    if (firstNode) {
                                                    [firstNode.edges addObject:newEdge];
                                             //   }
                                                //if (isTwoWay)
                                                //{
                                               // if (secondNode) {
                                                    [secondNode.edges addObject:newEdge];
                                                //}
                                                //}
                                                [edges setObject:newEdge forKey:newEdge.edgeId];
                                            }
                                            
                                        }
                                    }
                                }
                            } while ((transitionElement = transitionElement->nextSibling));
                        }
                    } while ((XFloorElement = XFloorElement->nextSibling));
                }
            }
        }
    }

    
    return resBit;
}

@end
