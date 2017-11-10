//
//  GSChildViewController.h
//  tableView嵌套tableView并刷新及滚动条是否显示
//
//  Created by BIM on 2017/11/10.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GSChildViewController : UIViewController
@property (nonatomic, assign) BOOL vcCanScroll;
@property (nonatomic, assign) BOOL isRefresh;
@property (nonatomic,strong)UITableView *tableView;

@property (nonatomic,assign)BOOL cellCanScroll;
@end
