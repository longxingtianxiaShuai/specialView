//
//  PathNode.h
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 11. 28..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface VBPathNode : NSObject
@property (readonly, nonatomic) int nodeId;
@property (readonly, nonatomic) double x;
@property (readonly, nonatomic) double y;
@property (readonly, nonatomic) int floor;
@property (readonly, nonatomic) int type;

@end
