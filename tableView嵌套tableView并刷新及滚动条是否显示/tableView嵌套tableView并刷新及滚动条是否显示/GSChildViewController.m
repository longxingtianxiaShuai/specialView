//
//  GSChildViewController.m
//  tableView嵌套tableView并刷新及滚动条是否显示
//
//  Created by BIM on 2017/11/10.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "GSChildViewController.h"

#import <SVPullToRefresh.h>

/**
 * 随机数据
 */
#define RandomData [NSString stringWithFormat:@"随机数据---%d", arc4random_uniform(1000000)]

@interface GSChildViewController ()<UITableViewDelegate,UITableViewDataSource>

//@property (nonatomic,strong)UITableView *tableView;
/** 用来显示的假数据 */
@property (strong, nonatomic) NSMutableArray *data;
@end

@implementation GSChildViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setUpSubviews];
    __weak typeof(self)weakSelf = self;
    self.automaticallyAdjustsScrollViewInsets = YES;
    [self.tableView addInfiniteScrollingWithActionHandler:^{
        [weakSelf insertRowAtBottom];
    }];
}

- (void)insertRowAtTop
{
    for (int i = 0; i<5; i++) {
        [self.data insertObject:RandomData atIndex:0];
    }
    __weak UITableView *tableView = self.tableView;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [tableView reloadData];
       
    });
}

- (void)insertRowAtBottom
{
    for (int i = 0; i<5; i++) {
        [self.data addObject:RandomData];
    }
    __weak GSNewTableView *tableView = self.tableView;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [tableView reloadData];
        [tableView.infiniteScrollingView stopAnimating];
    });
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setUpSubviews{
    GSNewTableView *tableView = [[GSNewTableView alloc] init];
    tableView.delegate = self;
    tableView.dataSource = self;
    self.tableView = tableView;
    tableView.frame = self.view.bounds;
    [self.view addSubview:tableView];
    self.tableView.contentInset = UIEdgeInsetsMake(0, 0, 64, 0);
}


- (void)setIsRefresh:(BOOL)isRefresh{
    _isRefresh = isRefresh;
    [self insertRowAtTop];
}
#pragma mark UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.data.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *cellID = @"cellID";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        
    }
   // cell.textLabel.text = @"第二个控制器";
     cell.textLabel.text = [NSString stringWithFormat:@"第二个控制器--%@",self.data[indexPath.row]];
    cell.backgroundColor = [UIColor redColor];
    return cell;
}
#pragma mark UITableViewDelegate
- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    NSLog(@"scrollView.contentOffset.y===   %f",scrollView.contentOffset.y);
    // 只有控制这个tableview的滑动才可以 实现下拉刷新 所以设置scrollView.contentOffset = CGPointZero
    // 向下拖动  scrollView.contentOffset.y 值小于零
    //  vcCanScroll 从上一个控制器传递过来


    
    if (!_vcCanScroll) {
       scrollView.contentOffset = CGPointZero;
        
    }
    if (scrollView.contentOffset.y <= 0) {
        _vcCanScroll = NO;
        // 到顶通知父视图改变状态  用于有多个同级的控制器  用来设置所有控制器中的tableview 偏移量设置为 CGPointZero  还有就是设置父控制器中tableview的  canScroll;
         scrollView.contentOffset = CGPointZero;
        [[NSNotificationCenter defaultCenter] postNotificationName:@"leaveTop" object:nil];
        
    }
//    else{
//        _vcCanScroll = YES;
//    }
    // 设置是否显示垂直滚动条 VerticalScrollIndicator
    self.tableView.showsVerticalScrollIndicator =_vcCanScroll?YES:NO;
    
}

- (void)setVcCanScroll:(BOOL)vcCanScroll{
    _vcCanScroll = vcCanScroll;
//    if (!vcCanScroll) {
//        
//    }
}
//- (void)setCellCanScroll:(BOOL)cellCanScroll{
//
//    _vcCanScroll = cellCanScroll;
//    //self.cellCanScroll = cellCanScroll;
//    if (!cellCanScroll) {
//        self.tableView.contentOffset = CGPointZero;
//    }
//
//}
#pragma mark lazy
- (NSMutableArray *)data
{
    if (!_data) {
        self.data = [NSMutableArray array];
        for (int i = 0; i<5; i++) {
            [self.data addObject:RandomData];
        }
    }
    return _data;
}
- (void)dealloc{
    
    NSLog(@"子控制器销毁了");
}
@end
