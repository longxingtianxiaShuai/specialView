//
//  MACoordinate.h
//  VBIPS
//
//  Created by Justin on 2014. 8. 28..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MACoordinate : NSObject

@property (nonatomic) float x;
@property (nonatomic) float y;
@property (nonatomic) int floor;
@property (nonatomic, assign) BOOL UpdatePosition;
@property (nonatomic, assign) float Headng;

@end
