//
//  MathLinear.m
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 12. 7..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import "MathLinear.h"
#include <math.h>

@implementation MathLinear

+(float)Distance:(Point3D*)point1 Point2:(Point3D*)point2
{
    Point3D Vector;
    
    Vector.X = point2->X - point1->X;
    Vector.Y = point2->Y - point1->Y;
    Vector.Z = point2->Z - point1->Z;
    
    return (float)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z );
}

+(Point3D)IntersectionPointOnLine:(Point3D*)point LineStart:(Point3D*)lineStart LineEnd:(Point3D*)lineEnd
{
    float LineMag=[MathLinear Distance:lineEnd Point2:lineStart];
    
    float U = ( ( ( point->X -lineStart->X ) * ( lineEnd->X - lineStart->X ) ) +
               ( ( point->Y - lineStart->Y ) * ( lineEnd->Y - lineStart->Y ) ) +
               ( ( point->Z - lineStart->Z ) * ( lineEnd->Z - lineStart->Z ) ) ) /
    ( LineMag * LineMag );
    
    Point3D result;
    result.X = lineStart->X + U * ( lineEnd->X - lineStart->X );
    result.Y = lineStart->Y + U * ( lineEnd->Y - lineStart->Y );
    result.Z = lineStart->Z + U * ( lineEnd->Z - lineStart->Z );
    
    return result;
}

+(Point3D)ClosestPointOnLine:(Point3D*)point
                   LineStart:(Point3D*)lineStart
                     LineEnd:(Point3D*)lineEnd;
{
    Point3D closestPoint;
    
    float LineMag=[MathLinear Distance:lineEnd Point2:lineStart];
    
    float U = ( ( ( point->X -lineStart->X ) * ( lineEnd->X - lineStart->X ) ) +
               ( ( point->Y - lineStart->Y ) * ( lineEnd->Y - lineStart->Y ) ) +
               ( ( point->Z - lineStart->Z ) * ( lineEnd->Z - lineStart->Z ) ) ) /
    ( LineMag * LineMag );
    
    if( U < 0.0f)
    {
        closestPoint=*lineStart;
    }
    else if(U > 1.0f)
    {
        closestPoint=*lineEnd;
    }
    else
    {
        closestPoint.X = lineStart->X + U * ( lineEnd->X - lineStart->X );
        closestPoint.Y = lineStart->Y + U * ( lineEnd->Y - lineStart->Y );
        closestPoint.Z = lineStart->Z + U * ( lineEnd->Z - lineStart->Z );
    }
    
    return closestPoint;
}
@end
