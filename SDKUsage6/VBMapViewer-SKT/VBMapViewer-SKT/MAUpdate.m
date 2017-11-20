//
//  MAUpdate.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 18..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MAUpdate.h"
#import "MADbPool.h"
#import "MAZoneInfo.h"
#import "MABuildingInfo.h"
#import "MALoadMapInfo.h"

#import "CommonLib.h"

#import "HttpComm.h"
#import "ZipArchive.h"

#import "SBJson.h"
#import "AFNetworking.h"
#import "MAStateManager.h"
#import "DebugOutput.h"

@implementation MAUpdate
{
    NSDictionary *dbResultDic;
    NSDictionary *zoneResultDic;
    NSDictionary *buildingResultDic;
    
    MADbPool *dbPool;
    
    MAUpdateState currentState;
    BOOL isDownloadState;

    NSMutableArray* fileInfoList;
    
    NSString *userId;
    NSString *userPw;
    
    long long totalDownloadFileSize;
    long long currentDownloadFileSize;
}

-(id)init
{
    self = [super init];
    
    if (self) {
        dbPool = [MADbPool Instance];
        
        isDownloadState = NO;
        
        /*
        // Normal
        userId = @"mobilemap@ezcarto.com";
        userPw = @"KoyIUg6jfGw5RXtzUJq/tw==";
         */
        
        // SKKnights
       // userId = @"admin";
        userId = @"qiujun";
        userPw = @"1111";

    }
    
    return self;
}

-(BOOL)isExistProjectData:(int)prjId;
{
    
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[CommonLib InternalIdValueToExternalIdValue:prjId type:@"P"]];
    // 로컬에 데이터 존재여부 확인
    if([[NSFileManager defaultManager] fileExistsAtPath:targetPath])
    {
        return YES;
    }
    
    return NO;
}

-(void)initDataFromServer
{
    [[DebugOutput instance] output:@"서버로부터 데이터 수신 중..."];
    
    if (self.serverUrl == nil) {
        NSAssert(NO, @"Server URL 없음!");
    }
    if (self.projectId == nil) {
        NSAssert(NO, @"Project ID 없음!");
    }
    
    // 1. DB/icon
    [self requestDataFromServer:@"/app/itfc/project/serviceData.json"
           afterTaskBlock:^(NSDictionary *resultDic) {
               dbResultDic = resultDic;
               
               [[DebugOutput instance] output:@"get db/icon %@", dbResultDic];
               
               NSArray *retArray = [resultDic valueForKey:@"result"];
               NSString *result = [retArray valueForKey:@"resultCode"];
               if ([result isEqualToString:@"F"] == YES) {
                   [self.delegate onNetworkFailed];
               } else {
                   [self.delegate onReadyForUpdate];
               }
               return;
           }];
    
    // 2. Zone
    [self requestDataFromServer:@"/app/itfc/zone/versionUrlList.json"
           afterTaskBlock:^(NSDictionary *resultDic) {
               zoneResultDic = resultDic;
               [[DebugOutput instance] output:@"get zone %@", zoneResultDic];
               
               NSArray *retArray = [resultDic valueForKey:@"result"];
               NSString *result = [retArray valueForKey:@"resultCode"];
               if ([result isEqualToString:@"F"] == YES) {
                   [self.delegate onNetworkFailed];
               } else {
                   [self.delegate onReadyForUpdate];
               }
               return;
    }];
    
    // 3. Building
    [self requestDataFromServer:@"/app/itfc/building/versionUrlList.json"
           afterTaskBlock:^(NSDictionary *resultDic) {
               buildingResultDic = resultDic;
               [[DebugOutput instance] output:@"get buildling %@", buildingResultDic];
               
               NSArray *retArray = [resultDic valueForKey:@"result"];
               NSString *result = [retArray valueForKey:@"resultCode"];
               if ([result isEqualToString:@"F"] == YES) {
                   [self.delegate onNetworkFailed];
               } else {
                   [self.delegate onReadyForUpdate];
               }
               return;
    }];

}

