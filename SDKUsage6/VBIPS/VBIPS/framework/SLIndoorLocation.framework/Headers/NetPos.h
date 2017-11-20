//
//  NetPos.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 07/03/14.
//
//

#import <Foundation/Foundation.h>

@interface NetPos : NSObject

- (id) initWithLinkID:(NSUInteger)linkID_ andDistance:(double)distance_;
- (id) initWithNetPos:(NetPos*)netPos;

@property NSUInteger linkID;
@property double distance;

@end
