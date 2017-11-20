//
//  MAFloorInfo.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 18..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MAFloorInfo : NSObject

@property (nonatomic, strong) NSString* building_id;
@property (nonatomic, strong) NSString* floor_id;
@property (nonatomic, strong) NSString* base_floor;
@property (nonatomic, strong) NSString* name_kr;
@property (nonatomic, strong) NSString* name_en;
@property (nonatomic, strong) NSString* sbm_file;
@property (nonatomic, assign) int groupId;
@end
