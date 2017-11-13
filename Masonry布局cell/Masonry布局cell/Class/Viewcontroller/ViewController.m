//
//  ViewController.m
//  Masonry布局cell
//
//  Created by BIM on 2017/11/8.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "ViewController.h"
#import "HomeCell.h"
#import "HomeModel.h"


@interface ViewController ()
@property (nonatomic,strong)NSMutableArray *arr;
@property (nonatomic,strong)NSMutableArray *arrModel;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [UIColor grayColor];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.tableView.estimatedRowHeight = 200;
    self.tableView.rowHeight = UITableViewAutomaticDimension;
    //注册表格单元
    [self.tableView registerClass:[HomeCell class] forCellReuseIdentifier:homeIndentifier];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark uitableviewdelegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    //因为是我们自定义的数据 所以 这里写arr而不是arrModel  因为只有这样才会调用arr的懒加载方法
    return self.arr.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    // 取出模型
    HomeModel *model = self.arrModel[indexPath.row];
    // 传递模型给cell
   HomeCell *cell = [tableView dequeueReusableCellWithIdentifier:homeIndentifier];
    cell.homeModel = model;
    //这两句代码一定要添加
    cell.frame = tableView.bounds;
    [cell layoutIfNeeded];
    
    [cell reloadData:model];
    return cell;
    
    
}
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
   HomeModel *model = self.arrModel[indexPath.row];
    return model.cellHeight ;
    //return 200;

}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}
#pragma mark lazy
-(NSMutableArray *)arr{
    if(_arr==nil){
        _arr=[NSMutableArray array];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西"];
        [_arr addObject:@"脑子是个好东西"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西"];
        [_arr addObject:@"高通与小米达成专利授权协议的主要影响在于小米的海外市场，因为在国内小米并不会遇到专利问题。然而，分析小米在海外遇到的问题会发现，这远不是签订一个专利协议能解决，周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工周鸿祎的当别人都这么做的时候，我们换一种方式去做；和雷军的打败一种东西的一定是更新的东西而不是简单的复制。越想越觉得他们的理念异曲同工"];
        [_arr addObject:@"很多时候我们过高估计了机遇的力量，低估了规划的重要性，不明确的乐观主义者只知道未来越来越好，却不知道究竟多好，因此不去制定具体计划。他想在未来获利，但是却认为没有必要制定具体规划。"];
        [_arr addObject:@"脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西脑子是个好东西"];
        for(int i=0;i<_arr.count;i++){
            HomeModel *m=[[HomeModel alloc]init];
            if (i %2 == 0) {
                m.dataArray = @[@1];
            }else{
                m.dataArray = @[@1,@2,@3,@4];
            }
            
            m.icon=[NSString stringWithFormat:@"%i",i];
            m.contentText=_arr[i];
            //把模型那存到模型数组中
            [self.arrModel addObject:m];
            
        }
        
    }
    return _arr;
}
- (NSMutableArray *)arrModel{
    if (!_arrModel) {
        _arrModel = [NSMutableArray array];
    }
    return _arrModel;
}

@end
