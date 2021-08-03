// ParameterType.h
//
// NNetModel

#pragma once

#include <functional>

using std::function;
using std::wostream;

class ParamType
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

	static wchar_t const * const GetName(ParamType::Value const);
	static wchar_t const * const GetUnit(ParamType::Value const);

	static void Apply2GlobalParameters(function<void(ParamType::Value const &)> const &);
	static void Apply2AllParameters   (function<void(ParamType::Value const &)> const &);

	friend wostream & operator<<(wostream &, ParamType::Value const &);

private:
	Value m_value;
};
