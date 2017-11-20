//
//  BalloonMessageView.h
//  PISAiOS
//
//  Created by daejin lee on 11. 11. 9..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface BalloonMessageView : UIView {

    UIImageView *mImageView;
    UILabel *mLabel;

    char *mSzID;
    char *mSzName;
}

-(void)InitImageLabel : (int)nPosX posY:(int)nPosY id:(char *)szID name:(char *)szName;

-(void)SetPos: (int)nPosX posY:(int)nPosY;

-(BOOL)IsInbound:(float)fPosX posY:(float)fPosY;

-(char *)GetNamePtr;

-(char *)GetIDPtr;

@end
