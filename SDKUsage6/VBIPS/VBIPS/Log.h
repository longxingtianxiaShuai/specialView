//
//  Log.h
//  VBIPS
//
//  Created by Justin on 2014. 8. 18..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Log : NSObject

- (void) setLogFileName:(NSString *)fileName;
- (void) saveSLLogWithLonguitude:(double)longuitude andLatitude:(double)latitude andX:(float)x andY:(float)y radius:(double)radius;
- (void) saveSLLogWithHeading:(double)heading;


@end
