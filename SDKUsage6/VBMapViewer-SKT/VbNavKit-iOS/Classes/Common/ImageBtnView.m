//
//  ImageBtnView.m
//  PISAiOS
//
//  Created by daejin lee on 11. 11. 17..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import "ImageBtnView.h"


@implementation ImageBtnView

@synthesize delegate;

- (id) initWithImage: (UIImage *) anImage
{
    
	if ((self = [super initWithImage:anImage]) != nil )
		self.userInteractionEnabled = YES;
    
	return self;
}

-(void)SetID:(int)nID
{
    mID = nID;
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	// Calculate and store offset, and pop view into front if needed
	/* CGPoint pt = */ [[touches anyObject] locationInView:self];
    
	[[self superview] bringSubviewToFront:self];
    
    [self.delegate fireTouch_fromImageBtnView : mID ];
}

-(void)dealloc
{
    //Temp-NSLog(@"ImageBtnView Called Dealloc!!");
    
    
    [super dealloc];
}


@end
