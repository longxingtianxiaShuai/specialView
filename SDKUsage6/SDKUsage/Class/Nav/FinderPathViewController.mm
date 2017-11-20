//
//  FinderPathViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/16.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "FinderPathViewController.h"

#import "MAStateManager.h"
#import "MAMapView.h"
#import "MAFloorInfo.h"
#import "DropdownMenu.h"
#import "VBPathFinder.h"
#import "VBPathGuider.h"
#import "MAVector.h"
#import "PathNode.h"

#define kCameraAlgneMin 15.0f
#define kCameraAlgneMax 95.0f
const float kGround = 700.0f;
@interface FinderPathViewController ()<DropdownMenuDelegate,UIGestureRecognizerDelegate,MADelegate>{
    
    CGFloat _origional;
    vbPathNode *node;
}
@property (nonatomic,strong)MAManager *manager;
@property (nonatomic,copy)NSString *url;
@property (nonatomic,assign)int prjId;
@property (nonatomic,strong)NSArray *floorinfoArray;
@property (nonatomic,strong)MAPoi *selectedPoi;  // 选中的poi

@property (nonatomic,strong)DropdownMenu *changeFloorMenu;
@property (nonatomic,strong)DropdownMenu *changeBackColorMenu;

@property (nonatomic,strong)VBPathFinder *pathFinder;
 @property (nonatomic, strong) MAVector *myPos;
@end

@implementation FinderPathViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.navigationItem.title = @"寻找路径模式";
    [self addPanChange2Dto3D];
   
    
}
- (void)viewWillAppear:(BOOL)animated{

    [super viewWillAppear:animated];
    if (_manager == nil) {
        [self mapConfiguration];
    }
    
    [self setUpSubviews];
}




- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc{
    [self onExit_Clicked:nil];
    
}

- (void)onExit_Clicked:(id)sender
{
    // 맵 데이터 삭제 후, 사용하면 안됨.
    
 
    [_manager clearData];
    [[_manager getMapView] removeFromSuperview];
    [_manager clearMap];
}

