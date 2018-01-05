//
//  TwoViewController.m
//  PushOrientation
//
//  Created by BIM on 2018/1/5.
//  Copyright © 2018年 BIM. All rights reserved.
//

#import "TwoViewController.h"
#import "ThreeViewController.h"
@interface TwoViewController ()

@end

@implementation TwoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor redColor];
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = CGRectMake(200, 200, 200, 200);
    
    [btn setTitle:@"点击跳转第三个" forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(pushToThree) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btn];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)pushToThree{
    ThreeViewController *three = [[ThreeViewController alloc] init];
    [self.navigationController pushViewController:three animated:YES];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
