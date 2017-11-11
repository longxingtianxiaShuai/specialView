//
//  GSParentViewController.m
//  tableView嵌套tableView并刷新及滚动条是否显示
//
//  Created by BIM on 2017/11/10.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "GSParentViewController.h"
#import "GSChildViewController.h"
#import <SVPullToRefresh.h>
#import "GSTableview.h"
@interface GSParentViewController ()<UITableViewDelegate,UITableViewDataSource>

@property (nonatomic,strong)GSTableview *gsTableView;
@property (nonatomic,strong)GSChildViewController *childViewController;
@property (nonatomic,assign)BOOL canScroll;

@end

@implementation GSParentViewController

#pragma mark life cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"tableView嵌套tableView手势Demo";
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(changeSrollStatus) name:@"leaveTop" object:nil];
    [self setUpSubviews];
    self.canScroll = YES;
    __weak typeof(self) weakSelf = self;
   // self.gsTableView.contentInsetAdjustmentBehavior = YES;
    self.automaticallyAdjustsScrollViewInsets = YES;
    [self.gsTableView addPullToRefreshWithActionHandler:^{
        [weakSelf insertRowAtTop];
    }];
    //self.gsTableView.scrollEnabled = NO;
}

- (void)insertRowAtTop
{
   // NSArray *sortTitles = @[@"全部",@"服饰穿搭",@"生活百货",@"美食吃货",@"美容护理",@"母婴儿童",@"数码家电"];
   
   // self.isRefresh = YES;
    self.childViewController.isRefresh = YES;
    [self.gsTableView.pullToRefreshView stopAnimating];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)setUpSubviews{
    GSTableview *tableView = [[GSTableview alloc] init];
    self.gsTableView = tableView;
    tableView.delegate = self;
    tableView.dataSource = self;
    tableView.frame = CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
    [self.view addSubview:tableView];
    
}
#pragma mark UITableViewDelegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.section == 0) {
        if (indexPath.row == 0) {
            return 200;
        }
        return 50;
    }
    return CGRectGetHeight(self.view.bounds);
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    CGFloat bottomCellOffset = [_gsTableView rectForSection:1].origin.y - 64;
    if (scrollView.contentOffset.y >= bottomCellOffset) {
        scrollView.contentOffset = CGPointMake(0, bottomCellOffset);
        
        if (self.canScroll) {
            self.canScroll = NO;
            self.childViewController.vcCanScroll = YES;
          
        }
    }else{
        if (!self.canScroll) {
            scrollView.contentOffset = CGPointMake(0, bottomCellOffset);
        }
    }
    self.gsTableView.showsVerticalScrollIndicator = _canScroll?YES:NO;
}

// 设置headview
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 44;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    
    UILabel *label = [[UILabel alloc] init];
    label.text = @"头部view";
    label.backgroundColor = [UIColor blueColor];
    return label;
}
#pragma mark UITableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    if (section == 1) {
        return 1;
    }
    return 2;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *firstRow = @"firstRow";
    static NSString *secondRow = @"secondRow";
    
    if (indexPath.section == 0) {
        if (indexPath.row == 0) {
            UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:firstRow];
            if (!cell) {
                cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:firstRow];
                cell.backgroundColor = [UIColor grayColor];
                cell.textLabel.text = @"第一组第一个";
            }
            return cell;
        }else{
            if (indexPath.row == 1) {
                UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:secondRow];
                if (!cell) {
                    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:secondRow];
                    cell.textLabel.text = @"第一组第二个";
                }
                return cell;
            }
        }
    }
    static NSString *secondSectionCell = @"secondSectionCell";
    if (indexPath.section == 1) {
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:secondSectionCell];
        if (!cell) {
            cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:secondSectionCell];
            GSChildViewController *childVc = [[GSChildViewController alloc] init];
            [cell.contentView addSubview:childVc.view];
            self.childViewController = childVc;
            [self addChildViewController:childVc];
        }
        return cell;
    }
    return nil;
}


#pragma mark action
- (void)changeSrollStatus{
    self.canScroll = YES;
    self.childViewController.vcCanScroll = NO;
    // self.contentCell.cellCanScroll = NO;  主要用于有多个tableview 都设置偏移量为 cgpointzero
    // 底部
   // CGFloat bottomCellOffset = [_gsTableView rectForSection:1].origin.y - 64;
    
}

@end
