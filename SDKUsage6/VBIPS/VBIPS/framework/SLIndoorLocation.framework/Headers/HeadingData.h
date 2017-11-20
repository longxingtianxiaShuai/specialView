//
//  CLHeadingSim.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 2/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef double LocationDirection;
typedef double HeadingComponentValue;

@interface HeadingData : NSObject {

}

- (id) initWithTimeStamp:(NSDate*)timestamp_ 
		 magneticHeading:(LocationDirection) magneticHeading_
			 trueHeading:(LocationDirection)trueHeading_
		 headingAccuracy:(LocationDirection) headingAccuracy_
					   x:(HeadingComponentValue) x_
					   y:(HeadingComponentValue) y_
					   z:(HeadingComponentValue) z_;
- (id) initWithMessage:(NSString*)message;


@property(readonly, nonatomic) LocationDirection magneticHeading;
@property(readonly, nonatomic) LocationDirection trueHeading;
@property(readonly, nonatomic) LocationDirection headingAccuracy;
@property(readonly, nonatomic) HeadingComponentValue x;
@property(readonly, nonatomic) HeadingComponentValue y;
@property(readonly, nonatomic) HeadingComponentValue z;
@property(readonly, nonatomic) NSDate *timestamp;


@end
