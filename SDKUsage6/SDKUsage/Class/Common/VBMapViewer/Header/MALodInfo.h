//
//  MALod.h
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 6. 3..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MALodIconInfo;

@interface MALodInfo : NSObject

@property (nonatomic) int floorId;
@property (nonatomic) int levelCount;
@property (nonatomic) float cameraMaxDistance;
@property (nonatomic, strong) NSMutableArray *iconInfo;

@end
