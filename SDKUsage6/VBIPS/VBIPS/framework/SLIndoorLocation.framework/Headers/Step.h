//
//  Step.h
//
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Step : NSObject {
}

- (Step*) initWithLength:(double) length_ andHeading:(double) heading_;
- (Step*) initWithTimestamp:(double)timestamp_ andLength:(double) length_ andHeading:(double) heading_;

/*! @property time Timestamp of the step.*/
@property (nonatomic) double timestamp;
/*! @property length Length of the step.*/
@property (nonatomic) double heading;
/*! @property heading Heading of the step.*/
@property (nonatomic) double length;

@end
