//
//  BaseTableViewController.m
//  SDKUsage
//
//  Created by BIM on 2017/10/17.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "BaseTableViewController.h"

@interface BaseTableViewController ()

@property (nonatomic,strong)UITableView *tableView;

@end
static NSString *cellID = @"cellID";
@implementation BaseTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setUpSubviews];
    
    [self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:cellID];
    self.tableView.tableFooterView = [[UIView alloc] init];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark 添加子控件
- (void)setUpSubviews{
    [self addSubViews];
    [self setUpConstraints];
    
}

- (void)addSubViews{
    self.tableView = [[UITableView alloc] init];
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    [self.view addSubview:self.tableView];
    
}
- (void)setUpConstraints{
    [self.tableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.view);
    }];
    
}

#pragma mark UITableViewDataSource


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID forIndexPath:indexPath];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:cellID];
    }
    cell.textLabel.text = self.dataSource[indexPath.row];
    return cell;
}

- (NSInteger)tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dataSource.count;
}

@end
