// LogarithmicTrackBar.h
//
// Utilities

#pragma once

#include <assert.h>
#include <math.h>       // pow
#include "util.h"

class LogarithmicTrackbar
{
public:
	static double const TRACKBAR_SCALING_FACTOR;

	static double TrackBar2ValueD( double dX ) // f(x) = 2 power (x/1000)
	{
		return pow( 2.0, dX / TRACKBAR_SCALING_FACTOR );
	}

	static long TrackBar2ValueL( long lX )
	{
		return CastToLong( TrackBar2ValueD( static_cast<double>( lX ) ) );
	}

	static double Value2TrackbarD( double dX )  // f(x) = 1000 * log2(x)
	{
		static double const dFactor = TRACKBAR_SCALING_FACTOR / log( 2 );
		return ( dX > 0.0 ) ? log( dX ) * dFactor : 0.0;
	}

	static long Value2TrackbarL( long lX )
	{
		return CastToLong( Value2TrackbarD( static_cast<double>( lX ) ) );
	}
};