-(BOOL)isNeedDBUpdate
{
    /*
    NSArray *retArray = [dbResultDic valueForKey:@"result"];
    NSString *result = [retArray valueForKey:@"resultCode"];
    if ([result isEqualToString:@"F"] == YES) {
        return NO;
    }*/
    
    NSArray *array = [dbResultDic valueForKey:@"retItem"];

    NSString *iconServerVersion = [CommonLib removeDashInVersionString:[self getVerionFromUrlString:[array valueForKey:@"iconSetURL"]]];
    NSString *dbServerVersion = [CommonLib removeDashInVersionString:[self getVerionFromUrlString:[array valueForKey:@"sqliteDBURL"]]];
    
    NSString* iconLocalVersion = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%@_IconLocalVersion",self.projectId]];
    NSString* dbLocalVersion = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%@_DbLocalVersion",self.projectId]];
    
    if ([iconLocalVersion longLongValue] < [iconServerVersion longLongValue] || [dbLocalVersion longLongValue] < [dbServerVersion longLongValue]) {
        
        return YES;
    }
    
    return NO;
}

-(BOOL)isNeedZoneUpdate
{
    //MAZoneInfo* zoneInfo = [dbPool getZoneInfo:self.projectId];
    
    NSArray *array = [zoneResultDic valueForKey:@"retItem"];
    
    // Zone 정보는 하나만 존재해야한다.
    if ([array count] != 1 ) {
        //NSAssert(NO, @"Zone 정보는 하나만 존재해야합니다!");
    }
    
    NSString *fileSize;
    NSString *zoneId;
    NSString *zoneMobileURL;
    NSString *zoneVer;
    
    for (int i=0; i<[array count]; i++) {
        
        NSArray *arr = [array objectAtIndex:i];
        
        fileSize = [arr valueForKey:@"fileSize"];
        zoneId = [arr valueForKey:@"zoneId"];
        zoneMobileURL = [arr valueForKey:@"zoneMobileURL"];
        zoneVer = [arr valueForKey:@"zoneVer"];
        
        if ([fileSize integerValue] != 0) {
            break;
        }
    }
    
    NSString* zoneLocalVersion = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%@_ZoneLocalVersion",self.projectId]];
    if ([zoneLocalVersion longLongValue] < [[CommonLib removeDashInVersionString:zoneVer] longLongValue]) {
        //NSLog(@"%lld", [[CommonLib removeDashInVersionString:zoneInfo.zone_ver] longLongValue]);
        //NSLog(@"%lld", [[CommonLib removeDashInVersionString:zoneVer] longLongValue]);
        return YES;
    }
    
    return NO;

}

-(BOOL)isNeedBuildingUpdate
{
   
    //MABuildingInfo* buildingInfo = [dbPool getBuilding:self.projectId];
    
    NSArray *array = [buildingResultDic valueForKey:@"retItem"];
    
    NSString *buildingId;
    NSString *buildingMobileURL;
    NSString *buildingVer;
    NSString *fileSize;
    
    for (int i=0; i<[array count]; i++) {
        
        NSArray *arr = [array objectAtIndex:i];
        
        buildingId = [arr valueForKey:@"buildingId"];
        buildingMobileURL = [arr valueForKey:@"buildingMobileURL"];
        buildingVer = [arr valueForKey:@"buildingVer"];
        fileSize = [arr valueForKey:@"fileSize"];
        
    }
    
    NSString* buildingLocalVersion = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%@_BuildingLocalVersion",self.projectId]];

    if ([buildingLocalVersion longLongValue] < [[CommonLib removeDashInVersionString:buildingVer] longLongValue]) {
        
        //NSLog(@"%lld", [[CommonLib removeDashInVersionString:buildingInfo.building_ver] longLongValue]);
        //NSLog(@"%lld", [[CommonLib removeDashInVersionString:buildingVer] longLongValue]);
        return YES;
    }
    
    return NO;
}

