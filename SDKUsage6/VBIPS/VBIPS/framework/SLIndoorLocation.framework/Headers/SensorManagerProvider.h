//
//  SensorManagerProvider.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/28/13.
//
//

#import <Foundation/Foundation.h>
#import "SensorManagerProtocol.h"

@interface SensorManagerProvider : NSObject

+ (id<SensorManagerProtocol>) getSensorManager;

@end
