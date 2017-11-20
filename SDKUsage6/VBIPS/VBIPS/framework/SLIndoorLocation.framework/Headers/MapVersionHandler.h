//
//  MapVersionHandler.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 2/27/13.
//
//

#import <Foundation/Foundation.h>

@protocol MapVersionHandlerDelegate <NSObject>
- (void) didFailGetMapVersionWithError:(NSError *)error;
@end

@interface MapVersionHandler : NSObject <NSURLConnectionDelegate> {
    NSMutableData *receivedData;
    void (^completionBlock)(int);
}

- (MapVersionHandler*) initWithMapId:(NSString*) mapId_ andCustomerId:(NSString*) customerId_ andUserId:(NSString*) userId_;
- (void) getVersionNrAvailableWithTimeout:(double) timeout andCompletion:(void (^)(int))completionBlock_;
- (int) getLocalMaxVersionNr;
- (NSString*) getLocalMaxVersionFileName;
- (int) getVersionFromFileName:(NSString*) fileName;
- (void) printFileByNSLog:(NSString*) fileName;

@property (nonatomic, weak) id <MapVersionHandlerDelegate> delegate;
@property (nonatomic, strong) NSString *mapId;
@property (nonatomic, strong) NSString *customerId;
@property (nonatomic, strong) NSString *userId;

@end
