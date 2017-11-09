//
//  TreeTableViewCell.h
//  TreeTableView
//
//  Created by Meonardo on 16/3/18.
//  Copyright © 2016年 yixiang. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TreeTableViewCell : UITableViewCell

@property (strong, nonatomic) IBOutlet UIButton *indicator; //我刚开始用图片不行, 出现你指出来那种问题 (不知道为什么, 你可以测试一下),
@property (strong, nonatomic) IBOutlet UILabel *titleLabel;

@end
