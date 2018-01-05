//
//  BaseNavViewController.m
//  Orientation
//
//  Created by BIM on 2018/1/5.
//  Copyright © 2018年 BIM. All rights reserved.
//

#import "BaseNavViewController.h"

@interface BaseNavViewController ()

@end

@implementation BaseNavViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (BOOL)shouldAutorotate{
   return [self.viewControllers.lastObject shouldAutorotate];
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
     return [self.viewControllers.lastObject supportedInterfaceOrientations];
     
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
