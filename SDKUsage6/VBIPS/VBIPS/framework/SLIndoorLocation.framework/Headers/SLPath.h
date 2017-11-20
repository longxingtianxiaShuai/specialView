//
//  SLPath.h
//  SLIndoorLocation
//
//  Created by skoglar on 02/07/14.
//  Copyright (c) 2014 SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SLPath : NSObject

-(id) initWithLocations:(NSArray*)pathNodes andPathLength:(double)pathLength andSegmentLengts:(NSArray*)segmentLengths;

@property (nonatomic, strong, readonly) NSArray *pathNodes;
@property (nonatomic, readonly) double pathLength;
@property (nonatomic, strong, readonly) NSArray *segmentLengths;

@end
