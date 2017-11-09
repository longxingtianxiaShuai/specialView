//
//  HomeCell.m
//  Masonry布局cell
//
//  Created by BIM on 2017/11/8.
//  Copyright © 2017年 BIM. All rights reserved.
//

#import "HomeCell.h"
#import "HomeModel.h"
#import <Masonry.h>

#define iconH 80
#define iconW 100
#define marginW 10
@interface HomeCell()<UICollectionViewDelegate,UICollectionViewDataSource>
@property (nonatomic,weak)UIImageView *iconImageView;
@property (nonatomic,weak)UILabel *contentLabel;
@property (nonatomic,weak)UIView *line;

@property (nonatomic,weak)UICollectionView *collectionView;
@property (nonatomic,copy)NSArray *arrayCollection;

@property (nonatomic,assign)CGFloat contentLabelH;

@property (nonatomic,strong)NSArray *dataArray;

@end

@implementation HomeCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        [self setUpSubviews];
    }
    return self;
}
- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

#pragma mark 布局子控件
- (void)setUpSubviews{
    [self setUpAddSubviews];
    [self setUpConstraints];
}
- (void)setUpAddSubviews{
    UIImageView *iconImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:self.homeModel.icon]];
    iconImageView.contentMode=UIViewContentModeScaleAspectFill;
    iconImageView.clipsToBounds=YES;
    [self.contentView addSubview:iconImageView];
    self.iconImageView = iconImageView;
    
    UILabel *contentLabel = [[UILabel alloc] init];
    contentLabel.numberOfLines=0; //多行显示
    contentLabel.font=[UIFont systemFontOfSize:16];
    [self.contentView addSubview:contentLabel];
    self.contentLabel = contentLabel;
    
    UIView *lineView = [[UIView alloc] init];
    lineView.backgroundColor = [UIColor grayColor];
    [self.contentView addSubview:lineView];
    self.line = lineView;
    
    
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc]init];
    layout.scrollDirection = UICollectionViewScrollDirectionVertical;
    UICollectionView *collectionView = [[UICollectionView alloc]initWithFrame:CGRectZero collectionViewLayout:layout];
    [collectionView registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"cell"];
    [self.contentView addSubview:collectionView];
    collectionView.delegate = self;
    collectionView.dataSource = self;
    self.collectionView.backgroundColor = [UIColor blueColor];
    self.collectionView = collectionView;
    
    
}
- (void)setUpConstraints{
    [self.iconImageView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(iconH);
        make.width.mas_equalTo(iconW);
        make.top.mas_equalTo(self.contentView.mas_top).mas_offset(marginW);
        make.centerX.mas_equalTo(self.contentView);
        
    }];
    
    [self.contentLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.contentView.mas_left).mas_offset(marginW);
        make.right.equalTo(self.contentView.mas_right).mas_offset(-marginW);
        make.top.equalTo(self.iconImageView.mas_bottom).mas_offset(marginW);
        // 高度等计算好后再添加
    }];
    [self.collectionView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.contentLabel.mas_bottom).mas_offset(marginW);
        make.left.equalTo(self.contentView.mas_left).mas_offset(marginW);
        make.right.equalTo(self.contentView.mas_right).mas_offset(-marginW);
        //make.bottom.equalTo(self.contentView.mas_bottom).mas_offset(-marginW);
        make.height.mas_equalTo(100);
    }];
//    [self.collectionView mas_updateConstraints:^(MASConstraintMaker *make) {
//        make.top.equalTo(self.contentLabel.mas_bottom).mas_offset(marginW);
//        make.left.equalTo(self.contentView.mas_left).mas_offset(marginW);
//        make.right.equalTo(self.contentView.mas_right).mas_offset(-marginW);
//        // make.bottom.equalTo(self.contentView.mas_bottom).mas_offset(-marginW);
//        make.height.mas_equalTo(400);
//    }];
    
    [self.line mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.contentView);
        make.right.equalTo(self.contentView);
        make.height.mas_equalTo(2);
        make.bottom.equalTo(self.contentView.mas_bottom);
    }];
    
}
- (void)reloadData:(NSArray *)dataArray model:(HomeModel *)homeModel{
    _homeModel = homeModel;
    [self.contentLabel mas_makeConstraints:^(MASConstraintMaker *make) {
                make.height.mas_equalTo(self.contentLabelH);
                NSLog(@"contentLabelH==  %f",self.contentLabelH);
            }];
    //把这段代码注释掉依然可以显示
    self.dataArray = dataArray;
    
    [self.collectionView reloadData];
    CGSize collectionviewSize = self.collectionView.collectionViewLayout.collectionViewContentSize;

        [self.collectionView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self.contentLabel.mas_bottom).mas_offset(marginW);
            make.left.equalTo(self.contentView.mas_left).mas_offset(marginW);
            make.right.equalTo(self.contentView.mas_right).mas_offset(-marginW);
            // make.bottom.equalTo(self.contentView.mas_bottom).mas_offset(-marginW);
           make.height.mas_equalTo(collectionviewSize.height);
        }];
   CGFloat maxy =  CGRectGetMaxY(self.collectionView.frame);
    NSLog(@"maxy== %f   %f",maxy,collectionviewSize.height);
    homeModel.cellHeight = CGRectGetMaxY(self.collectionView.frame) + 10;
}




// label内容高度
- (CGFloat)contentLabelH{
    if (!_contentLabelH) {
        
        CGSize size = CGSizeMake([UIScreen mainScreen].bounds.size.width - 2 * marginW, MAXFLOAT);
        NSLog(@"self.homeModel.contentText== %@",self.homeModel.contentText);
        CGFloat h = [self.homeModel.contentText boundingRectWithSize:size options:NSStringDrawingUsesLineFragmentOrigin attributes:@{NSFontAttributeName:[UIFont systemFontOfSize:16]} context:nil].size.height;
        // label和底部有marginw 的间距
        _contentLabelH = h + marginW;
    }
    return _contentLabelH;
}

- (void)setHomeModel:(HomeModel *)homeModel{
    
    _homeModel = homeModel;
    //self.arrayCollection.count = 10;
    self.arrayCollection = homeModel.dataArray;
    self.iconImageView.image = [UIImage imageNamed:homeModel.icon];
    self.contentLabel.text = homeModel.contentText;
    
}

#pragma mark collectionView
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    NSLog(@"self.arrayCollection.count=== %zd",self.arrayCollection.count);
    //return self.arrayCollection.count;
    return self.dataArray.count;
}
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    
    UICollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"cell" forIndexPath:indexPath];
    cell.backgroundColor = [UIColor redColor];
    return cell;
}

#pragma mark --UICollectionViewDelegateFlowLayout
//定义每个UICollectionView 的大小
- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    return CGSizeMake([UIScreen mainScreen].bounds.size.width/ 2 - 8, 145);
}

//定义每个UICollectionView 的 margin
-(UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section
{
    return UIEdgeInsetsMake(3, 3, 0.1, 3);
}
@end
