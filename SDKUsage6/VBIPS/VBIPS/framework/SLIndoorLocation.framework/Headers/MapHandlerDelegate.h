//
//  MapHandlerDelegate.h
//  SLIndoorLocation
//
//  Copyright (c) 2010-2014, SenionLab AB. All rights reserved.
//
//

#import <Foundation/Foundation.h>

@protocol MapHandlerDelegate <NSObject>
- (void) didFailInternetConnectionWithError:(NSError *)error;
- (void) didFailInvalidIds:(NSError *)error;
@end
