//
//  BalloonMessageView.m
//  PISAiOS
//
//  Created by daejin lee on 11. 11. 9..
//  Copyright 2011 VirtualBuilders. All rights reserved.
//

#import "BalloonMessageView.h"


@implementation BalloonMessageView

#define TAG_IMAGEVIEW   1
#define TAG_LABEL       2
#define TAG_MESSAGE     3


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        mImageView = nil;
        mLabel = nil;
        
        self.tag = TAG_MESSAGE;
        self.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        
        mSzName = NULL;
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


//-(void)InitImageLabel : (int)nPosX posY:(int)nPosY string:(char *)szText
-(void)InitImageLabel : (int)nPosX posY:(int)nPosY id:(char *)szID name:(char *)szName;
{
    
    mImageView = [[UIImageView alloc] init ];
    
    mImageView.tag = TAG_IMAGEVIEW;
    
    mLabel = [[UILabel alloc] init];
    mLabel.tag = TAG_LABEL;
    mLabel.backgroundColor = [UIColor clearColor];
    mLabel.numberOfLines = 0;
    mLabel.lineBreakMode = UILineBreakModeWordWrap;
    mLabel.font = [UIFont systemFontOfSize:14.0];
    
    mImageView.userInteractionEnabled = YES;
    
    [self addSubview:mImageView];
    [self addSubview:mLabel];
    
    [mImageView release];
    [mLabel release];
    
    
    
//    NSString *text = [NSString stringWithCString:szText encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingEUC_KR)];
    NSString *name = [NSString stringWithCString:szName encoding:NSUTF8StringEncoding]; 
    
    CGSize size = [name sizeWithFont:[UIFont systemFontOfSize:14.0]
                   constrainedToSize:CGSizeMake(220.0f, 480.0f)
                       lineBreakMode:UILineBreakModeWordWrap];
    
    UIImage *image = nil;
    
    
    //    imageView.frame = CGRectMake(320.0f-(size.width+30.0f), 2.0f, size.width+40.0f, size.height+24.0f);
    mImageView.frame = CGRectMake( 40.0f , 2.0f, size.width+27.0f, size.height+20.0f);
    
    mImageView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin;
    
    image = [[UIImage imageNamed:@"txt.png"] stretchableImageWithLeftCapWidth:28.0f topCapHeight:20.0f];
    if( image == nil )
        NSLog(@"Image Load Fail!!");
    
    //    label.frame = CGRectMake(307.0f-(size.width+5.0f), 10.0f, size.width+8.0f, size.height+5.0f);
    mLabel.frame = CGRectMake(50.0f, 5.0f, size.width+8.0f, size.height+5.0f);
    mLabel.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin;
    
    mImageView.image = image;
    mLabel.text = name;
    
    if( mSzName )
    {
        delete [] mSzName;
        mSzName = NULL;
    }
    
    
    int nLen = strlen( szID );
    mSzID = new char[nLen];
    sprintf( mSzID, "%s", szID );
    
    
    nLen = strlen( szName );
    mSzName = new char[nLen];
    sprintf( mSzName, "%s", szName );

}

-(char *)GetNamePtr
{
    return mSzName;
}

-(char *)GetIDPtr
{
    return mSzID;
}


-(void)SetPos: (int)nPosX posY:(int)nPosY
{
    
    CGRect imageFrame = mImageView.frame;
    //CGPoint imagePt = imageFrame.origin;
    //CGSize imageSize = imageFrame.size;
    
    
//    nPosX -= (CGRectGetWidth(imageFrame) - 4);
    
    //nPosX -= (CGRectGetWidth(imageFrame));
    
    float fGabX = (CGRectGetWidth(imageFrame))/5.0f;
    
    nPosX -= fGabX;
    
    //NSLog(@"ImageFrame Width: %f", CGRectGetWidth(imageFrame));
    
    nPosY -= (CGRectGetHeight(imageFrame) - 8);
    self.center = CGPointMake( nPosX, nPosY );

}

-(BOOL)IsInbound:(float)fPosX posY:(float)fPosY
{
    CGPoint selfCenter = self.center;
    //CGRect selfFrame = self.frame;
    CGRect imageFrame = mImageView.frame;
    //CGPoint imagePt = imageFrame.origin;
    
    float fX1 = selfCenter.x; // - (CGRectGetWidth(imageFrame) / 2);
    float fY1 = selfCenter.y - 5; // - (CGRectGetHeight(imageFrame) / 2);
    
    float fX2 = selfCenter.x + (CGRectGetWidth(imageFrame));
    float fY2 = fY1 + 35;
    
    
    if( fX1 <= fPosX && fX2 >= fPosX && fY1 <= fPosY && fY2 >= fPosY )
    {
        //Temp-NSLog(@"IsInbound BalloonMessage Touch Event");
        return TRUE;
    }
    
    return FALSE;

}



- (void)dealloc
{
//    [mImageView release];
//    [mLabel release];
    
    if( mSzName )
    {
        delete [] mSzName;
        mSzName = NULL;
    }
     
    [super dealloc];
    
    //Temp-NSLog(@"BalloonMessageView dealloc!!");
}


/*
- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	// Calculate and store offset, and pop view into front if needed
	CGPoint pt = [[touches anyObject] locationInView:self];
	//startLocation = pt;
	[[self superview] bringSubviewToFront:self];
    
    NSLog(@"auto detected Touched BalloonMessage");
}
*/

@end
