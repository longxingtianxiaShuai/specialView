//
//  MapHandler.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 1/7/13.
//
//

#import <Foundation/Foundation.h>
#import "MapVersionHandler.h"
#import "MapDownloader.h"
#import "MapHandlerDelegate.h"

@interface MapHandler : NSObject <MapDownloaderDelegate, MapVersionHandlerDelegate> {
    MapVersionHandler *versionHandler;
}

- (MapHandler*) initWithMapId:(NSString*) mapId andCustomerId:(NSString*)customerId andUserId:(NSString*)userId;
- (void) downloadUpdatedMapIfAvailableWithCompletion:(void (^)(NSString*))completionBlock_;

@property (nonatomic, weak) id <MapHandlerDelegate> delegate;
@property (nonatomic, retain) NSString *mapId;
@property (nonatomic, retain) NSString *customerId;
@property (nonatomic, retain) NSString *userId;

@end
