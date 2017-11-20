//
//  MADbPool.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 22..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MADbPool.h"

#import "MAPoi.h"
#import "MAZoneInfo.h"
#import "MABuildingInfo.h"
#import "MALoadMapInfo.h"
#import "MAFloorInfo.h"

#import "MAStateManager.h"
#import "PoiFilter.h"

#import "CommonLib.h"
#import "DebugOutput.h"
#import "MALodIconInfo.h"
#import "MALodInfo.h"

#import <sqlite3.h>
#import "MACategory.h"

@implementation MADbPool

+(MADbPool*)Instance
{
    static dispatch_once_t dispatchOnce;
    static MADbPool* instance = nil;
    
    dispatch_once(&dispatchOnce, ^{
        instance=[[MADbPool alloc] init];
    });
    
    return instance;
}

-(NSArray*)PoiListWithQuery:(NSString*)query
{
    MAStateManager *stateManager = [MAStateManager Instance];
    
    NSMutableArray *result = [NSMutableArray array];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    NSString *projectID = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];

    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", projectID, projectID]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                MAPoi* poi= [[MAPoi alloc] init];
                {
                    poi.poi_id=sqlite3_column_int(stmt,0);
                    poi.prj_id=sqlite3_column_int(stmt,1);;
                    poi.bld_id=sqlite3_column_int(stmt,2);;
                    poi.floor_id=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)];;
                    poi.code=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                    poi.name=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)];
                    poi.positioning_type_code=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,6)];
                    poi.positioned=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,7)];
                    poi.category_id=sqlite3_column_int(stmt,8);
                    poi.detail_id=sqlite3_column_int(stmt,9);
                    poi.space_id=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,10)];
                    poi.x=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,11)];
                    poi.y=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,12)];
                    poi.z=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,13)];
                    
                    if ([poi.z hasPrefix:@"-"]) {
                        poi.z = [poi.z substringFromIndex:1];
                    } else {
                        poi.z = [NSString stringWithFormat:@"-%@",poi.z];
                    }
                    
                    poi.tags=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,14)];
                    poi.desc=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,15)];
                    // Justin
                    //poi.icon_real_name=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,16)];
                    poi.icon_stored_name=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,16)];
                    poi.floor_name = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,17)];
                    poi.floor_idx = sqlite3_column_int(stmt, 18);
                    poi.base_floor = sqlite3_column_int(stmt, 19);
                    /*
                     poi.poiId=sqlite3_column_int(stmt,0);
                     poi.position=CGPointMake(sqlite3_column_int(stmt,1),sqlite3_column_int(stmt,2));
                     poi.floor=sqlite3_column_int(stmt,3);
                     poi.description=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                     poi.type=sqlite3_column_int(stmt,5);
                     poi.category=
                     [NSNumber numberWithInt:(sqlite3_column_int(stmt,6)*1000+sqlite3_column_int(stmt,7)*100+sqlite3_column_int(stmt,8))];
                     poi.iconFileName=[self IconFileName:sqlite3_column_int(stmt,6)
                     Category2:sqlite3_column_int(stmt,7)
                     Category3:sqlite3_column_int(stmt,8)];
                     */
                }
                
                [result addObject:poi];
                
                //NSLog(@"id : %d",sqlite3_column_int(stmt,0));
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return result;
}

