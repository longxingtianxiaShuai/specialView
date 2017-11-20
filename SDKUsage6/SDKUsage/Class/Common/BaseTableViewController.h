//
//  BaseTableViewController.h
//  SDKUsage
//
//  Created by BIM on 2017/10/17.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "BaseViewController.h"

@interface BaseTableViewController : BaseViewController <UITableViewDataSource,UITableViewDelegate>
@property (nonatomic,strong)NSArray *dataSource;
@end
