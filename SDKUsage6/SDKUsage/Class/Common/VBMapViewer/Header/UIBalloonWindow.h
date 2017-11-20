//
//  UIBalloonWindow.h
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 5. 26..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIBalloonWindow : UIWindow {
    
}

-(id)init:(NSString*)text posX:(int)x posY:(int)y;
-(void)show;
-(void)hide;
@end
