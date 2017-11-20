//
//  ManageHeadingList.h
//  SLIndoorLocation
//
//  Copyright 2011 SenionLab. All rights reserved.
//

#import <Foundation/Foundation.h>

#define MAXBUFFER 100


@interface ManageHeadingList : NSObject {
	
	int bufferSize;
	double sBuffer[MAXBUFFER];
	double timeBuffer[MAXBUFFER];
	int currentInd;
    int startInd;
	double maxBufferTime; // Max buffer time in seconds
	double sRemoved;	
	
}
- (ManageHeadingList*) init;
- (void) updateSSum:(double) s atTimeInstant: (NSDate*) timestamp;
- (void) addToBuffer:(double) s atTimeInstant: (double) t;
- (void) calculateNewStartInd;
- (void) adjustBuffer;
- (void) clearBuffer;
//- (void) adjustBufferSize;
//- (void) removeFirstItemFromBuffer;
- (void) calculateSSum:(double) sNew;


@property (nonatomic) double sSum; // Only thing the heading list should return

@end
