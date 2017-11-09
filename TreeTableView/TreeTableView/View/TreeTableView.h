//
//  TreeTableView.h
//  TreeTableView
//
//  Created by yixiang on 15/7/3.
//  Copyright (c) 2015年 yixiang. All rights reserved.
//

#import <UIKit/UIKit.h>
@class Node;

@protocol TreeTableCellDelegate <NSObject>

/** 点击哪个节点的代理方法.
 @param node 传出来的数据模型.
 @param reached 是否到达叶子节点(YES 为到达了, 你可以进行push 之类操作).
 @return void.
 */
-(void)cellClick:(Node *)node didReachToBottom:(BOOL)reached;

@end

@interface TreeTableView : UITableView

@property (nonatomic , weak) id<TreeTableCellDelegate> treeTableCellDelegate;

-(instancetype)initWithFrame:(CGRect)frame withData : (NSArray *)data;

@end