#pragma mark 2Dto3D
- (void)addPanChange2Dto3D{
    _origional = 1;
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGestureDown:)];
    panGesture.minimumNumberOfTouches = 3;
    panGesture.delegate = self;
    [self.view addGestureRecognizer:panGesture];
    
}
-(void)panGestureDown:(UIPanGestureRecognizer*)sender{
    
    
    
    typedef NS_ENUM(NSUInteger, UIPanGestureRecognizerDirection) {
        UIPanGestureRecognizerDirectionUndefined,
        UIPanGestureRecognizerDirectionUp,
        UIPanGestureRecognizerDirectionDown,
        UIPanGestureRecognizerDirectionLeft,
        UIPanGestureRecognizerDirectionRight
    };
    
    
    static UIPanGestureRecognizerDirection direction = UIPanGestureRecognizerDirectionUndefined;
    switch (sender.state) {
        case UIGestureRecognizerStateBegan: {
            
            if (direction == UIPanGestureRecognizerDirectionUndefined) {
                CGPoint velocity = [sender velocityInView:self.view];
                BOOL isVerticalGesture = fabs(velocity.y) > fabs(velocity.x);
                if (isVerticalGesture) {
                    if (velocity.y > 0) {
                        direction = UIPanGestureRecognizerDirectionDown;
                    } else {
                        direction = UIPanGestureRecognizerDirectionUp;
                    }
                }
                else {
                    if (velocity.x > 0) {
                        direction = UIPanGestureRecognizerDirectionRight;
                    } else {
                        direction = UIPanGestureRecognizerDirectionLeft;
                    }
                }
            }
            break;
        }
        case UIGestureRecognizerStateChanged: {
            
            switch (direction) {
                case UIPanGestureRecognizerDirectionUp: {
                    
                    
                    if (_origional < 1) {
                        _origional += 0.04;
                        float ANGLE = (_origional * (kCameraAlgneMax - kCameraAlgneMin)) + kCameraAlgneMin;
                        NSLog(@"CAMERA ANGLE : %f",ANGLE);
                        
                        if ((kCameraAlgneMax >= ANGLE && ANGLE >= kCameraAlgneMin)) {
                            [_manager setCameraAngle:ANGLE];
                        }
                    }
                    break;
                }
                case UIPanGestureRecognizerDirectionDown: {
                    
                    if (_origional > 0) {
                        _origional -= 0.04;
                        float ANGLE = (_origional * (kCameraAlgneMax - kCameraAlgneMin)) + kCameraAlgneMin;
                        NSLog(@"CAMERA ANGLE : %f",ANGLE);
                        
                        if ((kCameraAlgneMax >= ANGLE && ANGLE >= kCameraAlgneMin)) {
                            [_manager setCameraAngle:ANGLE];
                        }
                    }
                    
                    break;
                }
                    
                default: {
                    break;
                }
            }
            break;
        }
        case UIGestureRecognizerStateEnded: {
            
            NSLog(@"end");
            // 记录结束滑动位置
            NSLog(@"%f ====  "  ,_origional);
            direction = UIPanGestureRecognizerDirectionUndefined;
            
            
            float ANGLE = (_origional * (kCameraAlgneMax - kCameraAlgneMin)) + kCameraAlgneMin;
            NSLog(@"CAMERA ANGLE : %f",ANGLE);
            
            if ((kCameraAlgneMax >= ANGLE && ANGLE >= kCameraAlgneMin)) {
                [_manager setCameraAngle:ANGLE];
            }
            break;
        }
        default:
            break;
    }
    
    
}
#pragma mark 添加子控件
- (void)setUpSubviews{
    [self addChangeFloorMenu];
    [self addChangeBackColorMenu];
    
}
- (void)addChangeFloorMenu{
    DropdownMenu * dropdownMenu = [[DropdownMenu alloc] init];
    [dropdownMenu setFrame:CGRectMake(20, UIScreen.mainScreen.bounds.size.height - 50, 100, 40)];
    dropdownMenu.delegate = self;
    self.changeFloorMenu = dropdownMenu;
    NSArray *floorinfoA = [_manager getFloorInfo:[[_manager getCurrentBuildingId]intValue ]];
    self.floorinfoArray = floorinfoA;
    NSLog(@"floorinfoA == %@  \n",floorinfoA);
    
    for (MAFloorInfo *foolInfo in floorinfoA) {
        NSLog(@"foolInfo.floor_id===  %@",foolInfo.floor_id);
    }
    NSMutableArray *arrayM = [NSMutableArray array];
    for (int i = 0; i < floorinfoA.count; i++) {
        NSString *string = [NSString stringWithFormat:@"第%zd层",i];
        [arrayM addObject:string];
    }
    [dropdownMenu setMenuTitles:arrayM rowHeight:30];
    [self.view addSubview:dropdownMenu];
    
}

- (void)addChangeBackColorMenu{
    DropdownMenu * dropdownMenu = [[DropdownMenu alloc] init];
    [dropdownMenu setFrame:CGRectMake(UIScreen.mainScreen.bounds.size.width - 150, UIScreen.mainScreen.bounds.size.height - 50, 100, 40)];
    dropdownMenu.delegate = self;
    self.changeBackColorMenu = dropdownMenu;
    NSArray *array = @[@"添加",@"移除"];
    [dropdownMenu.mainBtn setTitle:@"路径" forState:UIControlStateNormal];
    [dropdownMenu setMenuTitles:array rowHeight:30];
    [self.view addSubview:dropdownMenu];
    
}
#pragma mark 初始化

