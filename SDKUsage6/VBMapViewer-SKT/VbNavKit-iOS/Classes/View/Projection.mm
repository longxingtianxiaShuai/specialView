//
//  Projection.m
//  VbNavKit-iOS
//
//  Created by Hunju Hwang on 12. 11. 14..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import "Projection.h"
#import "Matrix.hpp"

@implementation Projection
{
    mat4 windowMatrix;
}

-(id)initMatrix:(void*)matrixPtr withCameraHeight:(float)cameraHeight
{
    self = [super init];

    if (self) {
        mat4* matPtr=(mat4*)matrixPtr;
        
        memcpy(&windowMatrix, matPtr, sizeof(mat4));
        
        self.cameraHeight=cameraHeight;
    }
    return self;
}

-(CGPoint)WorldToWindowWithScreenSize:(CGSize)screenSize withX:(float)x withY:(float)y withZ:(float)z
{
    vec4 world_pt;
    {
        world_pt.x=x;
        world_pt.y=y;
        world_pt.z=z;
        world_pt.w=1.f;
    }
    
    ivec2 win_pos;
    
    vec4  projected_3d = windowMatrix * world_pt;
    // todo 自己修改  如果如此设置放大后poi不显示
   // if(abs(projected_3d.z/projected_3d.w) >= 0.5f)//카메라 뒤쪽은 계산하지 않음.
   //     return CGPointMake(-100,-100);
    
    float newX = (screenSize.width * (projected_3d.x/projected_3d.w + 1.f)/2.f);
    float newY = (screenSize.height * (projected_3d.y/projected_3d.w + 1.f)/2.f);
//    float newZ = (projected_3d.z/projected_3d.w + 1.f)/2.f;
    
    
    return CGPointMake((newX),screenSize.height - (newY));
}
@end
