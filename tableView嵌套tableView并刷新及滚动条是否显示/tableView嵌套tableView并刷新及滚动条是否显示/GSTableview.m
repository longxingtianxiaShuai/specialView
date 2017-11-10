//
//  GSTableview.m
//  tableView嵌套tableView并刷新及滚动条是否显示
//
//  Created by BIM on 2017/11/10.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "GSTableview.h"

@implementation GSTableview

/**
 同时识别多个手势
 
 @param gestureRecognizer gestureRecognizer description
 @param otherGestureRecognizer otherGestureRecognizer description
 @return return value description
 */
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return YES;
}

@end
