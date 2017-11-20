//
//  TopologyBuilder.m
//  DijkstraApp
//
//  Created by Junwoo Park on 11/16/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import "TopologyBuilder.h"
#import "VBNode.h"
#import "FileParser.h"

#define POSTFIX  @".csv"
#define CONNECTING_EDGE_FILE  @"connecting"
#define FILENUM_CACHE_FILE  @"filepointer"
#define PATHLIST_CACHE_FILE  @"pathlist"

@interface TopologyBuilder()


@property (nonatomic,strong) NSArray *nodeFileList;
@property (nonatomic,strong) NSArray *edgeFileList;
@property (nonatomic,strong) NSMutableDictionary *nodeCache;
@property (nonatomic,strong) NSMutableDictionary *edgeCache;

- (void) loadCache;

@end

@implementation Topology

- (id) initWithSrc: (VBNode *) src dest: (VBNode *) dest nodeCache: (NSDictionary *) nodeCache edgeCache:(NSDictionary *)edgeCache
{
    if (self = [super init])
    {
        self.src = src;
        self.dest = dest;
        self.nodeCache = nodeCache;
        self.edgeCache = edgeCache;
    }
    return self;
}

@end

@implementation TopologyBuilder

- (Topology *) buildFromSrcId:(int) srcId toDestId:(int) destId
{
    @try {
        
        VBNode *src;
        VBNode *dest;
        
        //[self loadCache];
        //[self loadCacheFromGml];
        
        src = [self.nodeCache objectForKey:[NSNumber numberWithInt:srcId]];
        dest = [self.nodeCache objectForKey:[NSNumber numberWithInt:destId]];
        
        if (src == nil || dest == nil)
        {
            return nil;
        }
        
        return [[Topology alloc] initWithSrc:src dest:dest nodeCache:self.nodeCache edgeCache:self.edgeCache];
    }
    @catch (NSException *exception) {
        return nil;
    }
    
}

- (id) initWithFile:(NSString *)filePath withType:(int)type nodeFileList:(NSArray *)nodeFileList edgeFileList:(NSArray *)edgeFileList
{
    if (self = [super init])
    {
        self.filePath = filePath;
        if (type == 0) {
            self.nodeFileList = nodeFileList;
            self.edgeFileList = edgeFileList;
            [self loadCache];
        } else if (type == 1) {
            self.filePath = filePath;
            [self loadCacheFromGml];
        }
    }
    return self;
}
/*
- (id) initWithGmlFilePath:(NSString *)filePath
{
    if (self = [super init])
    {
        self.filePath = filePath;
        [self loadCacheFromGml];
    }
    return self;
}

- (id) initWithTextFilePath: (NSString *)filePath nodeFileList:(NSArray *)nodeFileList edgeFileList:(NSArray *)edgeFileList
{
    if (self = [super init])
    {
        self.filePath = filePath;
        self.nodeFileList = nodeFileList;
        self.edgeFileList = edgeFileList;
    }
    return self;
}*/
- (void) flushCache
{
    self.nodeCache = nil;
    self.edgeCache = nil;
}
- (void) loadCache
{
    if (self.nodeCache == nil || self.edgeCache == nil)
    {
        self.nodeCache = [[NSMutableDictionary alloc] init];
        self.edgeCache = [[NSMutableDictionary alloc] init];
        @autoreleasepool {
            
            for (NSString *nodeFileName in self.nodeFileList)
            {
                NSString *fullFilePath = [self.filePath stringByAppendingPathComponent:nodeFileName];
                [FileParser loadNodesFromTextFilePath:fullFilePath toNodeDict:self.nodeCache];
            }
            for (NSString *edgeFileName in self.edgeFileList)
            {
                NSString *fullFilePath = [self.filePath stringByAppendingPathComponent:edgeFileName];
                [FileParser loadEdgesFromTextFilePath:fullFilePath andNodeDict:self.nodeCache toEdgeDict:self.edgeCache];
            }
        }
    }
}

- (void) loadCacheFromGml
{
    if (self.nodeCache == nil || self.edgeCache == nil)
    {
        self.nodeCache = [[NSMutableDictionary alloc] init];
        self.edgeCache = [[NSMutableDictionary alloc] init];
        @autoreleasepool {
             self.restrict_bit = [FileParser loadNodesAndEdgesFromGmlFilePath:self.filePath toNodeDict:self.nodeCache toEdgeDict:self.edgeCache];
        }
    }
}

- (NSDictionary *) getNodeCache
{
    return self.nodeCache;
}
- (NSDictionary *) getEdgeCache
{
    return self.edgeCache;
}
/*
- (void) dealloc
{
    [self flushCache];
}*/

@end
