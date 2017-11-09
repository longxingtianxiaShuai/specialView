//
//  HomeModel.h
//  Masonry布局cell
//
//  Created by BIM on 2017/11/8.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface HomeModel : NSObject
@property (nonatomic,copy)NSString *icon;
@property (nonatomic,copy)NSString *contentText;
@property (nonatomic,assign)CGFloat cellHeight;

@property (nonatomic,copy)NSArray *dataArray;
@end
