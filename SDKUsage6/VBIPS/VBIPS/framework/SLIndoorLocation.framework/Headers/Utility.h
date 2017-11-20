//
//  utility.h
//  navIn
//
//  Created by David Törnqvist on 2010-09-11.
//  Copyright (c) 2010-2011, SenionLab AB. All rights reserved.
//

#ifndef UTILITY_H_
#define UTILITY_H_

#import <CoreGraphics/CoreGraphics.h>


static inline double gaussianNoise(double mean, double stdDev);

static inline double gaussianNoise(double mean, double stdDev)
{
	//srandom(time(NULL));
	
	// Generate uniformly random number [0,1]
	float x1= (float)random() / RAND_MAX;
	float x2= (float)random() / RAND_MAX;
	
	// Convert to Gaussian distribution
	float n = sqrt( -2 * log(x1) ) * cos( 2*M_PI*x2 );
	
	return mean + stdDev*n;
}

static inline double gaussPdf(double x, double m, double R);

static inline double gaussPdf(double x, double m, double R)
{
	double z = x - m; 
    return 1/sqrt(2*M_PI*R) * exp( -1/(2*R) *z*z );
}

static inline double uniformNoise();

static inline double uniformNoise()
{
	return (double)random() / RAND_MAX;
}

static inline void segmentLength(CGPoint* p, double* d, int N);

static inline void segmentLength(CGPoint* p, double* d, int N)
{
	for (int i=0; i<N-1; i++) {
		d[i] = sqrt(pow(p[i+1].x-p[i].x,2) + pow(p[i+1].y-p[i].y,2));
	}
}

static inline void cumSum(double* x, double* sum, int N);

static inline void cumSum(double* x, double* sum, int N)
{
	sum[0] = x[0];
	for (int i=1; i<N; i++) {
		sum[i] = sum[i-1]+x[i];
	}
}

static inline void cumSum0(double* x, double* sum, int N);

static inline void cumSum0(double* x, double* sum, int N)
{
	sum[0] = 0.0f;
	for (int i=1; i<N; i++) {
		sum[i] = sum[i-1]+x[i-1];
	}
}

static inline void segmentHeading(CGPoint* p, double* heading, int N);

static inline void segmentHeading(CGPoint* p, double* heading, int N)
{
	for (int i=0; i<N-1; i++) {
		heading[i] = atan2(p[i+1].y-p[i].y, p[i+1].x-p[i].x);
	}
}

// Note SL_DEBUG is set in Project/Edit Project Settings/Build/Preprocessing/Preprocessor Macros
#ifdef SL_DEBUG
//#define SLLog(s, ...) NSLog(@"<%@:%s:{%d}> %s = %@", [[NSString stringWithUTF8String:__FILE__] lastPathComponent], NSStringFromSelector(_cmd), __LINE__, #__VA_ARGS__ , [NSString stringWithFormat:(s), ##__VA_ARGS__])
#define SLLog(s, ...) NSLog(@"<%@:%d> %@", [[NSString stringWithUTF8String:__FILE__] lastPathComponent], \
__LINE__, [NSString stringWithFormat:(s), ##__VA_ARGS__])
#else
//#define SLLog(s, ...) NSLog(@"no log message") 
#define SLLog(s, ...)
#endif /* SL_DEBUG */

#endif /* UTILITY_H_ */