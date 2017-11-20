//
//  MapPos.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 06/03/14.
//
//

#import "SLPoint3DPrivate.h"

@interface MapPos : SLPoint3D

- (id) initWithX:(double)x_ andY:(double)y_ andFloorNr:(NSInteger)floorNr_ andHeading:(double)heading_;
- (id) initWithPoint:(SLPoint3D*)point andHeading:(double)heading_;
- (id) initWithMapPos:(MapPos*)mapPos;

@property double heading;

@end
