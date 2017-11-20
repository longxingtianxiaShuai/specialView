//
//  MAPoi.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 6..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface MAPoi : NSObject

@property (nonatomic) int poi_id;
@property (nonatomic) int prj_id;
@property (nonatomic) int bld_id;
@property (nonatomic,strong) NSString* floor_id;
@property (nonatomic,strong) NSString* code;
@property (nonatomic,strong) NSString* name;
@property (nonatomic,strong) NSString* positioning_type_code;
@property (nonatomic,strong) NSString* positioned;
@property (nonatomic) int category_id;
@property (nonatomic) int detail_id;
@property (nonatomic,strong) NSString* space_id;
@property (nonatomic,strong) NSString* x;
@property (nonatomic,strong) NSString* y;
@property (nonatomic,strong) NSString* z;
@property (nonatomic,strong) NSString* tags;
@property (nonatomic,strong) NSString* desc;
@property (nonatomic,strong) NSString* icon_real_name;
@property (nonatomic,strong) NSString* icon_stored_name;
@property (nonatomic,strong) NSString* image_name;
@property (nonatomic) BOOL visible;
@property (nonatomic, strong) NSString *floor_name;
@property (nonatomic) int floor_idx;
@property (nonatomic) int base_floor;
@end
