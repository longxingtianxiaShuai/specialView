//
//  Poi2DIcon.m
//  PISAiOS
//
//  Created by daejin lee on 11. 10. 27..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import "Poi2DIconView.h"


@implementation Poi2DIconView

@synthesize delegate, poiId;

- (id) initWithImage: (UIImage *) anImage
{
//    mSzText = NULL;
    
	if ((self = [super initWithImage:anImage]) != nil )
		self.userInteractionEnabled = YES;
	return self;
}


- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	// Calculate and store offset, and pop view into front if needed
	/* CGPoint pt = */ [[touches anyObject] locationInView:self];
	//startLocation = pt;
	[[self superview] bringSubviewToFront:self];
    
    [self.delegate fireTouch_fromPoi2DIconView : mIdx shopID:(char *)mSzID name:(char *)mSzName];
}

-(void)dealloc
{
    //Temp-NSLog(@"Poi2DIconView Called Dealloc!!");
    
    /*
    if(mSzText )
    {
        delete [] mSzText;
        mSzText = NULL;
    }
     */
    
    [super dealloc];
}



-(void)SetIdx:(int)nIdx
{
    mIdx = nIdx;
}

-(void)SetId:(int)_id {
    mId = _id;
}
-(void)SetIdName:(const char*)szID name:(const char *)szName
{
    
    if(szID == NULL || szName == NULL )
        return;

    if( mSzID )
    {
        delete [] mSzID;
        mSzID = NULL;
    }
    
  
    int nLen = strlen(szID);
    mSzID = new char[nLen +1];
    
    sprintf(mSzID, "%s", szID);
    
    /////////////////////////////////////////

    if( mSzName )
    {
        delete [] mSzName;
        mSzName = NULL;
    }
    
    
    nLen = strlen(szName);
    mSzName = new char[nLen +1];
    
    sprintf(mSzName, "%s", szName);
    
}


-(const char *)GetText
{
//    return [mStrText cStringUsingEncoding: NSUTF8StringEncoding] ;
    
    return mSzName;
}





@end
