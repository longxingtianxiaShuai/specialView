//
//  ViewController.m
//  Orientation
//
//  Created by BIM on 2018/1/5.
//  Copyright © 2018年 BIM. All rights reserved.
//

#import "ViewController.h"
#import "TwoViewController.h"
#import "AppDelegate.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.navigationItem.title = @"第一个控制器";
}
- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    

//    NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
//    [[UIDevice currentDevice]setValue:value forKey:@"orientation"];
}
- (void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
//    AppDelegate *appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
//    appdelegate.allowRotation = 0;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotate
{
    
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
   // return [self.viewControllers.lastObjectsupportedInterfaceOrientations];
    return UIInterfaceOrientationMaskLandscapeRight;
}
- (IBAction)push:(id)sender {
    TwoViewController *two = [[TwoViewController alloc] init];
   // [self.navigationController pushViewController:two animated:YES];
    [self presentViewController:two animated:YES completion:nil];
}


@end
