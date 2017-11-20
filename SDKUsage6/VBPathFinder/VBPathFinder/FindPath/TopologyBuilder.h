//
//  TopologyBuilder.h
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import <Foundation/Foundation.h>

@class VBNode;

@interface Topology : NSObject

@property (nonatomic,strong) VBNode *src;
@property (nonatomic,strong) VBNode *dest;
@property (nonatomic,strong) NSDictionary *nodeCache;
@property (nonatomic,strong) NSDictionary *edgeCache;

- (id) initWithSrc: (VBNode *) src dest: (VBNode *) dest nodeCache:(NSDictionary *)nodeCache edgeCache:(NSDictionary *)edgeCache;

@end

@interface TopologyBuilder : NSObject

@property (nonatomic,strong) NSString *filePath;
@property (nonatomic, strong) NSNumber *restrict_bit;

- (Topology *) buildFromSrcId:(int) srcId toDestId:(int) destId;
- (id) initWithFile:(NSString *)filePath withType:(int)type nodeFileList:(NSArray *)nodeFileList edgeFileList:(NSArray *)edgeFileList;
- (void) flushCache;
- (NSDictionary *) getNodeCache;
- (NSDictionary *) getEdgeCache;
@end
