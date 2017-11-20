//
//  MathLinear.h
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 12. 7..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct tagPoint3D
{
    float X, Y, Z;
}
Point3D;

@interface MathLinear : NSObject 

+(float)Distance:(Point3D*)point1 Point2:(Point3D*)point2;

+(Point3D)IntersectionPointOnLine:(Point3D*)point LineStart:(Point3D*)lineStart LineEnd:(Point3D*)lineEnd;

//하나의 선과 하나의 점이 있을 때 점에서 선까지 가장 가까운 점을 구한다.
//보통은 선
+(Point3D)ClosestPointOnLine:(Point3D*)point
                   LineStart:(Point3D*)lineStart
                     LineEnd:(Point3D*)lineEnd;

@end
