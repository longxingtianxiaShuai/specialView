//
//  PathNode.h
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 11. 28..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>
@class VBNode;

@interface PathNode : NSObject
@property (nonatomic) int nodeId;
@property (nonatomic) double x;
@property (nonatomic) double y;
@property (nonatomic) int floor;
@property (nonatomic) int level;
@property (nonatomic) int type;

-(id)initWithNodeInfo:(VBNode*)info;
-(id)initWithTextInfo:(NSString*)textInfo;
@end
