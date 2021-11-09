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
		stimulusMaxFreq,
		stimulusMaxTime,
		baseFrequency,
		pulseSpeed,
		pulseWidth,
		threshold,
		peakVoltage,
		refractPeriod,
		timeResolution,
		tParameterLastGlobal = timeResolution,
		pulseRate,      // legacy
		signalLoss,     // legacy
		stdPulseRate,   // legacy
		tParameterLast = stdPulseRate
	};

	static wchar_t const * const GetName(ParamType::Value const);
	static wchar_t const * const GetUnit(ParamType::Value const);

	static void Apply2GlobalParameters(function<void(ParamType::Value const &)> const &);
	static void Apply2AllParameters   (function<void(ParamType::Value const &)> const &);

	friend wostream & operator<<(wostream &, ParamType::Value const &);

private:
	Value m_value;
};
