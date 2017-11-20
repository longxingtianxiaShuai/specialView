//
//  FileParser.h
//  DijkstraApp
//
//  Created by Junwoo Park on 11/19/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FileParser : NSObject

+ (void) loadNodesFromTextFilePath: (NSString *)filePath toNodeDict: (NSMutableDictionary *)nodes;
+ (void) loadEdgesFromTextFilePath: (NSString *)filePath andNodeDict: (NSDictionary *)nodes toEdgeDict: (NSMutableDictionary *)edges;
+ (NSNumber *) loadNodesAndEdgesFromGmlFilePath: (NSString *)filePath toNodeDict: (NSDictionary *)nodes toEdgeDict: (NSMutableDictionary *)edges;


@end