-(void)requestDataFromServer:(NSString*)serverPath
             afterTaskBlock:(void (^)(NSDictionary *resultDic))afterBlock
{
    NSString *jsonString = [NSString stringWithFormat:@"{\"sender\":{\"senderId\":\"%@\",\"senderPwd\":\"%@\"},\"project\":{\"prjId\":\"%@\"}}", userId, userPw, self.projectId];
    //NSString *jsonString = @"{\"sender\":{\"senderId\":\"sktuser@gmail.com\",\"senderPwd\":\"1111\"},\"project\":{\"prjId\":\"P0002\"}}";
    //NSString *jsonString = @"{\"test\": {\"name\":\"jjjhhh\"}}";
    
    //NSLog(@"jsonString : %@", jsonString);
    
    SBJsonParser1* parser = [[SBJsonParser1 alloc] init];
    NSMutableDictionary* dic = [parser objectWithString:jsonString];

    //NSLog(@"dic : %@", dic);
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    
    //manager.securityPolicy.allowInvalidCertificates = YES;
    manager.requestSerializer = [AFJSONRequestSerializer serializer];
    //[manager.requestSerializer setAuthorizationHeaderFieldWithUsername:userId password:userPw];
    
    //[manager.requestSerializer setValue:@"text/html" forHTTPHeaderField:@"Content-Type"];
    //[manager.requestSerializer setValue:@"text/html" forHTTPHeaderField:@"Accept"];
    
    //jsonResponseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/­json", @"text/json", @"text/javascript", @"text/html", nil];
    //manager.responseSerializer = jsonResponseSerializer;
    //manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    //manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    //manager.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"text/html"];
    //manager.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"application/json"];
    
    //[manager.requestSerializer setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
    
    //
    //

    NSString *url = [NSString stringWithFormat:@"%@%@",self.serverUrl, serverPath];
    
    //NSLog(@"url : %@", url);
    
    [manager POST:url parameters:dic    success:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        //NSLog(@"responseObject : %@", responseObject);
        afterBlock(responseObject);
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        [self.delegate onNetworkFailed];
        NSLog(@"Error: %@", error);
    }];
}

-(NSString*)getVerionFromUrlString:(NSString*)urlString {
    int slashCount = 0;
    NSString *versionString = @"";
    for (int i=(int)[urlString length]; i>=0; i--) {
        //NSLog(@"char : %c", [urlString characterAtIndex:i-1]);
        
        if ([[NSString  stringWithFormat:@"%c",[urlString characterAtIndex:i-1]] isEqualToString:@"/"] ) {
            slashCount++;
            continue;
        }
        if (slashCount == 2) {
            break;
        }
        versionString = [NSString stringWithFormat:@"%c%@",[urlString characterAtIndex:i-1], versionString];
    }
    
    return versionString;
}


