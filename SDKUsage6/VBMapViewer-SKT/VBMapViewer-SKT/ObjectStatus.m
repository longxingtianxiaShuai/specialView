//
//  ObjectStatus.m
//  VbNavKit-iOS-iiAC
//
//  Created by 황헌주 on 12. 12. 11..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import "ObjectStatus.h"

@implementation ObjectStatus

+(ObjectStatus*)Instance
{
    static dispatch_once_t dispatchOnce;
    static ObjectStatus* instance = nil;
    
    dispatch_once(&dispatchOnce, ^{
        instance=[[ObjectStatus alloc] init];
    });
    
    return instance;
}

-(id)init
{
    self=[super init];
    
    self.objectList=[[NSMutableArray alloc]init];
    
    return self;
}

+(void)AddObject:(id)object
{
    [[ObjectStatus Instance].objectList addObject:object];
    
    //[self PrintStatus];
}
+(void)RemoveObject:(id)object
{
    [[ObjectStatus Instance].objectList removeObject:object];
    
    //[self PrintStatus];
}

+(void)PrintStatus
{
    //NSMutableArray* objectList=[ObjectStatus Instance].objectList;

    //NSLog(@"Object Status Start");
    
    //for(id object in objectList)
    //{
     //   NSLog(@"Object=%@ Address=%p"
     //         ,NSStringFromClass([object class])
     //         ,object);
    //}
    //NSLog(@"Object Status End");
}
@end
