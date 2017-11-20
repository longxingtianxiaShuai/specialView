//
//  MapDownloader.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 2/27/13.
//
//

#import <Foundation/Foundation.h>

@protocol MapDownloaderDelegate <NSObject>
- (void) didFailDownloadMapWithError:(NSError *)error;
@end


@interface MapDownloader : NSObject <NSURLConnectionDelegate> {
    NSMutableData *receivedData;
    NSString *fileName;
    void (^completionBlock)(NSString*);
}

- (MapDownloader*) initWithMapId:(NSString*) mapId andCustomerId:(NSString*)customerId andUserId:(NSString*)userId;
- (void) downloadLatestMapWithCompletion:(void (^)(NSString*))completionBlock_;

@property (nonatomic, weak) id <MapDownloaderDelegate> delegate;
@property (nonatomic, retain) NSString *mapId;
@property (nonatomic, retain) NSString *customerId;
@property (nonatomic, retain) NSString *userId;

@end