//-(void)updateData:(NSString*)downUrl fileSize:(long long)fileSize fileType:(NSString*)fileType
-(void)updateData
{
    isDownloadState=YES;
    MAStateManager *stateManager = [MAStateManager Instance];
    // 비동기 코드 시작.
    // 내부블럭의 코드 실행에 영향을 받지 않고 바로 넘어간다.
    dispatch_async( dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ), ^(void)
                   {
                       @try{
                           //[self FireUpdateState:MA_CHECKING];

                           totalDownloadFileSize=0;
                           {
                               [self CheckUpdateNeed:fileInfoList
                                      afterTaskBlock:^(NSString *fileType, NSString* downUrl, long long fileSize) {
                                          totalDownloadFileSize+=fileSize;
                                          //NSLog(@"total size = %lld",self.totalDownloadFileSize);
                                      }];
                           }
                           
                           if(totalDownloadFileSize>0)
                           {
                               self.currentFileName=nil;
                               currentDownloadFileSize=0;
                               
                               [self FireUpdateState:MA_DOWNLOADING];
                               
                               [self CheckUpdateNeed:fileInfoList
                                      afterTaskBlock:^(NSString *fileType, NSString* downUrl, long long fileSize) {
                                          
                                          self.currentFileName = [[self GetDownloadFolder] stringByAppendingPathComponent:downUrl];
                                          
                                          // 생성되는 파일명을 *.zip으로 설정하고 로컬에 다운받는다.
                                          NSString *fileName = @"";
                                          if ([fileType isEqualToString:@"db"]) {
                                              fileName = [NSString stringWithFormat:@"%@.db",self.projectId];
                                          }
                                          else {
                                              fileName = [NSString stringWithFormat:@"%@.zip",fileType];
                                          }
                                          
                                          if([self DownloadFile:fileName withFileURL:downUrl withFileSize:fileSize])
                                          {
                                        
                                              // db는 압축된 상태가 아니다.
                                              if (![fileType isEqualToString:@"db"]) {

                                                  // 다운받은 파일의 압축을 해제한다
                                                  // /프로젝트코드/zone
                                                  MALoadMapInfo *loadInfo = [[MALoadMapInfo alloc] init];
                                                  loadInfo = [dbPool getLoadMapInfo:self.projectId];
                                                  
                                                  NSString *tempID = @"";
                                                  if ([fileType isEqualToString:@"building"]) {
                                                      //tempID = self.buildingId;
                                                      tempID = loadInfo.building_id;
                                                  }

                                                  if([self UnzipFile:fileName extractPath:[NSString stringWithFormat:@"%@/%@",self.projectId, fileType] buildingId:tempID])
                                                  {
                                                      [[DebugOutput instance] output:@"압축 해제 완료 : %@", [NSString stringWithFormat:@"%@/%@",self.projectId, fileType]];
                                                  }
                                                  else
                                                  {
                                                      [[DebugOutput instance] output:@"압축 해제 실패..."];
                                                  }
                                                  
                                              }
                                              else
                                              {
                                                  NSFileManager *fm =[NSFileManager defaultManager];
                                                  
                                                  NSString* downPath=[[self GetDownloadFolder] stringByAppendingPathComponent:fileName];
                                                 
                                                  NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/%@",self.projectId, fileName]];
                                                  
                                                 
                                                  [fm moveItemAtPath:downPath toPath:targetPath error:nil];
                                                  
                                                  [CommonLib DeleteFile:downPath];
                                                  [[DebugOutput instance] output:@"이동완료 : %@ -> %@", downPath, targetPath];
                                                  
                                              }
                                              
                                              
                                              self.currentFileName=nil;
                                              // Justin
                                              //self.currentDownloadFileSize+=fileSize;
                                              
                                              // Justin
                                             [self FireUpdateState:MA_DOWNLOADING];
                                          }
                                          else{
                                              [self FireUpdateState:MA_FAILED];
                                              
                                              return;
                                          }
                                      }];
                               
                               if(isDownloadState==NO) {
                                   [self FireUpdateState:MA_FAILED];
                               } else {
                                   [self saveLastUpdateDate];
                                   [self FireUpdateState:MA_SUCCESS];
                               }
                           }
                           else
                           {
                               [self saveLastUpdateDate];
                               [self FireUpdateState:MA_SUCCESS];
                           }
                       }
                       @finally {
                           [self DeleteDownloadFiles];
                           isDownloadState=NO;
                       }
                   });
}

-(void)CheckUpdateNeed:(NSMutableArray*)fileList
        afterTaskBlock:(void (^)(NSString* fileType, NSString* downUrl, long long fileSize))afterBlock
{
    for(NSArray* fileInfo in fileList){
        NSString* fileType=[fileInfo objectAtIndex:0];
        NSString* downUrl=[fileInfo objectAtIndex:1];
        long long fileSize=[[fileInfo objectAtIndex:2] longLongValue];
        
        afterBlock(fileType,downUrl,fileSize);
        
        if(isDownloadState==NO)
            break;
    }
}

-(void) saveLastUpdateDate
{
    NSDateFormatter *formatter = [[NSDateFormatter alloc]init];
    [formatter setDateFormat:@"yyyyMMddHHmmss"];
    /* 사용에 따라 바꿔 쓰세요 format은 YYYY-MM-DD HH:MM:SS ±HHMM */
    
    NSString *updateDate = [formatter stringFromDate:[NSDate date]];
    [[NSUserDefaults standardUserDefaults] setObject:updateDate forKey:[NSString stringWithFormat:@"%d",[CommonLib ExternalIdValueToInternalIdValue:self.projectId]]];
}

