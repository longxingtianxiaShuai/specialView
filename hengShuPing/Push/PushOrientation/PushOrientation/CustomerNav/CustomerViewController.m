//
//  CustomerViewController.m
//  PushOrientation
//
//  Created by BIM on 2018/1/5.
//  Copyright © 2018年 BIM. All rights reserved.
//

#import "CustomerViewController.h"

@interface CustomerViewController ()

@end

@implementation CustomerViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
//- (BOOL)shouldAutorotate{
//    return NO;
//}
//- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
//    return UIInterfaceOrientationMaskPortrait;
//}
//- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation{
//    return (toInterfaceOrientation == UIInterfaceOrientationPortrait);
//
//}


-(BOOL)shouldAutorotate{
        return [self.topViewController shouldAutorotate];
}

//支持的方向
 - (UIInterfaceOrientationMask)supportedInterfaceOrientations {
         return [self.topViewController supportedInterfaceOrientations];
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