-(MAPoi *)getPOIInfoFromId:(int)poiId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT  POI_ID, PRJ_ID, BLD_ID, FLOOR_ID, CODE_, NAME_, POSITIONING_TYPE_CODE, POSITIONED,  CATEGORY_ID ,DETAIL_ID, SPACE_ID, X_, Y_, Z_, TAGS_, DESC_"
                       " FROM PWP_POIINFO"
                       " WHERE POI_ID=%d"
                       , poiId
                       ];
    
    sqlite3 *myDB;
    NSString *projectID = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];

    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", projectID, projectID]];
    const char *dbPath = [targetPath UTF8String];
    MAPoi* poi= [[MAPoi alloc] init];
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                
                {
                    poi.poi_id=sqlite3_column_int(stmt,0);
                    poi.prj_id=sqlite3_column_int(stmt,1);;
                    poi.bld_id=sqlite3_column_int(stmt,2);;
                    poi.floor_id=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)];;
                    poi.code=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                    poi.name=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)];
                    poi.positioning_type_code=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,6)];
                    poi.positioned=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,7)];
                    poi.category_id=sqlite3_column_int(stmt,8);
                    poi.detail_id=sqlite3_column_int(stmt,9);
                    poi.space_id=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,10)];
                    poi.x=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,11)];
                    poi.y=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,12)];
                    poi.z=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,13)];
                    poi.tags=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,14)];
                    poi.desc=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,15)];
                    poi.icon_stored_name=[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,16)];
                    
                }
                
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return poi;


}

-(NSArray *) getCategoryList:(NSString *)prjId
{
    NSMutableArray *retArray = [[NSMutableArray alloc] initWithCapacity:3];
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT category_id, set_id, name_kr "
                       "FROM PWP_POICATEGORY "
                       "ORDER BY category_id"
                       ];
    
    NSLog(@"query : %@", query);
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                MACategory *category = [[MACategory alloc] init];
                category.categoryId = sqlite3_column_int(stmt, 0);
                category.setId = sqlite3_column_int(stmt, 1);
                category.name_kr = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];

                [retArray addObject:category];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return retArray;

}

-(NSArray*)PoiListWithFilter:(PoiFilter*)filter
{
    // Justin
    /*NSString* query = [NSString stringWithFormat:
                       @"SELECT  DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_REAL_NAME) "
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID "
                       " AND A.FLOOR_ID=%d "
                       , filter.Floor_id
                       ];
    */
    NSString* query = [NSString stringWithFormat:
                       @"SELECT  DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_STORED_NAME), D.NAME_KR, D.FLOOR_IDX, D.BASE_FLOOR"
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C, PWP_FLOORINFO D "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID AND A.POSITIONED='Y'"
                       " AND A.FLOOR_ID=%d AND D.FLOOR_ID=%d"
                       , filter.Floor_id, filter.Floor_id
                       ];
    
    
    
    return [self PoiListWithQuery:query];
}


-(MAZoneInfo*)getZoneInfo:(NSString*)prjId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT zone_id, zone_ver "
                       "FROM pwp_ZoneInfo "
                       "WHERE prj_id = %d", [CommonLib ExternalIdValueToInternalIdValue:prjId]
                       ];
    
    NSLog(@"query : %@", query);
    
    MAZoneInfo* zoneInfo = [MAZoneInfo alloc];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                zoneInfo.zone_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                zoneInfo.zone_ver = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return zoneInfo;
}

- (float) getNorthAxis:(NSString *)prjId
{
    float northAxis = 0.0;
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT north_axis "
                       "FROM pwp_BuildingInfo "
                       "WHERE prj_id = %d", [CommonLib ExternalIdValueToInternalIdValue:prjId]
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                northAxis = [[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)] floatValue];
               
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return northAxis;

}

-(MABuildingInfo*)getBuilding:(NSString*)prjId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT bld_id, bld_ver, name_kr, default_floor "
                       "FROM pwp_BuildingInfo "
                       "WHERE prj_id = %d", [CommonLib ExternalIdValueToInternalIdValue:prjId]
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query];
    MABuildingInfo* builgingInfo = [[MABuildingInfo alloc] init];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                builgingInfo.building_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                builgingInfo.building_ver = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
                builgingInfo.building_name = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return builgingInfo;
}

-(NSArray *)getBuildingInfo:(NSString *)zoneId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT bld_id, bld_ver, name_kr  "
                       "FROM pwp_BuildingInfo "
                       "WHERE zone_id = %@", zoneId
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    NSMutableArray *bldgArray = [[NSMutableArray alloc] initWithCapacity:10];
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                MABuildingInfo* builgingInfo = [[MABuildingInfo alloc] init];

                builgingInfo.building_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                builgingInfo.building_ver = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
                builgingInfo.building_name = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];
                builgingInfo.default_floor = [NSString stringWithFormat:@"%d",sqlite3_column_int(stmt,3)];
                [bldgArray addObject:builgingInfo];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return bldgArray;
}

