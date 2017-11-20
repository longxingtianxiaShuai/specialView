//
//  XWSBMViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/16.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "XWSBMViewController.h"
#import "MapViewShowModelViewController.h"
#import "LoadSBMSViewController.h"
#import "ChangeSBMViewController.h"
#import "SetBackgColorViewController.h"
#import "TextureToggleViewController.h"
#import "Change2DTo3DViewController.h"
#import "SBMAlphaViewController.h"
#import "ShowAllSBMViewController.h"
#import "InterLayerSpacingSBMViewController.h"
#import "FloorTransparencyViewController.h"

@interface XWSBMViewController ()
@property (nonatomic,strong)NSArray *dataSourcesbm;

@end

@implementation XWSBMViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.dataSource = self.dataSourcesbm;
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = @"XWSBM";

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    switch (indexPath.row) {
        case 0:
        {
            MapViewShowModelViewController *mapViewShowVc = [[MapViewShowModelViewController alloc] init];
            [self.navigationController pushViewController:mapViewShowVc animated:false];
            
        }
            break;
        case 1:
        {
            LoadSBMSViewController *loadSbmVc = [[LoadSBMSViewController alloc] init];
            [self.navigationController pushViewController:loadSbmVc animated:false];
            
        }
            
            break;
        case 2:
        {
            ChangeSBMViewController *changeSbmVc = [[ChangeSBMViewController alloc] init];
            [self.navigationController pushViewController:changeSbmVc animated:false];
            
        }
            
            break;
        case 3:
        {
            SetBackgColorViewController *setBcVc = [[SetBackgColorViewController alloc] init];
            [self.navigationController pushViewController:setBcVc animated:false];
            
        }
            
            break;
        case 4:
        {
            TextureToggleViewController *textureVc = [[TextureToggleViewController alloc] init];
            [self.navigationController pushViewController:textureVc animated:false];
        }
            
            break;
        case 5:
        {
            Change2DTo3DViewController *change2to3Vc = [[Change2DTo3DViewController alloc] init];
            [self.navigationController pushViewController:change2to3Vc animated:false];
        }
            
            break;
        case 6:
        {
            ShowAllSBMViewController *showAllVc = [[ShowAllSBMViewController alloc] init];
            [self.navigationController pushViewController:showAllVc animated:false];
            
            
        }
            
            break;
        case 7:
        {
            SBMAlphaViewController *sbmAVc = [[SBMAlphaViewController alloc] init];
            [self.navigationController pushViewController:sbmAVc animated:false];
            
            
        }
            
            break;
        case 8:
        {
            InterLayerSpacingSBMViewController *interLayerSpaceVc = [[InterLayerSpacingSBMViewController alloc] init];
            [self.navigationController pushViewController:interLayerSpaceVc animated:false];
            
        }
            
            break;
        case 9:
        {
            FloorTransparencyViewController *floorTranVc = [[FloorTransparencyViewController alloc] init];
            [self.navigationController pushViewController:floorTranVc animated:false];
            
        }
            
            break;
            
        default:
            break;
    }
}
#pragma mark lazy
- (NSArray *)dataSourcesbm{
    
    if (!_dataSourcesbm) {
        _dataSourcesbm = @[@"返回视图显示模式",
                        @"加载文件和显示默认楼层",
                        @"选择其他楼层",
                        @"设置视图的背景颜色",
                        @"纹理开关",
                           @"2D3D切换",
                           @"显示所有SBM",
//                        @"设置透明度(暂无接口)",
//                       
//                        @"层间距SBM(暂无接口)",
//                        @"楼层的透明度(暂无接口)"
                        
                        ];
    }
    return _dataSourcesbm;
}
@end
