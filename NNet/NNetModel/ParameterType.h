// ParameterType::Value.h
//
// NNetModel

#pragma once

#include <functional>

using std::function;
using std::wostream;

class ParameterType
{
public:
	enum class Value
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

	static wchar_t const * const GetName( ParameterType::Value const );
	static wchar_t const * const GetUnit( ParameterType::Value const );

	static void Apply2GlobalParameters( function<void(ParameterType::Value const &)> const & );
	static void Apply2AllParameters   ( function<void(ParameterType::Value const &)> const & );

	friend wostream & operator << ( wostream &, ParameterType const & );

private:
	Value m_value;
};
