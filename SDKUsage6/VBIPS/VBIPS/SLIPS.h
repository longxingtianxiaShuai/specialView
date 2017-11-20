//
//  SLIPS.h
//  VBIPS
//
//  Created by Justin on 2014. 6. 30..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SLIndoorLocation/SLIndoorLocation.h>
#import "BaseIPS.h"

@interface SLIPS : BaseIPS <SLIndoorLocationManagerDelegate, SLGeofencingDelegate>

@property (nonatomic, strong) SLIndoorLocationManager *locationManager;

- (id) initWithMapId:(NSString *)mapId andCustomerId:(NSString *)customerId;

@end
