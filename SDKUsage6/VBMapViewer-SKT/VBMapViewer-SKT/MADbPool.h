//
//  MADbPool.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 22..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MAZoneInfo;
@class MABuildingInfo;
@class MALoadMapInfo;
@class MAFloorInfo;
@class PoiFilter;
@class MAPoi;
@class MALodInfo;

@interface MADbPool : NSObject

+(MADbPool*)Instance;

-(NSArray*)PoiListWithQuery:(NSString*)query;
-(NSArray*)PoiListWithFilter:(PoiFilter*)filter;

-(MAZoneInfo*)getZoneInfo:(NSString*)projectId;
-(MABuildingInfo*)getBuilding:(NSString*)prjId;
-(NSArray *)getBuildingInfo:(NSString *)zoneId;
-(MALoadMapInfo*)getLoadMapInfo:(NSString*)prjId;

-(NSArray *)getFloorInfo:(NSString*)bldgId;
-(MAFloorInfo *)getFloor:(int)floor;
-(MAFloorInfo *)getFloorById:(NSString *)floorId;
                              
-(MAPoi *)getPOIInfoFromId:(int)poiId;

- (NSString *)getDefaultZoneId;
- (NSString *)getDefaultBuildingId;
- (NSString *)getDefaultFloorId;
- (MALodInfo *)getLodInfo:(int)floorId;
- (float) getNorthAxis:(NSString *)prjId;
- (NSArray *) getCategoryList:(NSString *)prjId;
- (NSArray *) getSbmFileList:(NSString *)prjId;
- (NSString *) getSbmGroupName:(NSString *)floorId;
- (NSArray *) getSameFloor:(MAFloorInfo *)floorInfo;
- (float) getMaxDistance;
@end
