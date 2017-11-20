//
//  Node.h
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Utility.h"
#include "SLPoint3DPrivate.h"

@interface Node : NSObject {

}
- (bool) isDeadEnd;
- (bool) containsLink:(int)linkID;

@property (nonatomic, strong) NSMutableArray *links;
@property (nonatomic, strong) SLPoint3D* pos;
@property (nonatomic) int type;

@end
