//
//  TwoViewController.m
//  Orientation
//
//  Created by BIM on 2018/1/5.
//  Copyright © 2018年 BIM. All rights reserved.
//

#import "TwoViewController.h"

@interface TwoViewController ()

@end

@implementation TwoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor redColor];
    UIView *view_top = [[UIView alloc]initWithFrame:CGRectMake(0, 0,self.view.bounds.size.height+20, 64)];
    [view_top setBackgroundColor:[UIColor greenColor]];
    [self.view addSubview:view_top];
    {
        UIButton *bt = [[UIButton alloc]initWithFrame:CGRectMake(80, 80, 80, 80)];
        [bt setBackgroundColor:[UIColor yellowColor]];
        [bt setTitle:@"点我返回" forState:UIControlStateNormal];
        [bt addTarget:self action:@selector(pushaction) forControlEvents:UIControlEventTouchUpInside];;
        [self.view addSubview:bt];
    }
}

-(void)pushaction{
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotate{
    return YES;
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskPortrait;
}
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation{
    return UIInterfaceOrientationPortrait;
}
@end
