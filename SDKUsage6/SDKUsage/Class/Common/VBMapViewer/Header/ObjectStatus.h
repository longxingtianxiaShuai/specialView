//
//  ObjectStatus.h
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 12. 11..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ObjectStatus : NSObject
@property(nonatomic,strong) NSMutableArray* objectList;

+(void)AddObject:(id)object;
+(void)RemoveObject:(id)object;
+(void)PrintStatus;
@end
