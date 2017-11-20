//
//  LoadSBMSViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/16.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "LoadSBMSViewController.h"
#import "MapViewController.h"
#import "MAManager.h"


@interface LoadSBMSViewController ()
@property (nonatomic,strong)MapViewController *mapVc;

@end

@implementation LoadSBMSViewController

#pragma mark life cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.navigationItem.title = @"加载文件显示默认楼层";
    self.mapVc = [[MapViewController alloc] init];
    [self.view addSubview:self.mapVc.view];
    [self addChildViewController:self.mapVc];
   
    
}


@end
