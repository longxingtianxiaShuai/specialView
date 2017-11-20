//
//  PriorityQueueARC.h
//  DijkstraApp
//
//  Created by Junwoo Park on 11/17/12.
//  Copyright (c) 2012 Junwoo Park. All rights reserved.
//

// PiroirityQueue under ARC
// Uses sorted array of object/weight pairs
// I had to implement this from scratch because CFBinaryHeap does not allow me to change the weight of element in the queue.
// I am aware that this will be more expensive than heap implementation but... I don't think implementing heap altogether will not gain too much performance benefit over effort.

#import <Foundation/Foundation.h>

@interface ObjectWeightWrapper : NSObject

@property (nonatomic,strong,readonly) id object;
@property (nonatomic,readonly) double weight;

- (id) initWithObject:(id) object andWeight:(double) weight;
- (NSComparisonResult) weightCompare:(ObjectWeightWrapper *) otherObject;

@end

@interface PriorityQueueARC : NSObject

- (void) addObject: (id) object withWeight: (double) weight;
- (ObjectWeightWrapper *) poll;
- (void) removeObject: (id) object;
- (BOOL) isEmpty;

@end

