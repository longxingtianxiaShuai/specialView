//
//  TouchInfo.h
//  VBMapViewer-SKT
//
//  Created by Justin on 2014. 7. 28..
//  Copyright (c) 2014년 Virtual Builders. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface TouchInfo : NSObject

@property (nonatomic, strong) NSSet *touches;
@property (nonatomic, strong) UIEvent *events;

@end
