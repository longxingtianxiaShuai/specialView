//
//  PlayViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/17.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "PlayViewController.h"
#import "CaneraModeViewController.h"
#import "MoveCaneraViewController.h"
#import "HRotationAnimationViewController.h"


@interface PlayViewController ()
@property (nonatomic,strong)NSArray *dataSourcesbm;
@end

@implementation PlayViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.dataSource = self.dataSourcesbm;
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = @"播放";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    switch (indexPath.row) {
        case 0:
        {
            CaneraModeViewController *cameraModeVc = [[CaneraModeViewController alloc] init];
            [self.navigationController pushViewController:cameraModeVc animated:false];
        }
            break;
        case 1:
        {
            MoveCaneraViewController *moveCameraVc = [[MoveCaneraViewController alloc] init];
            [self.navigationController pushViewController:moveCameraVc animated:false];
        }
            break;
        case 2:
        {
            
            HRotationAnimationViewController *hrotationVc = [[HRotationAnimationViewController alloc] init];
            [self.navigationController pushViewController:hrotationVc animated:false];
        }
            break;
            
            
        default:
            break;
    }
}


#pragma mark lazy
- (NSArray *)dataSourcesbm{
    
    if (!_dataSourcesbm) {
        _dataSourcesbm = @[@"设置相机模式",
                          
                           @"将摄像机标准点移动到输入的三维坐标",
                           @"水平旋转动画"
                           
                           ];
    }
    return _dataSourcesbm;
}
@end
