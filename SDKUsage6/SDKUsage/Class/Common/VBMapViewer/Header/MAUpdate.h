//
//  MAUpdate.h
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 18..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum
{
     MA_DOWNLOADING
    ,MA_SUCCESS
    ,MA_FAILED
}MAUpdateState;

@class MAUpdate;

@protocol MADelegate <NSObject>
@optional
-(void)onUpdateStateChanged:(MAUpdate*)state;
-(void)onNeedUpdate:(BOOL)update;
-(BOOL)onPoiTouched:(int)poiId x:(int)x y:(int)y;
-(BOOL)onBalloonTouched:(int)poiId;
-(void)onMapLoadingSuccess;
-(void)onMapLoadingFailed;
-(void)onNetworkFailed;
@required
-(void)onReadyForUpdate;
@end


@interface MAUpdate : NSObject

@property(nonatomic, strong) NSString *serverUrl;
@property(nonatomic, strong) NSString *projectId;
@property(nonatomic, strong) NSString *buildingId;

@property (nonatomic) int progress;

@property (nonatomic,strong) NSString* currentFileName;

@property(nonatomic,assign) id<MADelegate> delegate;

-(void)initDataFromServer;

-(void)startDbUpdate;
-(void)startZoneUpdate;
-(void)startBuildingUpdate;

-(BOOL)isNeedDBUpdate;
-(BOOL)isNeedZoneUpdate;
-(BOOL)isNeedBuildingUpdate;

-(BOOL)isExistProjectData:(int)prjId;

-(void)cancelUpdate;
-(void)deleteData:(int)prjId;
-(void)updateData;

-(MAUpdateState)getState;

-(void) setCurrentFileSize:(long long)fileSize;


@end