- (NSArray *) getSbmFileList:(NSString *)prjId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT B.SBM_FILE "
                       "FROM PWP_BUILDINGINFO A, PWP_FLOORINFO B, PWP_ZONEINFO C "
                       "WHERE A.PRJ_ID = %d AND A.BLD_ID = B.BLD_ID AND A.PRJ_ID = C.PRJ_ID "
                       "ORDER BY B.FLOOR_IDX"
                       , [CommonLib ExternalIdValueToInternalIdValue:prjId]];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    NSMutableArray *retArray = [[NSMutableArray alloc] initWithCapacity:10];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                NSString *sbmFile = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                sbmFile = [sbmFile substringWithRange:NSMakeRange(2, [sbmFile length]-2)];
                
                [retArray addObject:sbmFile];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return retArray;
}

-(MALoadMapInfo*)getLoadMapInfo:(NSString*)prjId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    MALoadMapInfo *loadMapInfo = [[MALoadMapInfo alloc] init];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT B.BLD_ID, B.FLOOR_ID, B.SBM_FILE, C.ZONE_ID, A.NAME_EN "
                       "FROM PWP_BUILDINGINFO A, PWP_FLOORINFO B, PWP_ZONEINFO C "
                       "WHERE A.PRJ_ID = %d AND A.BLD_ID = B.BLD_ID AND A.DEFAULT_FLOOR = B.FLOOR_IDX AND A.PRJ_ID = C.PRJ_ID "
                       , [CommonLib ExternalIdValueToInternalIdValue:prjId]];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                loadMapInfo.project_id = prjId;
                loadMapInfo.building_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                loadMapInfo.floor_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
                loadMapInfo.sbm_file = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];
                loadMapInfo.zone_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)];
                loadMapInfo.building_name_e = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                // 파일명 앞에 붙은 ./ 제거..
                loadMapInfo.sbm_file = [loadMapInfo.sbm_file substringWithRange:NSMakeRange(2, [loadMapInfo.sbm_file length]-2)];
                
                
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return loadMapInfo;
}

- (NSArray *) getSameFloor:(MAFloorInfo *)floorInfo
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT SBM_FILE "
                       "FROM PWP_FLOORINFO B "
                       "WHERE group_id = %d"
                       , floorInfo.groupId];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    NSMutableArray *ret = [[NSMutableArray alloc] initWithCapacity:2];
    
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                NSString *sbmFile = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                [ret addObject:[sbmFile substringWithRange:NSMakeRange(2, [sbmFile length]-2)]];
                
                
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return ret;
    
}


- (NSString *) getSbmGroupName:(NSString *)floorId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* query = [NSString stringWithFormat:
                       @"SELECT SBM_FILE "
                       "FROM PWP_FLOORINFO B "
                       "WHERE FLOOR_ID = %@"
                       , floorId];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    NSString *ret;
    
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                NSString *sbmFile = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                ret = [sbmFile substringWithRange:NSMakeRange(2, [sbmFile length]-2)];
                
               
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return ret;

}

-(MAFloorInfo *)getFloorById:(NSString *)floorId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    
    MAFloorInfo *floorInfo = [[MAFloorInfo alloc] init];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT BLD_ID, FLOOR_ID, BASE_FLOOR, NAME_KR, NAME_EN, SBM_FILE, GROUP_ID "
                       " FROM PWP_FLOORINFO "
                       " WHERE BLD_ID = %d AND FLOOR_ID = %@ AND is_main = 'True'" , stateManager.current_building_id, floorId
                       ];
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    //NSString *prjId = [NSString stringWithFormat:@"%d",stateManager.current_project_id];
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                
                floorInfo.building_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                floorInfo.floor_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
                floorInfo.base_floor = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];
                floorInfo.name_kr = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)];
                floorInfo.name_en = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                floorInfo.sbm_file = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)];
                floorInfo.groupId = sqlite3_column_int(stmt, 6);
                
                // 파일명 앞에 붙은 ./ 제거..
                floorInfo.sbm_file= [floorInfo.sbm_file substringWithRange:NSMakeRange(2, [floorInfo.sbm_file length]-2)];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return floorInfo;

}

