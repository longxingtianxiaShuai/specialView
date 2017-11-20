//
//  VBSimulation.h
//  VBIPS
//
//  Created by Justin on 2014. 8. 8..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MAVector;
@class MACoordinate;

@interface VBSimulation : NSObject

- (void) setData:(NSString *)filePath;
- (void) startSimulation;
- (MAVector *) getCurrentPos;
- (MACoordinate *) getCurrentInfo;
@end