- (void)mapConfiguration{
    if (_manager == nil) {
        [self mapSetting];
        //[_manager loadZone];
    }
    
}
- (void)mapSetting{
    self.url = baseUrl;
    //self.url = @"http://192.168.0.137:8080";
    self.prjId = VBMapProjtectId;
    // VBMapProjtectId
    
    self.manager = [[MAManager alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    [self.manager getMapView].frame = CGRectMake(0, 32, self.view.frame.size.width, self.view.frame.size.height - 40 - 65);
    [self.manager setBackgroundColorWithRed:144.0 green:165.0 blue:180.0];
    
    [self.view addSubview:[_manager getMapView]];
    
    [_manager setServerUrl:_url];
    [_manager setProjectId:_prjId];
    
    [_manager debugEnable:YES];
    //[SVProgressHUD showInfoWithStatus:@"初始化成功"];
    
    _manager.delegate = self;
    NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    MAStateManager *stateManager = [MAStateManager Instance];
    stateManager.current_project_id = _prjId;
    stateManager.downloadPath = documentPath;
    
   
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [self onLoadMap_Clicked:nil];
//    });
     [self onLoadMap_Clicked:nil];
    [_manager updatePanBoundAsAABB];
    
}

- (void)onLoadMap_Clicked:(id)sender
{
    
    [_manager loadMap:[[_manager getDefaultBuildingId] intValue] floorId:[_manager getDefaultFloorId]];
    
    
    MAMapView *map = (MAMapView *)[_manager getMapView];
    
    MAVector *vector = [_manager getMapPointX:[UIScreen mainScreen].bounds.size.width * 0.5 andY:[UIScreen mainScreen].bounds.size.height * 0.5];
    //[_manager mapMoveTo:vector];
    // [map mapCenterAsPx:3000.0f Py:0.0f Pz:10000.0f];
    [map mapCenterAsPx:vector.x  Py:vector.y Pz:vector.z];
    [_manager setZoom:22000.0];
    
    [_manager showPoi:YES];
    
    
}



#pragma mark MADelegate
- (void)onReadyForUpdate{
    [_manager  needUpdate:_prjId];
    //[_manager startUpdate];
}

-(void)onNeedUpdate:(BOOL)update
{
    if (update == YES) {
        [_manager startUpdate];
    } else {
//        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Update" message:@"它是最新的文件." delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
//        [alertView show];
      
        
    }
}
// 必须有
-(void)onUpdateStateChanged:(MAUpdate*)update
{
    switch([update getState])
    {
        case MA_DOWNLOADING:
            // labelProgress.text = [NSString stringWithFormat:@"Download:%d%%", update.progress];
            break;
            
        case MA_SUCCESS:
            break;
            
            // 업데이트 실패가 호출된 후, 라이브러리에서 해당 프로젝트에 해당하는 데이터를 지우지 않고, 앱에서 해당 데이터를 삭제함.
        case MA_FAILED:
            if ([_manager isExistProjectData:_prjId]) {
                [_manager deleteProjectData:_prjId];
            }
            break;
        default:
            NSAssert(NO,@"정의되지 않은 상태.");
            break;
    }
}
#pragma mark - Map Loading

- (void)onMapLoadingSuccess
{
    
    [_manager refresh];
    return;
    
}
- (void)onNetworkFailed
{
    NSLog(@"onNetworkFailed");
}
- (void)onMapLoadingFailed
{
    //[ActivityWindow Hide];
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Loading" message:@"Map Loading Failed" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    [alertView show];
}

#pragma mark POI点击事件
- (BOOL)onPoiTouched:(int)poiId x:(int)x y:(int)y{
    
    NSLog(@"点击了poi   %zd   %zd   %zd",poiId,x,y);
    MAPoi *poi = [_manager getPoiInfoById:poiId];
    NSLog(@"poi.desc=== %@",poi.desc);
    
    return NO;
}

- (BOOL)onBalloonTouched:(int)poiId{
    NSArray *poilist = [_manager getPoiInfoList];
    //NSLog(@"poilist=== %@",poilist);
    for (MAPoi *poi in poilist) {
        if (poi.poi_id == poiId) {
            self.selectedPoi = poi;
            break;
        }
    }
    [_manager mapMoveOnPoiInfo:self.selectedPoi];
    return NO;
}

#pragma mark DropdownMenu Delegate  选择楼层
- (void)dropdownMenu:(DropdownMenu *)menu selectedCellNumber:(NSInteger)number{
    NSLog(@"你选择了：%ld",number);
    [_manager setPoiTextSize:20];
    [_manager setPoiTextColor:[UIColor redColor]];
    
    if (menu == self.changeBackColorMenu) {
        NSLog(@"是否显示路径");
        if (0 == number) {//  显示路径
          
            MAFloorInfo *floorInfo = self.floorinfoArray[4];
        
            NSArray *poiA = [_manager getPoiInfoListByFloorId:floorInfo.floor_id];
            for (MAPoi *poi in poiA) {
               // NSLog(@"poi.category_id== %zd",poi.category_id);
                
            }
            
            NSMutableArray *arrayCategory = [[NSMutableArray alloc] initWithCapacity:10];

            [arrayCategory addObject:[NSNumber numberWithInt:17]];
            [arrayCategory addObject:[NSNumber numberWithInt:18]];
          
           // [_manager showPoi:YES];
            //[_manager setShowPoiType:arrayCategory];
            
            
           
            MAPoi *poi0 = [_manager getPoiInfoById:562];
            MAPoi *poi3 = [_manager getPoiInfoById:565];
            MAPoi *poi2 = [_manager getPoiInfoById:563];
//            MAPoi *poi0 = poiA[0];
//            MAPoi *poi3 = poiA[3];
            
            MAVector *begin = [[MAVector alloc] init];
            begin.x = [poi0.x floatValue];
            begin.y = [poi0.y floatValue];
            begin.z = [poi0.z floatValue];
            
            MAVector *middle = [[MAVector alloc] init];
            middle.x = [poi2.x floatValue];
            middle.y = [poi2.y floatValue];
            middle.z = [poi2.z floatValue];
            
            
            MAVector *end = [[MAVector alloc] init];
            end.x = [poi3.x floatValue];
            end.y = [poi3.y floatValue];
            end.z = [poi3.z floatValue];
            NSMutableArray *array = [NSMutableArray arrayWithObjects:begin,middle,end, nil];
 

            //[_manager addPath:array withColor:[UIColor redColor]];
            [self findPathDeparture:poi0 toArrival:poi3 via:nil rest:NO];
            [_manager showPath:YES];
            


           
 
            
        }else if (1 == number){ // 移除路径
            [_manager removePath];
            
        }
    }else{
        NSLog(@"改变楼层");
        
        [_manager showPoi:YES];
        [_manager loadMapAll:[[_manager getDefaultBuildingId]intValue]];
        MAFloorInfo *floorInfo = self.floorinfoArray[number];
        [_manager changeFloorById:floorInfo.floor_id];
        NSArray *categoryArray = [_manager getPoiCategory];
        NSLog(@"categoryArray==  %@  %@",categoryArray,self.floorinfoArray);
        
        NSArray *poiA = [_manager getPoiInfoListByFloorId:floorInfo.floor_id];
        
       
      NSString *currentFloorID =  [_manager getCurrentFloorId];
        
        NSLog(@"poiA===  %@ currentFloorID=== %@   floorInfo.floor_id== %@",poiA,currentFloorID,floorInfo.floor_id);
        
    }
    
    
}

/* shine begin 위치의 딜리게이트 */
-(void)didUpdateSLLocation:(MAVector *)location altitude:(float)altitude withUncertainty:(double)radius{
    
    
    _myPos.x = location.x;
    _myPos.y = 0;
    _myPos.z = location.y;
    
    
   
    float thisAltitude;
    
    if (altitude < kGround) {
        thisAltitude = kGround;
    }else{
        thisAltitude = altitude;
    }
    
    [_manager mapCenterAs:_myPos];
    //    [_ma showMyPos:YES x:_myPos.x y:_myPos.y z:_myPos.z red:0.0f green:0.7f blue:1.0f alpha:0.5f rad:(radius * 100.0f)];
    

    [self showMyPosition:YES];
    
  
}

// shine begin 현재 내 위치를 표시한다.
- (void) showMyPosition:(BOOL)show
{
    
    [_manager showDynamicPoi:show x:_myPos.x y:_myPos.y z:_myPos.z];
}

- (float)getTotalDistance:(NSArray *)nodeList
{
    NSArray *edgeList = [_pathFinder getEdgeListFromNodeList:nodeList];
    
    return [_pathFinder getTotalDistance:edgeList];
    
}
- (void) findPathDeparture:(MAPoi *)departure toArrival:(MAPoi *)arrival via:(MAPoi *)via rest:(BOOL)rest
{

    
    [_manager removePath];
    

    int kGround = 700;
    NSMutableArray *pois = [[NSMutableArray alloc] initWithCapacity:10];

    [pois addObject:departure];
    if (via) {
        [pois addObject:via];
    }
    [pois addObject:arrival];
    

    [_manager getGmlPath];
    
    NSLog(@"[_manager getGmlPath]=== %@",[_manager getGmlPath]);
    _pathFinder = [[VBPathFinder alloc] initWithFile:[_manager getGmlPath] withType:eGml];
 
    NSArray *arrayNode =  [_pathFinder getPathListWithPoiIds:pois withLastPosition:nil option:(rest == NO) ? 4 : 0];
    float totalDistance = [self getTotalDistance:arrayNode];

    NSLog(@"距离为totalDistance===   %f",totalDistance);
    
    if (arrayNode.count != 0) {
        NSMutableArray *array = [[NSMutableArray alloc] initWithCapacity:2];
        [array removeAllObjects];
        
        node = new vbPathNode[arrayNode.count];
        
        int count = 0;
        int totalCount = 0;
        int floorBefore = -1;
        UIColor *pathColor = [UIColor greenColor];
        
        for (PathNode *pn in arrayNode) {
            MAVector *v = [[MAVector alloc] init];
            
            if (array.count == 0) {
                v.x = pn.x;
                v.y = pn.level + kGround; // gndOffSet
                v.z = pn.y * -1.0;
                floorBefore = pn.floor;
                [array addObject:v];
            } else {
                if (pn.level + kGround == ((MAVector *)array[count-1]).y) {
                    v.x = pn.x;
                    v.y = pn.level + kGround;
                    v.z = pn.y * -1.0;
                    floorBefore = pn.floor;
                    [array addObject:v];
                } else {
                    //#A.
                    if (rest) {
                        pathColor = [UIColor redColor];
                    }
                    [_manager addPath:array withColor:pathColor withFloorId:floorBefore];
                    
                    [array removeAllObjects];
                    
                    v.x = pn.x;
                    v.y = pn.level + kGround; // gndOffSet
                    v.z = pn.y * -1.0;
                    floorBefore = pn.floor;
                    [array addObject:v];
                    count = 0;
                }
            }
            node[count].pos = vec3(v.x, pn.floor, v.z);
            node[count].floor = pn.floor;
            count++;
            totalCount++;
        }
        
        
        if  (count > 0) {
            // 如果有一个可以绘制路径的拓扑，那么它将会绘制路径。
          
            pathColor = [UIColor redColor];
        
            
            MAVector *vBegin = [[MAVector alloc] init];
            MAPoi *tmp = [pois objectAtIndex:0];
            vBegin.x = [tmp.x floatValue];
            vBegin.y = [tmp.y floatValue];
            vBegin.z = [tmp.z floatValue];
            [array insertObject:vBegin atIndex:0];
            
            
            MAVector *vEnd = [[MAVector alloc] init];
            tmp = [pois lastObject];
            vEnd.x = [tmp.x floatValue];
            vEnd.y = [tmp.y floatValue];
            vEnd.z = [tmp.z floatValue];
            [array addObject:vEnd];
            
            [_manager addPath:array withColor:pathColor withFloorId:floorBefore];
            
            [_manager showPath:YES];
            
//            MAVector *movigPosToCenter = [MAVector new];
//
//            MAVector *calculateBeginPos = [array objectAtIndex:0];
//            MAVector *calculateEndPos = [array lastObject];
//
//            movigPosToCenter.x = (calculateBeginPos.x + calculateEndPos.x) / 2.0f;
//            movigPosToCenter.y = (calculateBeginPos.y + calculateEndPos.y) / 2.0f;
//            movigPosToCenter.z = (calculateBeginPos.z + calculateEndPos.z) / 2.0f;
//
//  完成路线后，您将移动到起点和终点之间的中点。

        } else {
            
        }
    }else{
//        [self showFailToast];
//        if ([self.delegate respondsToSelector:@selector(failFindPathForNavi)]) {
//            [self.delegate failFindPathForNavi];
//        }
        
    }
    
//    if ([self.delegate respondsToSelector:@selector(getTotalDistance:)]) {
//        [self.delegate getTotalDistance:totalDistance / 10];
//    }
//    pathFindMode = YES;
}

@end




