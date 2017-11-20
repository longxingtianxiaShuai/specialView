//
//  Dijkstra.h
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import <Foundation/Foundation.h>
@class VBNode;

@interface Dijkstra : NSObject

+ (NSArray *) findBestPathFrom:(VBNode *)startNode To:(VBNode *)endNode option:(int)option;
+ (NSArray *) findBestEdgePathFrom:(VBNode *)startNode To:(VBNode *)endNode option:(int)option;

@end
