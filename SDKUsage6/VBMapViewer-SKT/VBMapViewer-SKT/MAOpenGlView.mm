//
//  MAOpenGlView.m
//  VbNavKit-iOS-iiAC
//
//  Created by 이주현 on 12. 11. 26..
//  Copyright (c) 2012년 dev4. All rights reserved.
//

#import "MAOpenGlView.h"
#import "ObjectStatus.h"
#import "GLView.h"

NSMutableArray* glViewList;

@implementation MAOpenGlView
{
    GLView* glView;
}

-(void)dealloc
{
   // NSAssert(glView==nil,@"uninit를 실행해야 합니다.");
}

- (id)initWithFrame:(CGRect)frame withDelegate2:(id<GLViewDelegate2>)delegate2
{
    self = [super initWithFrame:frame];

    if (self) {
        if(glViewList==nil)
            glViewList=[[NSMutableArray alloc]init];
        
        //if(glViewList.count>0) [glViewList.lastObject pauseRender];

        glView=[[GLView alloc] initWithFrame:frame withDelegate2:delegate2];
        
        //SKT용 기본 환경 - Ambient조명=1, 배경색=1
        RunTimeConfig config = [glView getRuntimeConfig];
        config.fAmbientColor4f[0]=config.fAmbientColor4f[1]=config.fAmbientColor4f[2]=config.fAmbientColor4f[3]=1.f;
        config.fBackgroundColor4f[0]=config.fBackgroundColor4f[1]=config.fBackgroundColor4f[2]=config.fBackgroundColor4f[3]=1.f;
        
        [glView setRuntimeConfig:&config];
        
        [glViewList addObject:glView];
    }
    
    //[ObjectStatus AddObject:self];

    return self;
}

-(GLView*)GLView
{
    return glView;
}

-(void)uninit
{
    //[ObjectStatus RemoveObject:self];

    [glView Prerelease];
    [glView releaseAll];

    [glViewList removeObject:glView];
    //glView=nil;
    
    // if(glViewList>0) [glViewList.lastObject resumeRender];
}

@end
