//
//  HomeModel.m
//  Masonry布局cell
//
//  Created by BIM on 2017/11/8.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "HomeModel.h"
#import "HomeCell.h"
@implementation HomeModel
- (CGFloat)cellHeight{
    if (!_cellHeight) {
        // 只在初始化的时候调用一次
        HomeCell *cell = [[HomeCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:homeIndentifier];
        // 调用cell计算高度的方法
        _cellHeight = [cell rowHeightWithCellModel:self];
    }
    return _cellHeight;
}
@end
