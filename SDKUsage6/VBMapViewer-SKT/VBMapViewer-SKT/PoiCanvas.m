//
//  PoiCanvas.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 1. 3..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "PoiCanvas.h"
#import "PoiIcon.h"

#import "MADbPool.h"
#import "MAPoi.h"

#import "Projection.h"

#import "UIBalloonView.h"

#import "MAStateManager.h"
#import "BalloonItem.h"
#import "MAVector.h"
#import "DebugOutput.h"


#import "MALodIconInfo.h"
#import "MAIcon.h"

#define POI_V_OFFSET    (-20)

typedef struct {
    int size;
    char *type;
} lodTypeInfo;



@implementation PoiCanvas {
    MAStateManager *stateManager;
    MADbPool* dbPool;
    Projection* projection;
    NSArray* poiList;
    NSMutableArray* balloonList;
    
    NSMutableSet* m_categories;
    NSMutableArray *dynamicPois;
    NSMutableArray *customIconList;
    
    int curLodLevel;
    int lodLevel;
    int curIconSize;
    float offSet;
    
    int poiTextSize;
    UIColor *poiTextColor;
    lodTypeInfo curLod;
    float currentLevel;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
        stateManager = [MAStateManager Instance];
        dbPool = [MADbPool Instance];
        
        self.poiFilter=[[PoiFilter alloc]init];
        self.poiFilter.delegate=self;
        
        balloonList = [[NSMutableArray alloc] initWithCapacity:10];
        customIconList = [[NSMutableArray alloc] initWithCapacity:10];
        
        curLodLevel = 0;
        lodLevel = -1;
        curIconSize = 24;
        offSet = 0.0;
        
        poiTextColor = [UIColor blackColor];
        poiTextSize = 13;
        
        currentLevel = 0.0f;
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

- (void)SetCategories:(NSArray *)catList
{
    if(m_categories)
        [m_categories removeAllObjects];
    else
        m_categories=[[NSMutableSet alloc]init];

    curLodLevel = -1;
    
    for (NSNumber *Id in catList) {
        [m_categories addObject:Id];
    }
    
    [self drawPoiIcons:YES];
}

- (NSArray *)getPoiCategory
{
    return [m_categories allObjects];
}

-(void)SelectedFloorChanged:(int)floor_id
{
    self.poiFilter.Floor_id=floor_id;
}

-(void)loadLodInfo
{
    self.lodInfo = [dbPool getLodInfo:stateManager.current_floor_id];
}

-(void)PoiFilterChanged:(PoiFilter *)poiFilter
{

    poiList = [dbPool PoiListWithFilter:poiFilter];
    
    [self drawPoiIcons:YES];
}

- (void)changePoiIcon:(int)poiId name:(NSString *)imageName
{
    for (MAPoi *poi in poiList) {
        if (poi.poi_id == poiId) {
            poi.image_name = imageName;
            
            MAIcon *icon = [[MAIcon alloc] init];
            icon.poiId = poiId;
            icon.imageName = imageName;
            
            BOOL found = NO;
            for (MAIcon *i in customIconList) {
                if (i.poiId == icon.poiId) {
                    found = YES;
                    break;
                }
            }
            if (found == NO) {
                [customIconList addObject:icon];
            }
            break;
        }
    }
    curLodLevel = -1;
    [self drawPoiIcons:YES];

}

- (void)restorePoiIcon:(int)poiId
{
    for (MAPoi *poi in poiList) {
        if (poi.poi_id == poiId) {
            poi.image_name = nil;
            
            for (MAIcon *icon in customIconList) {
                if (icon.poiId == poiId) {
                    [customIconList removeObject:icon];
                    break;
                }
            }
            
            break;
        }
    }
    curLodLevel = -1;
    [self drawPoiIcons:YES];
}

- (NSArray *) getPoiInfoList
{
  /*  NSString* query = [NSString stringWithFormat:
                       @"SELECT "
                       " DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_STORED_NAME), D.NAME_KR, D.FLOOR_IDX, D.BASE_FLOOR"
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C, PWP_FLOORINFO D "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID AND A.POSITIONED='Y' AND A.FLOOR_ID=D.FLOOR_ID"
                       ];
    [[DebugOutput instance] output:@"poi query : %@", query];
    
    return [dbPool PoiListWithQuery:query];*/
    return poiList;
}

- (NSArray *) getPoiInfoListByCategoryId:(int)categoryId
{
    NSString* query = [NSString stringWithFormat:
                       @"SELECT "
                       " DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_STORED_NAME), D.NAME_KR, D.FLOOR_IDX"
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C, PWP_FLOORINFO D "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID AND A.POSITIONED='Y'"
                       " AND A.FLOOR_ID=D.FLOOR_ID"
                       " AND A.CATEGORY_ID=%d"
                       ,categoryId
                       ];
    [[DebugOutput instance] output:@"poi query : %@", query];
    
    return [dbPool PoiListWithQuery:query];
}

- (NSArray *)getPoiInfoListByFloorId:(NSString *)floorId
{
    NSString* query = [NSString stringWithFormat:
                       @"SELECT "
                       " DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_STORED_NAME), D.NAME_KR, D.FLOOR_IDX"
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C, PWP_FLOORINFO D "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID AND A.POSITIONED='Y'"
                       " AND A.FLOOR_ID=%@ AND D.FLOOR_ID=%@"
                       ,floorId, floorId
                       ];
    [[DebugOutput instance] output:@"poi query : %@", query];
    
    return [dbPool PoiListWithQuery:query];
}

-(void)initPoiList:(NSArray *)list
{
    /*
    NSString* query = [NSString stringWithFormat:
                       @"SELECT "
                       " DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_STORED_NAME), D.NAME_KR, D.FLOOR_IDX "
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C, D.NAME_KR, D.FLOOR_IDX "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID AND A.POSITIONED='Y'  AND A.FLOOR_ID=D.FLOOR_ID"
                       ];*/
    
    NSString* query = [NSString stringWithFormat:
                       @"SELECT "
                       " DISTINCT A.POI_ID, A.PRJ_ID, A.BLD_ID, A.FLOOR_ID, A.CODE_, A.NAME_, A.POSITIONING_TYPE_CODE, A.POSITIONED,  A.CATEGORY_ID ,A.DETAIL_ID, A.SPACE_ID, A.X_, A.Y_, A.Z_, A.TAGS_, A.DESC_, TRIM(C.ICON_STORED_NAME), D.NAME_KR, D.FLOOR_IDX, D.BASE_FLOOR"
                       " FROM PWP_POIINFO A, PWP_POICATEGORYDETAIL B, PWP_POIICONSET C, PWP_FLOORINFO D "
                       " WHERE A.DETAIL_ID=B.DETAIL_ID AND B.ICONSET_ID=C.ICONSET_ID AND A.POSITIONED='Y' AND A.FLOOR_ID=D.FLOOR_ID"
                       ];

    [[DebugOutput instance] output:@"poi query : %@", query];
    
    poiList = [dbPool PoiListWithQuery:query];
    
    for (MAPoi *p1 in poiList) {
        for (MAPoi *p2 in list) {
            if (p1.poi_id == p2.poi_id) {
                p1.visible = YES;
                p1.x = p2.x;
                p1.y = p2.y;
                p1.z = [NSString stringWithFormat:@"%f", [p2.z floatValue] * -1.0];
                p1.name = p2.name;
            }
        }
    }
    //poiList = [list copy];
    [self drawPoiIcons:YES];
    
    
    
}

-(void)setDynamicPoi:(NSArray *)pois
{
    dynamicPois = (NSMutableArray *)pois;
    [self drawPoiIcons:YES];
}

-(void)removeDynamicPoiAll
{
    [dynamicPois removeAllObjects];
    [self drawPoiIcons:YES];
}

- (void) showDynamicPoi:(BOOL)show x:(float)x y:(float)y z:(float)z
{
    for (MAPoi *poi in dynamicPois) {
		if (poi.poi_id == 500000) {
			poi.visible = show;
			poi.x = [NSString stringWithFormat:@"%f", x];
			poi.y = [NSString stringWithFormat:@"%f", y];
			poi.z = [NSString stringWithFormat:@"%f", z];
		}

    }
	
    [self drawPoiIcons:NO];

}

- (void) refresh
{
    [self performSelector:@selector(reDraw) withObject:nil afterDelay:0.001f];
    
}

- (void) reDraw
{
    [self drawPoiIcons:NO];

}

-(void)removeDynamicPoiById:(int)Id
{
    BOOL find = NO;
    int index = 0;
    for (MAPoi *poi in dynamicPois) {
        if (poi.poi_id == Id) {
            find = YES;
            break;
        } else {
            index++;
        }
    }
    
    if (find) {
        [dynamicPois removeObjectAtIndex:index];
        [self drawPoiIcons:YES];
    }
}

-(void)removeDynamicPoiByArray:(NSArray *)pois
{
    NSMutableArray *findArray = [[NSMutableArray alloc] initWithCapacity:1];
    for (MAPoi *poi1 in dynamicPois) {
        for (MAPoi *poi2 in pois) {
            if (poi1.poi_id == poi2.poi_id) {
                [findArray addObject:poi1];
            }
        }
    }
    
    [dynamicPois removeObjectsInArray:findArray];
    [self drawPoiIcons:YES];
}

-(void)onGLViewUpdated:(Projection*)_projection cameraYaw:(float)cameraYaw cameraPitch:(float)cameraPitch cameraHeight:(float)cameraHeight cameraOffset:(float)cameraOffset
{
    if (_projection == projection) {
        return;
    }
    
    projection=_projection;
	
    if (self.lodInfo.cameraMaxDistance == 0) {
        lodLevel = curLodLevel = -1;
    } else {
        offSet = cameraOffset;
        if (offSet > 0) {
            if (offSet > self.lodInfo.cameraMaxDistance) {
                offSet = self.lodInfo.cameraMaxDistance;
            } else if (offSet <= CAMERA_MIN_DIST) {
                offSet = CAMERA_MIN_DIST + 1.0;
            }
            lodLevel = (self.lodInfo.levelCount * (offSet - CAMERA_MIN_DIST)) / (self.lodInfo.cameraMaxDistance - CAMERA_MIN_DIST) - 1;
        }
    }
    [self drawPoiIcons:NO];
}

-(NSArray *)listDynamicPoi
{
    return dynamicPois;
}

- (void) setGroundLevel:(float)level
{
    currentLevel = level;
}

- (void)showPoi:(BOOL)show
{
    for (MAPoi *poi in poiList) {
        poi.visible = show;
    }
    
    if (show == NO) {
        for (UIBalloonView *bv in self.subviews) {
            if ([bv isKindOfClass:[UIBalloonView class]]) {
                [bv removeFromSuperview];
            }
        }
        
        [balloonList removeAllObjects];
        [m_categories removeAllObjects];
    }
    curLodLevel = -1;
    [self drawPoiIcons:YES];
}

- (void)showPoiById:(NSArray *)Ids show:(BOOL)show
{
    for (MAPoi *poi in poiList) {
        for (NSNumber *Id in Ids) {
            if (poi.poi_id == [Id intValue]) {
                poi.visible = show;
            }
        }
    }
    curLodLevel = -1;
    [self drawPoiIcons:YES];
}

- (void)showPoiByInfo:(NSArray *)pois show:(BOOL)show
{
    for (MAPoi *poi1 in poiList) {
        for (MAPoi *poi2 in pois) {
            if (poi1.poi_id == poi2.poi_id) {
                poi1.visible = show;
            }
        }
    }
    curLodLevel = -1;
    [self drawPoiIcons:YES];
}

- (MAPoi *)getPoiInfoById:(int)poiId
{
    for (MAPoi *poi in poiList) {
        if (poi.poi_id == poiId) {
            return poi;
        }
    }
    return nil;
}

-(void)getPoiInfo:(MAPoi *)poi;
{
    memset(&curLod, 0, sizeof(lodTypeInfo));
    if (lodLevel < 0) {
        lodLevel = 0;
    }
    for (int i = 0; i <[self.lodInfo.iconInfo count]; i++) {
        MALodIconInfo *iconInfo = [self.lodInfo.iconInfo objectAtIndex:i];
        if (poi.detail_id == iconInfo.detailId) {
            curLod.size = [[iconInfo.sizeArray objectAtIndex:lodLevel] intValue];
            curLod.type = (char *)[[iconInfo.typeArray objectAtIndex:lodLevel] UTF8String];
            if (curLod.type == NULL) {
                [[DebugOutput instance] output:@"poi type is null - error"];
                
                //WONIL strcpy는 사용하지 말라는 필수 수정 요청 사항이 있음.
                //strcpy(curLod.type, "null");
                NSString* strNull = @"null";
                curLod.type = (char *)[strNull UTF8String];
            }
            break;
        }
    }
}

- (void)setCustomPoiIcon:(MAPoi *)poi
{
    for (MAIcon *icon in customIconList) {
        if (icon.poiId == poi.poi_id) {
            poi.image_name = icon.imageName;
            break;
        }
    }
}

-(void)drawPoiIcons:(BOOL)refresh
{
    // 아이콘 전체 삭제
    if (refresh || stateManager.isZone) {
        for(PoiIcon *pv in self.subviews)
        {
            if ([pv isKindOfClass:[PoiIcon class]]) {
                [pv removeFromSuperview];
            }
        }
    
        for (UIBalloonView *bv in self.subviews) {
            if ([bv isKindOfClass:[UIBalloonView class]]) {
                [bv removeFromSuperview];
            }
        }
    }
    // Zone일 경우 POI그리지 않는다.
    if (stateManager.isZone) return;
    

    for(MAPoi* poi in poiList)
    {
        if (poi.visible == NO) {
            continue;
        }
        
        if([m_categories count] > 0)
        {
            if([m_categories containsObject:[NSNumber numberWithInt:poi.category_id]]==NO)
                continue;
        }
        
       // NSLog(@"POI pos = %f, %f, %f", [poi.x floatValue], [poi.y floatValue], [poi.z floatValue]);
        
//#warning Ground Level 설정한 값을 POI 좌표 계산에도 적용 해 줘야 함.
        CGPoint poiPostion=[projection WorldToWindowWithScreenSize:self.frame.size withX:[poi.x floatValue] withY:([poi.y floatValue] - currentLevel/*수직이동량* 시청 지하1층인 경우 바닥 높이가 -660이므로 -(-660) =+660.f*/) withZ:[poi.z floatValue]];
       // NSLog(@"poi canvas :%f,%f, %f,%f",self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
//#warning OpenGL화면과 POI가 그려지는 화면의 원점이 다른 것 같은데, 수직으로 차이가 나는 양이 있으면 여기서 그 값을 보정해 줘야 합니다. 확인 부탁 드립니다.
        poiPostion.y += 20;
        
        //if(poiPostion.x>=0 && poiPostion.x<=self.frame.size.width&&poiPostion.y>=0&& poiPostion.y<=self.frame.size.height)
        {
            NSString *type = @"";
            NSNumber *size = [NSNumber numberWithInt:24];
            
            if (curLodLevel != lodLevel || refresh) {
                [self getPoiInfo:poi];
                if (curLod.size != 0 && curLod.type != nil) {
                    size = [NSNumber numberWithInt:curLod.size];
                    type = [NSString stringWithUTF8String:curLod.type ? curLod.type : ""];
                }
            }

            PoiIcon *poiIcon;
            if (refresh) {
                [self setCustomPoiIcon:poi];

                poiIcon = [[PoiIcon alloc] initWithFrame:CGRectMake(0, 0, 24, 24) andPoi:poi andIconSize:[size intValue] andTextColor:poiTextColor andTextSize:poiTextSize];
                poiIcon.delegate = self;
                poiIcon.tag = poi.poi_id;
            } else {
                for (PoiIcon *pv in self.subviews) {
                    if (pv.tag == poi.poi_id) {
                        poiIcon = pv;
                        break;
                    }
                }
            }
            
            if (curLodLevel != lodLevel || refresh) {
                [poiIcon setIconSize:[size intValue]];
                curIconSize = [size intValue];
            }
            
            poiIcon.center = CGPointMake(poiPostion.x, poiPostion.y  - poiIcon.frame.size.height/2 + POI_V_OFFSET);
            BOOL showPoi = YES;
            if (lodLevel != curLodLevel || refresh) {
                poiIcon.type = type;
            }
            if ([poiIcon.type isEqualToString:@"null"] || [poiIcon.type isEqualToString:@"ITEXT"]) {
                poiIcon.hidden = NO;
                [poiIcon showText:YES];
            } else if ([poiIcon.type isEqualToString:@"ICON"]) {
                poiIcon.hidden = NO;
                [poiIcon showText:NO];
               
            } else {
                poiIcon.hidden = NO;
                [poiIcon showText:NO];
//                showPoi = NO;
//                poiIcon.hidden = YES;
//                for (UIBalloonView *bv in self.subviews) {
//                    if ([bv isKindOfClass:[UIBalloonView class]]) {
//                        if (poiIcon.tag == bv.tag) {
//                            [bv removeFromSuperview];
//                        }
//                    }
//                }
//
//                for (BalloonItem *balloon in balloonList) {
//                    if (balloon.poi_id == poiIcon.tag) {
//                        [balloonList removeObject:balloon];
//                        break;
//                    }
//                }
            }
          
            
            if (refresh) {
                [self addSubview:poiIcon];
            }
            
            if(poiPostion.x>=0 && poiPostion.x<=self.frame.size.width&&poiPostion.y>= 5 && poiPostion.y<=self.frame.size.height - 40 && showPoi == YES) {
                poiIcon.hidden = NO;
            } else {
                
                poiIcon.hidden = YES;
            }

        }
        
    }
    
    if (curLodLevel != lodLevel && offSet > 0) {
        curLodLevel = lodLevel;
    }
    
    [self drawDynamicPois:refresh];
    [self drawBalloonView:refresh];
    
}

- (void) drawBalloonView:(BOOL)refresh
{
    for(PoiIcon *pv in self.subviews)
    {
        if ([pv isKindOfClass:[PoiIcon class]]) {
            for (BalloonItem *balloon in balloonList) {
                if (pv.maPoi.poi_id == balloon.poi_id) {
                    UIBalloonView *balloonView;
                    if (refresh) {
                        balloonView = [[UIBalloonView alloc] init:balloon posX:pv.center.x posY:pv.frame.origin.y - 30];
                        balloonView.delegate = self;
                        balloonView.tag = balloon.poi_id;
                        
                        [self addSubview:balloonView];
                    } else {
                        for (UIBalloonView *bv in self.subviews) {
                            if ([bv isKindOfClass:[UIBalloonView class]]) {
                                if (bv.tag == balloon.poi_id) {
                                    bv.center = CGPointMake(pv.center.x, pv.frame.origin.y - 17);
                                    
                                    if (bv.frame.origin.y > 0 && bv.frame.origin.y <  self.frame.size.height - 70) {
                                        bv.hidden  = NO;
                                    } else {
                                        bv.hidden = YES;
                                    }
                                }
                            }
                        }
                    }
                  
                }
            }
        }
    }

}

- (void) drawDynamicPois:(BOOL)refresh
{
    for (MAPoi *poi in dynamicPois) {
        
        //if (stateManager.current_floor_id != [poi.floor_id intValue]) {
        //    continue;
        //}
        
        CGPoint poiPostion=[projection WorldToWindowWithScreenSize:self.frame.size withX:[poi.x floatValue] withY:[poi.y floatValue] withZ:[poi.z floatValue]];
        poiPostion.y += 20;
        //if(poiPostion.x>=0 && poiPostion.x<=self.frame.size.width&&poiPostion.y>=0&& poiPostion.y<=self.frame.size.height)
        {
            
            PoiIcon *poiIcon;
            if (refresh) {
                poiIcon = [[PoiIcon alloc] initWithFrame:CGRectMake(0, 0, 24, 24) andPoi:poi andIconSize:curIconSize andTextColor:[UIColor redColor] andTextSize:poiTextSize];
                poiIcon.delegate = self;
                poiIcon.tag = poi.poi_id;
                
            } else {
                for (PoiIcon* pv in self.subviews) {
                    if (pv.tag == poi.poi_id) {
                        poiIcon = pv;
                        break;
                    }
                }
            }
            
            [poiIcon setIconSize:curIconSize];
            poiIcon.center = CGPointMake(poiPostion.x, poiPostion.y - poiIcon.frame.size.height/2/* + POI_V_OFFSET*/);
            
            
            // 화면바깥으로 나갈경우 그리지 않는다.
            if (/*poiIcon.center.y < poiIcon.frame.size.height/2 || (poiIcon.center.y + poiIcon.frame.size.height/2 )> self.frame.size.height ||*/ poi.visible == NO) {
                poiIcon.hidden = YES;
            } else {
                poiIcon.hidden = NO;
            }
            
            [poiIcon setTextColor:[UIColor redColor]];
            [poiIcon setBoldText];
            //[poiIcon showText:YES];
            if (refresh) {
                [self addSubview:poiIcon];
            }
        }
        
    }

}

- (void)setPoiTextSize:(int)size
{
    poiTextSize = size;
    for(PoiIcon *pv in self.subviews)
    {
        if ([pv isKindOfClass:[PoiIcon class]]) {
            [pv setTextSize:poiTextSize];
        }
    }
}

- (void)setPoiTextColor:(UIColor *)color
{
    poiTextColor = color;
    for(PoiIcon *pv in self.subviews)
    {
        if ([pv isKindOfClass:[PoiIcon class]]) {
            [pv setTextColor:poiTextColor];
        }
    }

}
/*
-(void)DrawPoiIcons:(NSArray*)_poiList cameraYaw:(float)yaw cameraPitch:(float)pitch cameraHeight:(float)height
{
 
    //NSLog(@"cameraYaw : %f", yaw);
    //NSLog(@"cameraPitch : %f", pitch);
    //NSLog(@"cameraHeight : %f", height);
    
    // Zone일 경우 POI그리지 않는다.
    
    
    // Camera 상하 피치
    float cameraPitch;
    if (isnan(pitch)) {
        cameraPitch = 0;
    }
    else {
        cameraPitch = pitch;
    }

    // 아이콘 전체 삭제
    for(PoiIcon *view in self.subviews)
    {
        if ([view isKindOfClass:[PoiIcon class]]) {
            [view removeFromSuperview];
        }
        
    }
    
    for (UIBalloonView *view in self.subviews) {
        if ([view isKindOfClass:[UIBalloonView class]]) {
            [view removeFromSuperview];
        }
    }
    if (stateManager.isZone) return;
    if (self.showPoi == NO) return;
    
    for(MAPoi* poi in _poiList)
    {

        CGPoint poiPostion=[projection WorldToWindowWithScreenSize:self.frame.size withX:[poi.x floatValue] withY:[poi.y floatValue] withZ:[poi.z floatValue]];
        
        if(poiPostion.x>=0 && poiPostion.x<=self.frame.size.width&&poiPostion.y>=0&& poiPostion.y<=self.frame.size.height)
        {
            PoiIcon *poiIcon = [[PoiIcon alloc] initWithFrame:CGRectMake(0, 0, 24, 24) withPoi:poi];
            poiIcon.delegate = self;
            //poiIcon.center = CGPointMake(poiPostion.x, poiPostion.y);
            //poiIcon.center = CGPointMake(poiPostion.x, poiPostion.y-poiIcon.frame.size.height/2);
            poiIcon.center = CGPointMake(poiPostion.x, (poiPostion.y-poiIcon.frame.size.height/2) + cameraPitch);
            
 
            // 1 | 2
            // -----
            // 3 | 4
            
            // 1
            if (poiPostion.x <= self.frame.size.width / 2 && poiPostion.y < self.frame.size.height / 2) {
                poiIcon.center = CGPointMake(poiPostion.x+(cameraPitch*10.0f), poiPostion.y+(cameraPitch*10.0f) - poiIcon.frame.size.height/2);
            }
            // 2
            else if (poiPostion.x >= self.frame.size.width / 2 && poiPostion.y < self.frame.size.height / 2) {
                poiIcon.center = CGPointMake(poiPostion.x-(cameraPitch*10.0f), poiPostion.y+(cameraPitch*10.0f) - poiIcon.frame.size.height/2);
            }
            // 3
            else if (poiPostion.x <= self.frame.size.width / 2 && poiPostion.y > self.frame.size.height / 2) {
                poiIcon.center = CGPointMake(poiPostion.x-(cameraPitch*10.0f), poiPostion.y-(cameraPitch*20.0f) - poiIcon.frame.size.height/2);
            }
            // 4
            else if (poiPostion.x >= self.frame.size.width / 2 && poiPostion.y > self.frame.size.height / 2) {
                poiIcon.center = CGPointMake(poiPostion.x+(cameraPitch*10.0f), poiPostion.y-(cameraPitch*20.0f) - poiIcon.frame.size.height/2);
            }
 

            // 화면바깥으로 나갈경우 그리지 않는다.
            if (poiIcon.center.y < poiIcon.frame.size.height/2 || (poiIcon.center.y + poiIcon.frame.size.height/2 )> self.frame.size.height) {
                continue;
            }
            
            [self addSubview:poiIcon];
        }
    }
    
    
    for(PoiIcon *view in self.subviews)
    {
        if ([view isKindOfClass:[PoiIcon class]]) {
         
            for (BalloonItem *balloon in balloonList) {
                if (view.maPoi.poi_id == balloon.poi_id) {
                    UIBalloonView *balloonView = [[UIBalloonView alloc] init:balloon.name posX:view.center.x posY:view.center.y - 40];
                    [self addSubview:balloonView];
                }
            }
        }
    }
    
}
*/

-(CGPoint)getScreenPoint:(MAVector *)pos
{
    return [projection WorldToWindowWithScreenSize:self.frame.size withX:pos.x withY:pos.y withZ:pos.z];
}

#pragma mark -

- (void)selectPoiIcon:(PoiIcon*)poiIcon withPoi:(MAPoi*)maPoi
{
    if ([_touchDelegate respondsToSelector:@selector(onPoiTouched:x:y:)]) {
        [[DebugOutput instance] output:@"POI 터치 : ID = %d, Name = %@, x = %f, y = %f, z = %f", maPoi.poi_id, maPoi.name, maPoi.x, maPoi.y, maPoi.z];
        BOOL response = [_touchDelegate onPoiTouched:maPoi.poi_id x:poiIcon.center.x y:poiIcon.frame.origin.y];
        if (response == YES) {
            return;
        } else {
            [balloonList removeAllObjects];
            for (UIBalloonView *view in self.subviews) {
                if ([view isKindOfClass:[UIBalloonView class]]) {
                    [view removeFromSuperview];
                }
            }
            
            if ([self containsBalloonItem:maPoi.poi_id] == NO) {
                BalloonItem *item = [[BalloonItem alloc] init];
                item.name = maPoi.name;
                item.poi_id = maPoi.poi_id;

                [balloonList addObject:item];
            
                UIBalloonView *balloonView = [[UIBalloonView alloc] init:item posX:poiIcon.center.x posY:poiIcon.frame.origin.y - 30];
                balloonView.delegate = self;
                balloonView.tag = maPoi.poi_id;
                [self addSubview:balloonView];
            }
        }
    }
}

-(BOOL)containsBalloonItem:(int)poiId
{
    BOOL contains = NO;
    for (BalloonItem *balloon in balloonList) {
        if (balloon.poi_id == poiId) {
            contains = YES;
            break;
        }
    }
    return contains;
}

- (void)selectBalloon:(BalloonItem *)item
{
    if ([_touchDelegate respondsToSelector:@selector(onBalloonTouched:)]) {
        
        [[DebugOutput instance] output:@"Balloon 터치 : ID = %d", item.poi_id];
        
        [balloonList removeAllObjects];
        for (UIBalloonView *view in self.subviews) {
            if ([view isKindOfClass:[UIBalloonView class]]) {
                    [view removeFromSuperview];
                }
            }
        }

        
        BOOL response = [_touchDelegate onBalloonTouched:item.poi_id];
    
    /*
        if (response == YES) {
            return;
        } else {
            [balloonList removeObject:item];
            for (UIBalloonView *view in self.subviews) {
                if ([view isKindOfClass:[UIBalloonView class]]) {
                    if (view.tag == item.poi_id) {
                        [view removeFromSuperview];
                    }
                }
            }

        }
    }*/

}
@end

