//
//  HomeCell.h
//  Masonry布局cell
//
//  Created by BIM on 2017/11/8.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HomeModel;
static  NSString *homeIndentifier=@"homeCell";

@interface HomeCell : UITableViewCell

@property (nonatomic,strong)HomeModel *homeModel;

- (void)reloadData :(HomeModel *)homeModel;
@end
