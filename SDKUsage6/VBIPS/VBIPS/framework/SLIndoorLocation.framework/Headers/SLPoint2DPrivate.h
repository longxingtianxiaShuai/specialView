//
//  SLPoint2DPrivate.h
//  SLIndoorLocation
//
//

#import <Foundation/Foundation.h>

@interface SLPoint2D : NSObject

- (id) initWithX:(double)x_ andY:(double)y_;
- (id) initWithSLPoint2D:(SLPoint2D*)point;

- (double) length;
- (void) normalize;
- (void) multiply:(double)factor;
- (void) add:(SLPoint2D*) vec;
- (void) add:(double)length inDir:(SLPoint2D*)dir;
- (void) projectOn:(SLPoint2D*)vec;
- (void) projectOrthogonalTo:(SLPoint2D*)vec;
- (void) setEquals:(SLPoint2D*)vec;

@property (nonatomic) double x;
@property (nonatomic) double y;

@end