-(BOOL)checkZoneVersion:(NSDictionary*)zoneDic
{
    MAZoneInfo* zoneInfo = [dbPool getZoneInfo:self.projectId];
    
    NSArray *array = [zoneDic valueForKey:@"retItem"];
    
    // Zone 정보는 하나만 존재해야한다.
    if ([array count] != 1 ) {
        //NSAssert(NO, @"Zone 정보는 하나만 존재해야합니다!");
    }
    
    NSString *fileSize;
    NSString *zoneId;
    NSString *zoneMobileURL;
    NSString *zoneVer;
    
    for (int i=0; i<[array count]; i++) {
        
        NSArray *arr = [array objectAtIndex:i];
        
        fileSize = [arr valueForKey:@"fileSize"];
        zoneId = [arr valueForKey:@"zoneId"];
        zoneMobileURL = [arr valueForKey:@"zoneMobileURL"];
        zoneVer = [arr valueForKey:@"zoneVer"];
        
        if ([fileSize integerValue] != 0) {
            break;
        }
        //NSLog(@"ver : %f", [[self removeDashInString:zoneVer] doubleValue]);
    }
    
    //NSLog(@"local ver: %f", [[CommonLib removeDashInVersionString:zoneInfo.zone_ver] doubleValue]);
    //NSLog(@"server ver: %f", [[CommonLib removeDashInVersionString:zoneVer] doubleValue]);
    
    if ([zoneInfo.zone_ver doubleValue] < [[CommonLib removeDashInVersionString:zoneVer] doubleValue]) {
        //[self updateZoneData:zoneMobileURL fileSize:[fileSize longLongValue]];
        return YES;
    }
    
    return NO;
}

-(void)startDbUpdate
{
    // NSLog(@"dbResultDic : %@", dbResultDic);
    NSArray *array = [dbResultDic valueForKey:@"retItem"];
    
    NSString *iconFileSize;
    NSString *dbFileSize;
    
    NSString *iconMobileURL;
    NSString *dbMobileURL;
    
    iconFileSize = [array valueForKey:@"iconSetFileSize"];
    iconMobileURL = [array valueForKey:@"iconSetURL"];
    
    dbFileSize = [array valueForKey:@"dbFileSize"];
    dbMobileURL = [array valueForKey:@"sqliteDBURL"];
    
    [[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[NSString stringWithFormat:@"%@_IconLocalVersion",self.projectId]];
    [[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[NSString stringWithFormat:@"%@_DbLocalVersion",self.projectId]];
    
    NSString *iconServerVersion = [CommonLib removeDashInVersionString:[self getVerionFromUrlString:iconMobileURL]];
    NSString *dbServerVersion = [CommonLib removeDashInVersionString:[self getVerionFromUrlString:dbMobileURL]];
    
    NSString* iconLocalVersion=[[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%@_IconLocalVersion",self.projectId]];
    NSString* dbLocalVersion=[[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithFormat:@"%@_DbLocalVersion",self.projectId]];
    
    fileInfoList = nil;
    fileInfoList = [[NSMutableArray alloc] init];
    
    NSMutableArray *iconArray = [[NSMutableArray alloc] init];
    [iconArray addObject:@"icons"];
    [iconArray addObject:iconMobileURL];
    [iconArray addObject:iconFileSize];
    
    NSMutableArray *dbArray = [[NSMutableArray alloc] init];
    [dbArray addObject:@"db"];
    [dbArray addObject:dbMobileURL];
    [dbArray addObject:dbFileSize];
    
    // icon 버전체크
    if ([iconLocalVersion longLongValue] < [iconServerVersion longLongValue]) {
        [[DebugOutput instance] output:@"icon 다운로드 준비 완료"];
        //[self updateData:iconMobileURL fileSize:[iconFileSize longLongValue] fileType:@"icons"];
        [fileInfoList addObject:iconArray];
        
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithLongLong:[iconServerVersion longLongValue]] forKey:[NSString stringWithFormat:@"%@_IconLocalVersion",self.projectId]];
    }
    
    // db 버전체크
    if ([dbLocalVersion longLongValue] < [dbServerVersion longLongValue]) {
        
        // db 다운로드
        [[DebugOutput instance] output:@"db 다운로드 준비 완료"];
        //[self updateData:dbMobileURL fileSize:[dbFileSize longLongValue] fileType:@"db"];
        [fileInfoList addObject:dbArray];
        
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithLongLong:[dbServerVersion longLongValue]] forKey:[NSString stringWithFormat:@"%@_DbLocalVersion",self.projectId]];
    }
    
    //NSLog(@"fileInfoList : %@", fileInfoList);
    
    //[self updateData];
    //NSString* currentVersion=[[NSUserDefaults standardUserDefaults] objectForKey:[self FileTag:fileName]];
    //[[NSUserDefaults standardUserDefaults] setObject:version forKey:[self FileTag:fileName]];
    //[[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[self FileTag:fileName]];
}

