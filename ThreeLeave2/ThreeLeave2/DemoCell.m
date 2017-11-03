//
//  DemoCell.m
//  ThreeDemo
//
//  Created by BIM on 2017/11/3.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "DemoCell.h"
#import "DemoModel.h"

@implementation DemoCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void)updateCellWithModel:(DemoModel *)model{
    // 如果第一级cell
    if (model.second_index == 0 && model.three_index == 0) {
        self.textLabel.text = [NSString stringWithFormat:@"%ld.%ld.%ld",model.first_index,model.second_index,model.three_index];
        return;
        
    }
    // 如果是二级cell
    if (model.three_index == 0) {
         self.textLabel.text = [NSString stringWithFormat:@"    %ld.%ld.%ld",model.first_index,model.second_index,model.three_index];
        return;
    }
    // 如果是三级cell
    self.textLabel.text = [NSString stringWithFormat:@"        %ld.%ld.%ld",model.first_index,model.second_index,model.three_index];
    
}
@end
