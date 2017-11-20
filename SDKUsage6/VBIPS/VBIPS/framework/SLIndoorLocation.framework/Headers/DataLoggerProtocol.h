//
//  DataLoggerProtocol.h
//  SLIndoorLocation
//
//  Created by David Törnqvist on 11/29/13.
//
//

#import <Foundation/Foundation.h>
#import "Step.h"
#import "DeviceInfo.h"
#import "LogFileInfo.h"
#import "NetworkMap.h"
#import "SLIndoorLocationEnums.h"
#import "MonitoredRegionMeasurement.h"
#import "BeaconRssiMeasurement.h"
#import "MagnetometerData.h"
#import "HeadingData.h"
#import "AccelerometerData.h"
#import "GyroData.h"
#import "BTinfo.h"
#import "SLCoordinate3D.h"

@protocol DataLoggerProtocol <NSObject>

- (void) openForWritingSimulationData:(NSString*)fileName;
- (void) openWithMap:(NetworkMap*) map andMapVersion:(int) mapVersion andUserId:(NSString*) userId andCustomerId:(NSString*)customerId;
- (void) close;
- (void) disableSensorAndPositionLogging;
- (void) printFileByNSLog;
- (void) writeAccelerometerData : (AccelerometerData*) accData;
- (void) writeGyroData : (GyroData*) gyroData;
- (void) writeMagnetometerData : (MagnetometerData*) magData;
- (void) writeHeading : (HeadingData*) heading;
//- (void) writeCLLocation : (CLLocation*) location;
- (void) writeStep : (Step*) step;
- (NSTimeInterval) writeSLCoordinate3D : (SLCoordinate3D*)pos withUncertainty : (double)posStdDev;
- (void) writeRef:(SLCoordinate3D*)pos;
- (void) writeInitPos:(SLCoordinate3D*)pos;
- (void) writeStartPos:(SLCoordinate3D*)pos;
- (void) writeEndPos:(SLCoordinate3D*)pos;
- (void) writeDtap:(SLCoordinate3D*)pos;
- (void) writeNSString : (NSString*) text;
- (void) writeNavigationSurvey:(NSString*) survey;
- (void) writeNavigationComment:(NSString*) comment;
- (void) writeBT:(NSMutableDictionary*)btScan;
- (void) writeHeading:(double)heading andHeadingStatus:(BOOL)status;
- (void) writeLocationStatus:(SLLocationStatusType)status andFloorNrProbability:(double)floorNrPropability andDisturbanceIndicator:(double)disturbanceIndicator forTime:(NSTimeInterval)time;
- (void) writeBeaconRegionEntry:(MonitoredRegionMeasurement*) mRegion;
- (void) writeBeaconRssiDict:(NSMutableDictionary*)iBeaconScan;
- (void) writeBeaconRssi:(BeaconRssiMeasurement*) beaconRssi;
- (void) writeBTinfo:(BTinfo*) btInfo;
+ (NSString*) getLogDir;
+ (NSString *)getUUID;
- (void) writeLogFileInfoToFile;
+ (void) listLogFiles;
+ (NSMutableDictionary*) getDatabaseEntries;
+ (void)removeAllDatabaseEntries;
+ (void)removeAllUploadedDatabaseEntries;
+ (NSMutableArray*) getLogFileInfoList;
+ (NSInteger) getNumberOfLogFiles;

@property (nonatomic,readonly) bool isEnabled;
@property (nonatomic) bool sensorEnabled;
@property (nonatomic) bool logIBDB;
@property (nonatomic) bool logIBTI;

@end
