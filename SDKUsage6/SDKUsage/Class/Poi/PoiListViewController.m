//
//  PoiListViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/17.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "PoiListViewController.h"
#import "POIShowHideViewController.h"
#import "POIShowHideCategoryViewController.h"
#import "PoiClickEventViewController.h"

@interface PoiListViewController ()
@property (nonatomic,strong)NSArray *dataSourcesbm;
@end

@implementation PoiListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.dataSource = self.dataSourcesbm;
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = @"兴趣点";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    switch (indexPath.row) {
        case 0:
        {
            POIShowHideViewController *poiShowHideVc = [[POIShowHideViewController alloc] init];
            [self.navigationController pushViewController:poiShowHideVc animated:false];
        }
            break;
        case 1:
        {
            POIShowHideCategoryViewController *poiShowHideCategoryVc = [[POIShowHideCategoryViewController alloc] init];
            [self.navigationController pushViewController:poiShowHideCategoryVc animated:false];
        }
            break;
        case 2:
        {
            PoiClickEventViewController *poiClickVc = [[PoiClickEventViewController alloc] init];
            [self.navigationController pushViewController:poiClickVc animated:false];
        }
            break;
       
            
        default:
            break;
    }
}



#pragma mark lazy
- (NSArray *)dataSourcesbm{
    
    if (!_dataSourcesbm) {
        _dataSourcesbm = @[@"设置POI显示/隐藏",
                           
                           @"POI按分类显示隐藏",
                           @"POI的点击事件"
                           
                           ];
    }
    return _dataSourcesbm;
}
@end
