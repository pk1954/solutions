// tParameter.h
//
// NNetModel

#pragma once

#include <functional>

using std::function;

enum class tParameter
{
	pulseSpeed,
	pulseWidth,
	signalLoss,
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