-(void)startZoneUpdate
{
    NSArray *array = [zoneResultDic valueForKey:@"retItem"];
    
    // Zone 정보는 하나만 존재해야한다.
    if ([array count] != 1 ) {
        //NSAssert(NO, @"Zone 정보는 하나만 존재해야합니다!");
    }
    
    NSString *fileSize;
    NSString *zoneId;
    NSString *zoneMobileURL;
    NSString *zoneVer;

    for (int i=0; i<[array count]; i++) {

        NSArray *arr = [array objectAtIndex:i];
        
        fileSize = [arr valueForKey:@"fileSize"];
        zoneId = [arr valueForKey:@"zoneId"];
        zoneMobileURL = [arr valueForKey:@"zoneMobileURL"];
        zoneVer = [arr valueForKey:@"zoneVer"];
        
        if ([fileSize integerValue] != 0) {
            break;
        }
    }

    NSMutableArray *zoneArray = [[NSMutableArray alloc] init];
    [zoneArray addObject:@"zone"];
    [zoneArray addObject:zoneMobileURL];
    [zoneArray addObject:fileSize];
        
        //NSLog(@"zone 다운로드");
    [fileInfoList addObject:zoneArray];
        
    [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithLongLong:[[CommonLib removeDashInVersionString:zoneVer] longLongValue]] forKey:[NSString stringWithFormat:@"%@_ZoneLocalVersion",self.projectId]];
        //[self updateData];
    
}

-(void)startBuildingUpdate
{
    
    NSArray *array = [buildingResultDic valueForKey:@"retItem"];
    
    NSString *buildingId;
    NSString *buildingMobileURL;
    NSString *buildingVer;
    NSString *fileSize;
    
    for (int i=0; i<[array count]; i++) {
        
        NSArray *arr = [array objectAtIndex:i];

        buildingId = [arr valueForKey:@"buildingId"];
        buildingMobileURL = [arr valueForKey:@"buildingMobileURL"];
        buildingVer = [arr valueForKey:@"buildingVer"];
        fileSize = [arr valueForKey:@"fileSize"];
    }
   

    NSMutableArray *bldgArray = [[NSMutableArray alloc] init];
    [bldgArray addObject:@"building"];
    [bldgArray addObject:buildingMobileURL];
    [bldgArray addObject:fileSize];
    [bldgArray addObject:buildingId];
        
    [[DebugOutput instance] output:@"Building 다운로드 준비 완료"];
    [fileInfoList addObject:bldgArray];
        
    [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithLongLong:[[CommonLib removeDashInVersionString:buildingVer] longLongValue]] forKey:[NSString stringWithFormat:@"%@_BuildingLocalVersion",self.projectId]];
        //[self updateData];
}

-(void)DeleteDownloadFiles
{
    [CommonLib DeleteFile:[self GetDownloadFolder]];
}

