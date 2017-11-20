//
//  PathEdge.h
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 12. 7..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface VBPathEdge : NSObject

@property ( nonatomic) int edgeId;
@property ( nonatomic) int startNode;
@property ( nonatomic) int endNode;
@property ( nonatomic) bool isBidirection;
@property ( nonatomic) float length;

@end
