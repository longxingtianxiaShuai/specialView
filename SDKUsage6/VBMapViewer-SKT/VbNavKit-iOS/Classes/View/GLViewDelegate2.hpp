//
//  GLViewDelegate.h
//  VbNavKit-iOS
//
//  Created by Hunju Hwang on 12. 11. 13..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Projection;

@protocol GLViewDelegate2 <NSObject>
-(void)onGLViewUpdated:(Projection*)newMatrix;
@end
