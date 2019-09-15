// LogarithmicTrackBar.h
//
// Utilities

#pragma once

#include <assert.h>
#include <math.h>       // pow

class LogarithmicTrackbar
{
public:
	static double const TRACKBAR_SCALING_FACTOR;

	static long TrackBar2Value( long lX ) // f(x) = 2 power (x/1000)
	{
		double const dX = static_cast<double>( lX ) / TRACKBAR_SCALING_FACTOR;
		double dRes = pow( 2.0, dX );
		return static_cast<long>( dRes );
	}

	static long Value2Trackbar( long lX )  // f(x) = 1000 * log2(x)
	{
		static double const dFactor = TRACKBAR_SCALING_FACTOR / log( 2 );

		assert( lX > 0 );
		double const dX = static_cast<double>( lX );
		double const dY = log( dX ) * dFactor;
		return static_cast<long>( dY );
	}

};
