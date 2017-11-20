//
//  NavListViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/17.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "NavListViewController.h"
#import "FinderPathViewController.h"
#import "RemovePathViewController.h"
#import "GuidedSimulationViewController.h"
#import "MyLocationViewController.h"


@interface NavListViewController ()
@property (nonatomic,strong)NSArray *dataSourcesbm;
@end

@implementation NavListViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.dataSource = self.dataSourcesbm;
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = @"导航";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    switch (indexPath.row) {
        case 0:
        {
            FinderPathViewController *finderVc = [[FinderPathViewController alloc] init];
            [self.navigationController pushViewController:finderVc animated:false];
        }
            break;
        case 1:
        {
            RemovePathViewController *removePathVc = [[RemovePathViewController alloc] init];
            [self.navigationController pushViewController:removePathVc animated:false];
        }
            break;
        case 2:
        {
            GuidedSimulationViewController *guidePathVc = [[GuidedSimulationViewController alloc] init];
            [self.navigationController pushViewController:guidePathVc animated:false];
        }
            break;
        case 3:
        {
            MyLocationViewController *mylocationVc = [[MyLocationViewController alloc] init];
            [self.navigationController pushViewController:mylocationVc animated:false];
        }
            break;
        
            
        default:
            break;
    }
}

#pragma mark lazy
- (NSArray *)dataSourcesbm{
    
    if (!_dataSourcesbm) {
        _dataSourcesbm = @[@"寻找路径模式",
                           @"移除路径",
                           @"模拟导航",
                          // @"显示/隐藏和更新我的位置"
                           
                           ];
    }
    return _dataSourcesbm;
}

@end
