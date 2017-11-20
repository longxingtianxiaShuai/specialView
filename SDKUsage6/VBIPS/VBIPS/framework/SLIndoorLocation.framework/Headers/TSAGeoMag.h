//
//  TSAGeoMag.h
//  SLIndoorLocation
//
//  Copyright 2011 SenionLab. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface TSAGeoMag : NSObject {
	
	/**
	 *	Geodetic altitude in km. An input,
	 *	but set to zero in this class.  Changed 
	 *	back to an input in version 5.  If not specified,
	 *	then is 0.
	 */
	double alt;
	/**
	 *	Geodetic latitude in deg.  An input.
	 */
	double glat;
	/**
	 *	Geodetic longitude in deg.  An input.
	 */
	double glon;
	/**
	 *	Time is decimal years.  An input
	 */
	double time;
	/**
	 *	Geomagnetic declination in deg.
	 *	East is positive, West is negative.
	 *	(The negative of variation.)
	 */
	double dec;
	/**
	 *	Geomagnetic inclination in deg.
	 *	Down is positive, up is negative.
	 */
	double dip;
	/**
	 *	Geomagnetic total intensity, in nano Teslas.
	 */
	double ti;
	/**
	 *	Geomagnetic grid variation, referenced to
	 *	grid North.  Not calculated or output in version 5.0.
	 */
	//private double gv = 0;
	/**
	 *	The maximum number of degrees of the spherical harmonic model.
	 */
	int maxdeg;
	/**
	 *	The maximum order of spherical harmonic model.
	 */
	int maxord;
	int n,m,j,D1,D2,D3,D4; //icomp,i,
	//private boolean bOutDated = false;
	/**	
	 *	An error flag to set whether the input file with the fit
	 *	coefficients was found, and read OK.  If the input file is not found,
	 *	or the data wasn't read,  
	 *	it is true, and the declination, etc. is always returned as 0.0.
	 *	In version 5, this is always false, so that a calculation is always
	 *	performed.
	 */
	bool inputError;
	
	/**	Added in version 5.  In earlier versions the date for the calculation was held as a
	 *  constant.  In version 5, if no date is specified in the calulation, this date is used
	 * 	by default.
	 */
	double currentFixedDate;
	
	/**	Added in version 5.  In earlier versions the altitude for the calculation was held as a
	 *  constant at 0.  In version 5, if no altitude is specified in the calculation, this
	 * 	altitude is used by default.
	 */
	double currentFixedAltitude;
	
	/**
	 *	The gauss coefficients of main geomagnetic model (nt).
	 */
	double c[13][13];
	//	private double c[][] = new double[13][13];
	/**
	 *	The gauss coefficients of secular geomagnetic model (nt/yr).
	 */
	double cd[13][13];
	/**
	 *	The time adjusted geomagnetic gauss coefficients (nt).
	 */
	double tc[13][13];
	/**
	 *	The theta derivative of p(n,m) (unnormalized).
	 */
	double dp[13][13];
	/**
	 *	The Schmidt normalization factors.
	 */
	double snorm[169];
	/**
	 *	The sine of (m*spherical coord. longitude).
	 */
	double sp[13];
	/**
	 *	The cosine of (m*spherical coord. longitude).
	 */
	double cp[13];
	double fn[13];
	double fm[13];
	/**
	 *	The associated legendre polynomials for m=1 (unnormalized).
	 */
	double pp[13];
	double k[13][13];
	double pi,dtr,epoch,gnm,hnm,dgnm,dhnm,flnmj,otime,oalt,
	olat,olon,dt,rlon,rlat,srlon,srlat,crlon,crlat,srlat2,
	crlat2,q,q1,q2,ct,st,r2,r,d,ca,sa,aor,ar,br,bt,bp,bpp,
	par,temp1,temp2,parp,bx,by,bz,bh, a,b,re ,a2,b2,c2,a4,b4, c4;
	
	NSString *inputVec;
	
	
}

- (TSAGeoMag*) init;
- (void) initModel;
- (void) initModelString;
- (double) getDeclinationFromLatitude:(double)lat_ andLongitude:(double)lon_;
- (double) getDeclinationFromLatitude:(double)lat_ andLongitude:(double)lon_ andYear:(double)year_ andAltitude:(double)alt_;
- (void) setCoeff;


@end

