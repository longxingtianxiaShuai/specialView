//
//  ViewController.m
//  ThreeLeave2
//
//  Created by BIM on 2017/11/3.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "ViewController.h"
#import "DemoCell.h"
#import "DemoModel.h"

@interface ViewController ()<UITableViewDelegate,UITableViewDataSource>
@property (nonatomic,strong)UITableView *myTableView;
@property (nonatomic,strong)NSMutableArray *attrArr;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [self setUpSubViews];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark lazy
- (NSMutableArray *)attrArr{
    if (!_attrArr) {
        _attrArr = [NSMutableArray array];
        NSArray *first_indexs = @[@1,@2];
        NSArray *second_indexs = @[@0,@1];
        NSArray *third_indexs = @[@0,@1,@2];
        for (int i = 0 ; i < first_indexs.count; i++) {
            for (int j = 0; j < second_indexs.count; j++) {
                for (int k = 0; k < third_indexs.count; k++) {
                    DemoModel *model = [[DemoModel alloc] init];
                
                    model.first_index = [first_indexs[i] integerValue];
                    if (j == 0) {
                        
                        if (k == 0) {
                            model.second_index = 0;
                            model.three_index = 0;
                            model.is_open = NO;
                            model.is_show = YES;
                            [_attrArr addObject:model];
                        }
                       continue;
                    }
                    model.second_index = [second_indexs[j] integerValue];
                    model.three_index = [third_indexs[k] integerValue];
                    model.is_show = NO;
                    model.is_open = NO;
                    [_attrArr addObject:model];
                    
                    
                }
            }
        }
    }
    
    return _attrArr;
}

#pragma mark UITableViewDelegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    DemoModel *model = self.attrArr[indexPath.row];
    
    if (model.is_show) {
        return 44;
    }else{
        
        return 0;
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    [self clickCellAtIndexPath:indexPath];
}
#pragma mark UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.attrArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *cellID = @"cellID";
    DemoCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (!cell) {
        cell = [[DemoCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        
    }
    [cell updateCellWithModel:self.attrArr[indexPath.row]];
    return cell;
}
#pragma mark action
- (void)clickCellAtIndexPath:(NSIndexPath *)indexPath{
    DemoModel *currentModel = self.attrArr[indexPath.row];
    // 如果是第一级cell
    if (currentModel.second_index == 0 && currentModel.three_index == 0) {
        if (currentModel.is_open) {
            for (DemoModel *model  in _attrArr) {
                // 是否是当前分组
                BOOL is_belong = model.first_index == currentModel.first_index;
                
                // 是否是第一级cell
                BOOL is_current = model.second_index == 0;
                
                if (is_belong && !is_current) {
                    model.is_show = NO;
                    model.is_open = NO;
                }else{
                    // 其他分组不做处理
                }
            }
            currentModel.is_open = NO;
            [self.myTableView reloadData];
            return;
        }else{ // 未展开
            for (DemoModel *model in _attrArr) {
                // 是否是当前分组
                BOOL is_belong = model.first_index == currentModel.first_index;
                
                // 是否是第一级cell
                BOOL is_current = model.second_index == 0;
                if (is_belong && !is_current) {
                    if (model.three_index == 0) {
                        model.is_show = YES;
                    }else{
                        // 三级cell 不操作
                    }
                }else{
                    
                    // 其他一级cell不操作
                }
            }
            currentModel.is_open = YES;
            [_myTableView reloadData];
            return;
            
            
        }
        
    }
    
    // 如果是第二级cell
    if (currentModel.three_index == 0) {
        if (currentModel.is_open) {
            for (DemoModel *model in _attrArr) {
                // 是否在当前分组下
                BOOL is_belong = model.first_index == currentModel.first_index&& model.second_index == currentModel.second_index;
                // 是否是二级cell
                BOOL is_current = model.three_index == 0;
                if (is_belong && !is_current) {
                    model.is_show = NO;
                }else{
                    // 不做处理
                }
            }
            currentModel.is_open = NO;
            [_myTableView reloadData];
            return;

        }else{ // 如果第二组是关闭的
            for (DemoModel *model in _attrArr) {
                // 是否在当前分组下
                BOOL is_belong = model.second_index == currentModel.second_index && model.first_index == currentModel.first_index;
                // 是否是二级分组
                BOOL is_current = model.three_index == 0;
                if (is_belong && !is_current) {
                    model.is_show = YES;
                }else{
                    
                    // 其他分组不做处理
                }
            }
            currentModel.is_open = YES;
            [_myTableView reloadData];
            return;
            
        }
    }
    
    // 如果是第三级cell
    NSLog(@"点击了三级cell");
}
#pragma mark 布局子控件
- (void)setUpSubViews{
    UITableView *tableview = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, 375, 667) style:UITableViewStylePlain];
    tableview.delegate = self;
    tableview.dataSource = self;
    self.myTableView = tableview;
    [self.view addSubview:tableview];
    
    
    
}

@end
