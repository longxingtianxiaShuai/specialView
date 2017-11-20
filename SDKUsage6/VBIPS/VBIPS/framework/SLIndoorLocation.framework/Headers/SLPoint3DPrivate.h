//
//  SLPoint3DPrivate.h
//  SLIndoorLocation
//
//

#import "SLPoint2DPrivate.h"

@interface SLPoint3D : SLPoint2D

- (id) initWithX:(double)x_ andY:(double)y_ andFloorNr:(NSInteger)floorNr_;
- (id) initWithSLPoint2D:(SLPoint2D*)point andFloorNr:(NSInteger)floorNr_;
- (id) initWithSLPoint3D:(SLPoint3D*)point;

@property (nonatomic) NSInteger floorNr; /**< Floor number*/

@end
