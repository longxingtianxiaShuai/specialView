//
//  ViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/16.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "ViewController.h"
#import "SdkInitViewController.h"
#import "UpdateViewController.h"
#import "XWSBMViewController.h"
#import "PlayViewController.h"
#import "PoiListViewController.h"
#import "NavListViewController.h"
#import "GetMethodViewController.h"

@interface ViewController ()
@property (nonatomic,strong)NSArray *dataSourcesbm;
@end

static NSString *cellID = @"cellID";
@implementation ViewController
#pragma mark Life cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

  
    self.dataSource = self.dataSourcesbm;
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = @"概览";
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    switch (indexPath.row) {
        case 0:
        {
            SdkInitViewController *sdkInitVc = [[SdkInitViewController alloc] init];
            [self.navigationController  pushViewController:sdkInitVc animated:false];
            
            
        }
            break;
            
        case 1:
        {
            UpdateViewController *updateVc = [[UpdateViewController alloc] init];
            [self.navigationController  pushViewController:updateVc animated:false];
            
            
        }
            break;
        case 2:
        {
            XWSBMViewController *sbmVc = [[XWSBMViewController alloc] init];
            [self.navigationController  pushViewController:sbmVc animated:false];
            
            
        }
            break;
            
        case 3:
        {
            PlayViewController *playVc = [[PlayViewController alloc] init];
            [self.navigationController  pushViewController:playVc animated:false];
            
            
        }
            break;
        case 4:
        {
            PoiListViewController *poiListVc = [[PoiListViewController alloc] init];
            [self.navigationController  pushViewController:poiListVc animated:false];
            
            
        }
            break;
            // NavListViewController
        case 5:
        {
            NavListViewController *navListVc = [[NavListViewController alloc] init];
            [self.navigationController  pushViewController:navListVc animated:false];
            
            
        }
            break;
        case 6:
        {
            GetMethodViewController *getMethodVc = [[GetMethodViewController alloc] init];
            [self.navigationController  pushViewController:getMethodVc animated:false];
            
            
        }
            break;
            
        default:
            break;
    }
}
#pragma mark lazy
- (NSArray *)dataSourcesbm{
    
    if (!_dataSourcesbm) {
        _dataSourcesbm = @[@"初始化",
                           @"更新",
                           @"XWSBM",
                           @"播放",
                           @"兴趣点",
                           @"导航",
                           @"Get方法"
                           
                           ];;
    }
    return _dataSourcesbm;
}



@end
