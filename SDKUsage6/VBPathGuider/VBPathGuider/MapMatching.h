//
//  MapMatching.h
//  VBPathGuider
//
//  Created by Justin on 2014. 8. 20..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MAVector.h"

@interface MapMatching : NSObject

- (void) setNodeList:(NSArray *)list;
- (void) setEdgeList:(NSArray *)list;
- (MAVector3D *) mapMatching:(MAVector3D *)pos;

@end
