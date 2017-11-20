//
//  PoiFilter.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 12..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "PoiFilter.h"

@implementation PoiFilter

@synthesize Floor_id=_Floor_id;
@synthesize Categories=_Categories;

- (id)init
{
    self = [super init];
    
    if(self){
        [self Reset];
    }
    
    return self;
}

-(void)dealloc
{
    self.Categories=nil;
    self.delegate=nil;
}

-(void)Reset
{
    _Floor_id=1;
    
    if(self.delegate)
        [self.delegate PoiFilterChanged:self];
}

-(void)setCategories:(NSArray*)categories
{
    _Categories=categories;
    
    if(self.delegate)
        [self.delegate PoiFilterChanged:self];
}

-(void)setFloor_id:(int)floor_id
{
    _Floor_id=floor_id;
    
    if(self.delegate)
        [self.delegate PoiFilterChanged:self];
}


@end
