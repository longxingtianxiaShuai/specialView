//
//  MapPosEst.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 07/03/14.
//
//

#import "MapPos.h"

@interface MapPosEst : MapPos

- (id) initWithMapPos:(MapPos*)mapPos_ andPosStdDev:(double)posStdDev_ andFloorNrPropability:(double)floorNrPropability_;
- (id) initWithX:(double)x_ andY:(double)y_ andFloorNr:(NSInteger)floorNr_ andHeading:(double)heading_ andPosStdDev:(double)posStdDev_ andFloorNrPropability:(double)floorNrPropability_;

@property double posStdDev;
@property double floorNrPropability;

@end
