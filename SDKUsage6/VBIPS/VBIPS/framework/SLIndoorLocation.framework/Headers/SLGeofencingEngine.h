//
//  SLGeofencingEngine.h
//  SLIndoorLocation
//
//

#import <Foundation/Foundation.h>
#import "SLGeofencing.h"
#import "SLCoordinate3D.h"

@interface SLGeofencingEngine : NSObject <SLGeofencing>

- (void) newPosition:(SLCoordinate3D*)location;

@end
