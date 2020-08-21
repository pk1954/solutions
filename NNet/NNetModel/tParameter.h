// tParameter.h
//
// NNetModel

#pragma once

#include <functional>

using std::function;
using std::wostream;

enum class tParameter
{
	pulseSpeed,
	pulseWidth,
	signalLoss,  // not used, just for compatibility
	threshold,
	peakVoltage,
	refractPeriod,
	timeResolution,
	tParameterLastGlobal = timeResolution,
	pulseRate,
	tParameterLast = pulseRate
};

extern wchar_t const * const GetParameterName( tParameter const );
extern wchar_t const * const GetParameterUnit( tParameter const );

extern void Apply2GlobalParameters( function<void(tParameter const &)> const & );
extern void Apply2AllParameters   ( function<void(tParameter const &)> const & );

extern wostream & operator << ( wostream &, tParameter const & );
