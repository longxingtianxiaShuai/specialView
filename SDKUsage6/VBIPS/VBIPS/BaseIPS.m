//
//  BaseIPS.m
//  VBIPS
//
//  Created by Justin on 2014. 6. 30..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "BaseIPS.h"

@implementation BaseIPS {
    float hMatrix[9];
    CGPoint centerInput, scaleInput;
    CGPoint centerOutput, scaleOutput;

}

- (void) startUpdatingLocation
{
    
}

- (void) stopUpdatingLocation
{
    
}

- (void) resetUpdatingLocation
{
    
}

- (void) setSLTransform:(float *)matrix centerInput:(CGPoint)centerInput_ scaleInput:(CGPoint)scaleInput_ centerOutput:(CGPoint)centerOutput_ scaleOutput:(CGPoint)scaleOutput_ azimuthOffset:(double)azimuthOffset_
{
    memcpy(&hMatrix[0], matrix, sizeof(float) * 9);
    
    centerInput = centerInput_;
    scaleInput = scaleInput_;
    centerOutput = centerOutput_;
    scaleOutput = scaleOutput_;
    azimuthOffset = azimuthOffset_;
}

- (CGPoint) convertToVBPositionFromX:(float)x andY:(float)y
{
    CGPoint normalizedPoint;
    normalizedPoint.x = (x - centerInput.x) / scaleInput.x;
    normalizedPoint.y = (y - centerInput.y) / scaleInput.y;
    
    float x_ = normalizedPoint.x * hMatrix[0] + normalizedPoint.y * hMatrix[1] + hMatrix[2];
    float y_ = normalizedPoint.x * hMatrix[3] + normalizedPoint.y * hMatrix[4] + hMatrix[5];
    float w_ = normalizedPoint.x * hMatrix[6] + normalizedPoint.y * hMatrix[7] + hMatrix[8];
    
    CGPoint pt;
    pt.x = (x_ / w_ * scaleOutput.x) + centerOutput.x;
    pt.y = (y_ / w_ * scaleOutput.y) + centerOutput.y;
    
    return pt;
}

- (void) setSLSimulationData:(NSString *)filePath
{
    //simulation = [[VBSimulation alloc] init];
    //[simulation setData:filePath];
}

- (void) startSLSimulation
{
    //[simulation startSimulation];
}

@end

