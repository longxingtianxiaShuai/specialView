//
//  PoiFilter.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 12..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

@class PoiFilter;

@protocol PoiFilterDelegate <NSObject>
-(void)PoiFilterChanged:(PoiFilter*)poiFilter;
@end

@interface PoiFilter : NSObject
@property (nonatomic) int Floor_id;
@property (nonatomic,strong) NSArray* Categories;
@property (nonatomic,assign) id<PoiFilterDelegate> delegate;

-(void)Reset;
@end
