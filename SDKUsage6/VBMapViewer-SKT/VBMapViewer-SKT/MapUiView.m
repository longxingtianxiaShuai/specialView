//
//  MapUiView.m
//  VBMapViewer-SKT
//
//  Created by Leejoohyun on 2014. 2. 18..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import "MapUiView.h"
#import "CommonLib.h"
#import "FloorSlider.h"
#import "PoiCanvas.h"
#import "MAMapView.h"

@implementation MapUiView {
    UIButton *zoneButton;;
    UIButton *fullscreenButton;
    UIButton *textureButton;
    UIButton *cameraButton;
    UIButton *myPositionButton;
    UIButton *categoryButton;
    UIButton *poiButton;
    
    FloorSlider *floorSlider;
}
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
        //[self setBackgroundColor:[UIColor redColor]];
        
        //[self setUserInteractionEnabled:NO];
        
        [self drawUI];
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

// justin - png 파일들은 framework으로 제공되어야 하나?
- (void)drawUI
{
    zoneButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(10, 20, 38, 38) action:@selector(btnShowZone:) normalImageName:@"building_001.png" highlightImageName:@"building_001_on.png" target:self];
    [self addSubview:zoneButton];
    
    fullscreenButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(272, 20, 38, 38) action:@selector(btnShowZone:) normalImageName:@"fullscreen_001.png" highlightImageName:@"fullscreen_001_on.png" target:self];
    [self addSubview:fullscreenButton];
    
    textureButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(272, 70, 38, 38) action:@selector(btnShowZone:) normalImageName:@"texturemode_001.png" highlightImageName:@"texturemode_001_on.png" target:self];
    [self addSubview:textureButton];
    
    cameraButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(272, 120, 38, 38) action:@selector(btnShowZone:) normalImageName:@"walkingview_001.png" highlightImageName:@"walkingview_001_on.png" target:self];
    [self addSubview:cameraButton];
    
    myPositionButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(10, self.frame.size.height - 48, 38, 38) action:@selector(btnShowZone:) normalImageName:@"yah_001.png" highlightImageName:@"yah_001_on.png" target:self];
    [self addSubview:myPositionButton];
    
    categoryButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(230, self.frame.size.height - 48, 38, 38) action:@selector(btnShowZone:) normalImageName:@"category_001.png" highlightImageName:@"category_001_on.png" target:self];
    [self addSubview:categoryButton];
    
    poiButton = [CommonLib createButton:UIButtonTypeCustom size:CGRectMake(272, self.frame.size.height - 48, 38, 38) action:@selector(btnShowPoi:) normalImageName:@"poi_001.png" highlightImageName:@"poi_001_on.png" target:self];
    [self addSubview:poiButton];
    
    floorSlider = [[FloorSlider alloc] initWithFrame:CGRectMake(272, 170, 38, 219)];
    [self addSubview:floorSlider];

}

-(void)btnShowZone:(id)sender {
    
}

-(void)btnShowPoi:(id)sender {
    
}

@end
