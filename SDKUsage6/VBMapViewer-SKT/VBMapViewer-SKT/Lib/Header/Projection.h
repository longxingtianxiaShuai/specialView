//
//  Projection.h
//  VbNavKit-iOS
//
//  Created by Hunju Hwang on 12. 11. 14..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface Projection : NSObject
@property (nonatomic) float cameraHeight;

-(id)initMatrix:(void*)matrixPtr withCameraHeight:(float)cameraHeight;
-(CGPoint)WorldToWindowWithScreenSize:(CGSize)screenSize withX:(float)x withY:(float)y withZ:(float)z;
@end
