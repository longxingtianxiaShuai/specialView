//
//  MAStateManager.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 22..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MAStateManager : NSObject

+(MAStateManager*)Instance;

@property (nonatomic) int current_project_id;
@property (nonatomic) int current_zone_id;
@property (nonatomic) int current_building_id;
@property (nonatomic) int current_floor_id;
@property (nonatomic) float north_axis;
@property (nonatomic) BOOL isZone;
@property (nonatomic, strong) NSString *downloadPath;
@property (nonatomic, strong) NSString* default_zoneId;
@property (nonatomic, strong) NSString* default_bldgId;
@property (nonatomic, strong) NSString* default_floorId;
@property (nonatomic, strong) NSString* building_name_e;

@end
