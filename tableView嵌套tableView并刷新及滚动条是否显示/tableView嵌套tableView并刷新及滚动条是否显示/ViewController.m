//
//  ViewController.m
//  tableView嵌套tableView并刷新及滚动条是否显示
//
//  Created by BIM on 2017/11/9.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "ViewController.h"
#import "GSParentViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)pushToGSTVc:(id)sender {
    GSParentViewController *par = [[GSParentViewController alloc] init];
    [self.navigationController pushViewController:par animated:YES];
}



@end
