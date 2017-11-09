//
//  TreeTableView.m
//  TreeTableView
//
//  Created by yixiang on 15/7/3.
//  Copyright (c) 2015年 yixiang. All rights reserved.
//

#import "TreeTableView.h"
#import "Node.h"
#import "TreeTableViewCell.h"

@interface TreeTableView ()<UITableViewDataSource,UITableViewDelegate>

@property (nonatomic , strong) NSArray *data;//传递过来已经组织好的数据（全量数据）

@property (nonatomic , strong) NSMutableArray *tempData;//用于存储数据源（部分数据）

@property (nonatomic, assign)NSInteger maxDepth;//节点最深处

@end

@implementation TreeTableView

-(instancetype)initWithFrame:(CGRect)frame withData : (NSArray *)data{
    self = [super initWithFrame:frame style:UITableViewStyleGrouped];
    if (self) {
        self.dataSource = self;
        self.delegate = self;
        _data = data;
        _tempData = [self createTempData:data];
        [self registerNib:[UINib nibWithNibName:@"TreeTableViewCell" bundle:[NSBundle mainBundle]] forCellReuseIdentifier:@"node_cell_id"];
    }
    return self;
}

/**
 * 初始化数据源
 */
-(NSMutableArray *)createTempData : (NSArray *)data{
    NSMutableArray *tempArray = [NSMutableArray array];
    NSInteger maxDepth = 0;
    for (int i=0; i<data.count; i++) {
        Node *node = [_data objectAtIndex:i];
        
        if (node.depth > maxDepth)//找最深的
        {
            maxDepth = node.depth;
        }
        
        if (node.expand) {
            [tempArray addObject:node];
        }
    }
    _maxDepth = maxDepth;
    return tempArray;
}


#pragma mark - UITableViewDataSource

#pragma mark - Required

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _tempData.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *NODE_CELL_ID = @"node_cell_id";
    
    TreeTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:NODE_CELL_ID];
    if (!cell) {
        cell = [[[NSBundle mainBundle] loadNibNamed:@"TreeTableViewCell" owner:self options:nil] lastObject];
    }
    
    Node *node = [_tempData objectAtIndex:indexPath.row];
    
    // cell有缩进的方法
    cell.indentationLevel = node.depth; // 缩进级别
    cell.indentationWidth = 30.f; // 每个缩进级别的距离
    
    cell.titleLabel.text = node.name;
    
    return cell;
}


#pragma mark - Optional
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 0.01;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 40;
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section{
    return 0.01;
}

#pragma mark - UITableViewDelegate

#pragma mark - Optional
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    //点击cell 改变 indicator 的状态
    TreeTableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    cell.indicator.selected = !cell.indicator.selected;
    
    //先修改数据源
    Node *parentNode = [_tempData objectAtIndex:indexPath.row];
    
    if (![self NodeHasChildrenNode:parentNode]) //是否到了最深处
    {
        if (_treeTableCellDelegate && [_treeTableCellDelegate respondsToSelector:@selector(cellClick:didReachToBottom:)]) {
            [_treeTableCellDelegate cellClick:parentNode didReachToBottom:YES];
        }
    }
    else
    {
        if (_treeTableCellDelegate && [_treeTableCellDelegate respondsToSelector:@selector(cellClick:didReachToBottom:)]) {
            [_treeTableCellDelegate cellClick:parentNode didReachToBottom:NO];
        }
        
        NSUInteger startPosition = indexPath.row+1;
        NSUInteger endPosition = startPosition;
        BOOL expand = NO;
        for (int i=0; i<_data.count; i++)
        {
            Node *node = [_data objectAtIndex:i];
            if (node.parentId == parentNode.nodeId)
            {
                node.expand = !node.expand;
                if (node.expand)
                {
                    [_tempData insertObject:node atIndex:endPosition];
                    expand = YES;
                    endPosition++;
                }
                else
                {
                    expand = NO;
                    endPosition = [self removeAllNodesAtParentNode:parentNode];
                    break;
                }
            }
        }
        //获得需要修正的indexPath
        NSMutableArray *indexPathArray = [NSMutableArray array];
        for (NSUInteger i=startPosition; i<endPosition; i++)
        {
            NSIndexPath *tempIndexPath = [NSIndexPath indexPathForRow:i inSection:0];
            [indexPathArray addObject:tempIndexPath];
        }
        
        //插入或者删除相关节点
        if (expand)
        {
            [self insertRowsAtIndexPaths:indexPathArray withRowAnimation:UITableViewRowAnimationAutomatic];
            [self setIndicatorStates:tableView]; //先视图更新后进行状态更改操作
        }
        else
        {
            [self resetAllDeletedCellStates:indexPathArray tableView:tableView];// 先恢复 将要 "删除" 掉cell 的状态
            [self deleteRowsAtIndexPaths:indexPathArray withRowAnimation:UITableViewRowAnimationAutomatic];
        }

    }
}

- (void)resetAllDeletedCellStates:(NSArray *)indexPaths tableView:(UITableView *)tableView//恢复状态
{
    for (NSIndexPath *indexPath in indexPaths)
    {
        TreeTableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
        cell.indicator.hidden = NO;
        cell.indicator.selected = NO;
    }
}

- (void)setIndicatorStates:(UITableView *)tableView //改变cell 上的 indicator 状态
{
    NSInteger index = 0;
    for (Node *node in _tempData)
    {
        index = [_tempData indexOfObject:node];
        TreeTableViewCell *cell = [tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:index inSection:0]];
//        NSLog(@"%p", cell);
        if (_maxDepth == node.depth)//如果已经是最深节点 全部置为选中状态, 这样就没必要循环去检测他的子节点是否最深了
        {
            cell.indicator.selected = YES;
            cell.indicator.hidden = YES;
        }
        else
        {
            if (![self NodeHasChildrenNode:node])
            {
                cell.indicator.selected = YES;
                cell.indicator.hidden = YES;
            }
        }
    }
}

- (BOOL)NodeHasChildrenNode:(Node *)node //检查是否到了节点最深处
{
    BOOL flag = NO;
    for (Node *tempNode in _data)
    {
        if (node.nodeId == tempNode.parentId) //只要有一个就证明他有子节点
        {
            flag = YES;
            break;
        }
    }
    return flag;
}

/**
 *  删除该父节点下的所有子节点（包括孙子节点）
 *
 *  @param parentNode 父节点
 *
 *  @return 该父节点下一个相邻的统一级别的节点的位置
 */
-(NSUInteger)removeAllNodesAtParentNode : (Node *)parentNode{
    NSUInteger startPosition = [_tempData indexOfObject:parentNode];
    NSUInteger endPosition = startPosition;
    for (NSUInteger i=startPosition+1; i<_tempData.count; i++) {
        Node *node = [_tempData objectAtIndex:i];
        endPosition++;
        if (node.depth <= parentNode.depth) {
            break;
        }
        if(endPosition == _tempData.count-1){
            endPosition++;
            node.expand = NO;
            break;
        }
        node.expand = NO;
    }
    if (endPosition>startPosition) {
        [_tempData removeObjectsInRange:NSMakeRange(startPosition+1, endPosition-startPosition-1)];
    }
    return endPosition;
}

//- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    _selected = NO;
//}

@end
