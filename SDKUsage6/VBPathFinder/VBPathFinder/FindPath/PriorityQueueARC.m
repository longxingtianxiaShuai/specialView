//
//  PriorityQueueARC.m
//  DijkstraApp
//
//  Created by Junwoo Park on 11/17/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

#import "PriorityQueueARC.h"


#pragma mark -
#pragma mark Object And Weight Pair Wrapping object.
@interface ObjectWeightWrapper()

@property (nonatomic,strong) id object;
@property (nonatomic) double weight;

@end


@implementation ObjectWeightWrapper

- (id) initWithObject:(id) object andWeight:(double) weight
{
    if (self = [super init])
    {
        self.object = object;
        self.weight = weight;
    }
    return self;
}

- (NSComparisonResult) weightCompare:(ObjectWeightWrapper *) otherObject
{
    /* sort in reverse order because removing last is less costly*/
    if (self.weight < otherObject.weight)
        return NSOrderedDescending;
    if (self.weight == otherObject.weight)
        return NSOrderedSame;
    return NSOrderedAscending;
        
}

- (id) init
{
    return [self initWithObject:nil andWeight:0];
}


@end

#pragma mark -
#pragma mark Private Fields etc
@interface PriorityQueueARC()

@property (nonatomic,strong) NSMutableArray *list;
@property (nonatomic,strong) NSMutableSet *entryList;

@end

@implementation PriorityQueueARC

- (id) init
{
    if (self = [super init])
    {
        self.list = [[NSMutableArray alloc] init];
        self.entryList = [[NSMutableSet alloc] init];
    }
    return self;
}

- (void) addObject: (id) object withWeight: (double) weight
{
    if ([self.entryList member:object]) {
        [self removeObject:object];
    }
    
    [self.list addObject:[[ObjectWeightWrapper alloc] initWithObject:object andWeight:weight]];
    [self.entryList addObject:object];
    
    [self.list sortUsingSelector:@selector(weightCompare:)];
}
- (ObjectWeightWrapper *) poll;
{
    if ([self.list count] == 0)
    {
        return nil;
    }
    ObjectWeightWrapper *entry = [self.list lastObject];
    
    [self.list removeLastObject];
    [self.entryList removeObject:entry.object];
    
    return entry;
}

- (void) removeObject: (id) toRemove
{
    NSIndexSet *removeList = [self.list indexesOfObjectsPassingTest:^BOOL(id obj, NSUInteger idx, BOOL *stop) {
        ObjectWeightWrapper *entry = (ObjectWeightWrapper *) obj;
        
        return [entry.object isEqual:toRemove];
    }];
    
    [self.list removeObjectsAtIndexes:removeList];
    [self.entryList removeObject:toRemove];

}

- (BOOL) isEmpty
{
    return [self.entryList count] == 0;
}
@end
