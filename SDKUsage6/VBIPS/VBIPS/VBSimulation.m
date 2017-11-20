//
//  VBSimulation.m
//  VBIPS
//
//  Created by Justin on 2014. 8. 8..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "VBSimulation.h"
#import "MAVector.h"
#import <math.h>
#import "MACoordinate.h"

@implementation VBSimulation {
    NSMutableArray *data;
    int posIndex;
    
    int via;
    int step;
}

- (id) init
{
    self = [super init];
    if (self) {
        data = [[NSMutableArray alloc] initWithCapacity:10];
        posIndex = 0;
        step = -1;
        posIndex = 0;
        via = 0;
    }
    return self;
}

- (void) setData:(NSString *)filePath
{
    NSString *contents = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
    NSArray *line = [contents componentsSeparatedByString:@"\r\n"];
    for (NSString *l in line) {
        NSArray *coord = [l componentsSeparatedByString:@","];
        if (coord.count > 8) {
            MACoordinate *c = [[MACoordinate alloc] init];
            
            if ([coord[0] isEqualToString:@"H"]) {
                c.UpdatePosition = NO;
                c.floor = [coord[3] intValue];
                c.Headng = [coord[4] floatValue];
            } else {
                c.UpdatePosition = YES;
                c.x = [coord[6] floatValue];
                c.y = [coord[7] floatValue];
                c.floor = [coord[3] intValue];
            }
         //   NSLog(@"line = %@", l);
            [data addObject:c];
        }
    }
}

- (void) startSimulation
{
    
}

- (MACoordinate *) getCurrentInfo
{
    return  data[posIndex++];
}

- (MAVector *) getCurrentPos
{
    if (posIndex < [data count] - 1) {
        MACoordinate *c1 = (MACoordinate *)data[posIndex+1];
        MACoordinate *c2 = (MACoordinate *)data[posIndex];
        MAVector *v = [[MAVector alloc] init];
        if (step == -1) {
            double len = sqrt(pow(c1.x-c2.x,2) + pow(c1.y-c2.y,2))/* + pow(c1.floor-c2.floor,2))*/;
            step = (int)len/50;
            
            v.x = c2.x;
            v.y = c2.y;
            v.z = c2.floor;
            return v;
        } else {
            if (via == step) {
                posIndex++;
                via = 0;
                step = -1;
                
                v.x = c1.x;
                v.y = c1.y;
                v.z = c1.floor;
                return v;
            } else {
                float factor = via++/(float)step;
                
                //MAVector *ret = [[MAVector alloc] init];
                v.x = c2.x * (1-factor) + c1.x * factor;
                v.y = c2.y * (1-factor) + c1.y * factor;
                v.z = c2.floor * (1-factor) + c1.floor * factor;
                return v;
            }
        }
    }
    return nil;
}

@end