-(MAFloorInfo *)getFloor:(int)floor
{
    MAStateManager *stateManager = [MAStateManager Instance];
    
    MAFloorInfo *floorInfo = [[MAFloorInfo alloc] init];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT BLD_ID, FLOOR_ID, BASE_FLOOR, NAME_KR, NAME_EN, SBM_FILE, GROUP_ID "
                       " FROM PWP_FLOORINFO "
                       " WHERE BLD_ID = %d AND BASE_FLOOR = %d " , stateManager.current_building_id, floor
                       ];
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    //NSString *prjId = [NSString stringWithFormat:@"%d",stateManager.current_project_id];
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
               
                
                floorInfo.building_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                floorInfo.floor_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
                floorInfo.base_floor = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];
                floorInfo.name_kr = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)];
                floorInfo.name_en = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                floorInfo.sbm_file = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)];
                floorInfo.groupId = sqlite3_column_int(stmt, 6);
                
                // 파일명 앞에 붙은 ./ 제거..
                floorInfo.sbm_file= [floorInfo.sbm_file substringWithRange:NSMakeRange(2, [floorInfo.sbm_file length]-2)];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return floorInfo;

}

-(NSArray *)getFloorInfo:(NSString*)bldgId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    
    
    NSMutableArray *floorArray = [[NSMutableArray alloc] initWithCapacity:10];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT BLD_ID, FLOOR_ID, BASE_FLOOR, NAME_KR, NAME_EN, SBM_FILE, GROUP_ID "
                       " FROM PWP_FLOORINFO "
                       " WHERE BLD_ID = %@ AND is_main = 'True' ORDER BY BASE_FLOOR DESC " , bldgId
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    //const char *dbPath = [[[Resource Instance]PoiDbFilePath] UTF8String];
    //const char *dbPath = [[[NSBundle mainBundle] pathForResource:@"skt_info" ofType:@"db"] UTF8String];
    //NSString *prjId = [NSString stringWithFormat:@"%d",stateManager.current_project_id];
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                MAFloorInfo *floorInfo = [[MAFloorInfo alloc] init];
                
                floorInfo.building_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                floorInfo.floor_id = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)];
                floorInfo.base_floor = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)];
                floorInfo.name_kr = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)];
                floorInfo.name_en = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)];
                floorInfo.sbm_file = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)];
                floorInfo.groupId = sqlite3_column_int(stmt, 6);
                // 파일명 앞에 붙은 ./ 제거..
                floorInfo.sbm_file= [floorInfo.sbm_file substringWithRange:NSMakeRange(2, [floorInfo.sbm_file length]-2)];
                
                [floorArray addObject:floorInfo];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return floorArray;
}

- (NSString *)getDefaultZoneId
{
    NSString *retValue = @"";
    MAStateManager *stateManager = [MAStateManager Instance];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT ZONE_ID"
                       " FROM PWP_ZONEINFO "
                       " WHERE PRJ_ID = %d" ,  stateManager.current_project_id
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    NSString *projectID = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",projectID, projectID]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                retValue = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
               
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return retValue;

}

- (NSString *)getDefaultBuildingId
{
    NSString *retValue = @"";
    MAStateManager *stateManager = [MAStateManager Instance];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT BLD_ID"
                       " FROM PWP_BUILDINGINFO "
                       " WHERE PRJ_ID = %d AND ZONE_ID = %@" ,  stateManager.current_project_id, [self getDefaultZoneId]
                       ];
    
   [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    NSString *projectID = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",projectID, projectID]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                retValue = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return retValue;

}