-(void)FireUpdateState:(MAUpdateState)state
{
    currentState=state;
    
    if (state == MA_FAILED) {
        [[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[NSString stringWithFormat:@"%@_ZoneLocalVersion",self.projectId]];
        [[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[NSString stringWithFormat:@"%@_BuildingLocalVersion",self.projectId]];
        [[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[NSString stringWithFormat:@"%@_IconLocalVersion",self.projectId]];
        [[NSUserDefaults standardUserDefaults] setObject:@"" forKey:[NSString stringWithFormat:@"%@_DbLocalVersion",self.projectId]];

    }
    
    dispatch_async( dispatch_get_main_queue(), ^(void)
                   {
                       [self.delegate onUpdateStateChanged:self];
                   });
}

-(void)cancelUpdate
{
    isDownloadState = NO;
}


// Justin - 삭제할 때 version 정보도 삭제해야 하지 않나?
-(void)deleteData:(int)prjId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:[CommonLib InternalIdValueToExternalIdValue:prjId type:@"P"]];
    
    [CommonLib DeleteFile:targetPath];
    
    [[DebugOutput instance] output:@"삭제 완료"];
}

-(NSString*)GetDownloadFolder
{
#if TARGET_IPHONE_SIMULATOR
    NSString *tempFolderPath =  @"/Users/VB/Temp";
#else
    NSString *tempFolderPath=[NSTemporaryDirectory() stringByAppendingPathComponent:@"Download"];
#endif
    [CommonLib CreateFolder:tempFolderPath];
    
    return tempFolderPath;
}

-(void) setCurrentFileSize:(long long)fileSize
{
    // Justin
    currentDownloadFileSize += fileSize;
    self.progress = (int)(((float)currentDownloadFileSize / (float)totalDownloadFileSize) * 100);
    
    [self FireUpdateState:MA_DOWNLOADING];
}

-(BOOL)DownloadFile:(NSString*)fileName withFileURL:(NSString*)fileURL withFileSize:(long long)fileSize
{
    NSString* tempFileName=[[self GetDownloadFolder] stringByAppendingPathComponent:fileName];
    
    // Justin
    if([[NSFileManager defaultManager] fileExistsAtPath:tempFileName])
        [CommonLib DeleteFile:tempFileName];
    
    NSOutputStream* outStream=[NSOutputStream outputStreamToFileAtPath:tempFileName append:YES];
    [outStream open];
    
    long long writtenSize=0;
    
    CFHTTPMessageRef response=[HttpComm HTTPRequestSync:fileURL
                                        withJobContinue:&isDownloadState
                                          withOutStream:outStream
                                        withWrittenSize:&writtenSize
                                                 update:self
                               
                               ];
    
    if(response)
        CFRelease(response);
    
    [outStream close];
    
    
    return fileSize==writtenSize;
}

-(BOOL)UnzipFile:(NSString*)fileName extractPath:(NSString*)extractPath buildingId:(NSString*)buildingId
{
    MAStateManager *stateManager = [MAStateManager Instance];
    NSString *extPath = extractPath;
    if (![buildingId isEqualToString:@""]) {
        extPath = [NSString stringWithFormat:@"%@/%@", extractPath, [CommonLib InternalIdValueToExternalIdValue:[buildingId intValue] type:@"B"]];
    }
    
    NSString* sourceFile=[[self GetDownloadFolder] stringByAppendingPathComponent:fileName];

    NSString* targetPath=[stateManager.downloadPath stringByAppendingPathComponent:extPath];
    
    [CommonLib DeleteFile:targetPath];
    
    ZipArchive *za = [[ZipArchive alloc] init];
    
    if ([za UnzipOpenFile:sourceFile])
    {
        BOOL ret = [za UnzipFileTo: targetPath overWrite: YES];
        
        if (ret)
        {
            [za UnzipCloseFile];
            return YES;
        }
    }
    
    return NO;
}

-(MAUpdateState)getState
{
    return currentState;
}
@end

