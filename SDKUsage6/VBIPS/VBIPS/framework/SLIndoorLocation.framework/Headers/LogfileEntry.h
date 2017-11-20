//
//  LogfileEntry.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 2/8/13.
//
//

#import <Foundation/Foundation.h>

@interface LogfileEntry : NSObject {
	NSString *mapName;
	NSString *fileName;
	NSString *comment;
	NSInteger navResult;
    NSInteger starttime;
    bool uploaded;
}

- (id) initWithMapName:(NSString*)mapName_ andFileName:(NSString*)fileName_ andComment:(NSString*)comment_ andNavResult:(NSInteger)navResult_ andStarttime:(NSInteger)starttime_ andUploaded:(BOOL)uploaded_;

- (NSString *)getTableViewString;

@property (nonatomic, retain) NSString *mapName;
@property (nonatomic, retain) NSString *fileName;
@property (nonatomic, retain) NSString *comment;
@property (nonatomic) NSInteger navResult;
@property (nonatomic) NSInteger starttime;
@property (nonatomic) bool uploaded;

@end