- (NSString *)getDefaultFloorId
{
    NSString *retValue = @"";
    MAStateManager *stateManager = [MAStateManager Instance];
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT B.FLOOR_ID"
                       " FROM PWP_BUILDINGINFO A, PWP_FLOORINFO B"
                       " WHERE A.PRJ_ID = %d AND A.DEFAULT_FLOOR = B.FLOOR_IDX" ,  stateManager.current_project_id
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query];
    
    sqlite3 *myDB;
    
    NSString *projectID = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db",projectID, projectID]];
    const char *dbPath = [targetPath UTF8String];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                
                retValue = [CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)];
                
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return retValue;
}

- (float) getMaxDistance
{
    float maxValue = 0.0f;
    MAStateManager *stateManager = [MAStateManager Instance];
    
    NSString* query1 = [NSString stringWithFormat:
                        @"SELECT max(cam_max_dist)"
                        " FROM PWP_POILODINFO"
                        ];
    
    [[DebugOutput instance] output:@"query : %@", query1];
    
    sqlite3 *myDB;
    
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    //MALodInfo *info = [[MALodInfo alloc] init];
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query1 UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                maxValue = [[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)] floatValue];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    NSLog(@"max distance = %f", maxValue);
    return maxValue;
}

- (MALodInfo *)getLodInfo:(int)floorId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    
    MALodInfo *info = [[MALodInfo alloc] init];
    
    NSString* query1 = [NSString stringWithFormat:
                        @"SELECT level_cnt, cam_max_dist, icon_size_0, icon_size_1, icon_size_2, icon_size_3,icon_size_4, icon_size_5, icon_size_6, icon_size_7"
                        " icon_size_8, icon_size_9"
                        " FROM PWP_POILODINFO"
                        " WHERE floor_id = %d" , floorId
                       ];
    
    [[DebugOutput instance] output:@"query : %@", query1];
    
    sqlite3 *myDB;
    
    NSString *prjId = [CommonLib InternalIdValueToExternalIdValue:stateManager.current_project_id type:@"P"];
    
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@.db", prjId, prjId]];
    const char *dbPath = [targetPath UTF8String];
    
    //MALodInfo *info = [[MALodInfo alloc] init];
    NSMutableArray *sizeArray = [[NSMutableArray alloc] initWithCapacity:10];
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query1 UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                info.floorId = floorId;
                info.levelCount = [[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)] intValue];
                info.cameraMaxDistance = [[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)] floatValue];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,6)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,7)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,8)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,9)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,10)] intValue] ]];
                [sizeArray addObject:[NSNumber numberWithInt:[[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,11)] intValue] ]];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    NSString* query2 = [NSString stringWithFormat:
                        @"SELECT detail_id, icon_type_0, icon_type_1, icon_type_2, icon_type_3,icon_type_4, icon_type_5, icon_type_6, icon_type_7"
                        " icon_type_8, icon_type_9"
                        " FROM PWP_POILODTYPE"
                        " WHERE floor_id = %d" , floorId
                        ];
    
    [[DebugOutput instance] output:@"query : %@", query2];
    
    if (sqlite3_open(dbPath, &myDB)==SQLITE_OK) {
        
        const char *sql = [query2 UTF8String];
        
        sqlite3_stmt *stmt;
        
        if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, NULL)==SQLITE_OK) {
            
           // NSMutableArray *detail = [[NSMutableArray alloc] initWithCapacity:10];
            while(sqlite3_step(stmt)==SQLITE_ROW) {
                MALodIconInfo *iconInfo = [[MALodIconInfo alloc] init];
                NSMutableArray *typeArray = [[NSMutableArray alloc] initWithCapacity:10];
                //detail = [[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)] intValue];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,1)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,2)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,3)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,4)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,5)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,6)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,7)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,8)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,9)]];
                [typeArray addObject:[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,10)]];
                
                [iconInfo.sizeArray addObjectsFromArray:sizeArray];
                [iconInfo.typeArray addObjectsFromArray:typeArray];
                iconInfo.detailId = [[CommonLib CStringToNSString:(const char*)sqlite3_column_text(stmt,0)] intValue];
                
                [info.iconInfo addObject:iconInfo];
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(myDB);
    
    return info;
}


@end
