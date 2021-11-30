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

	static wchar_t const * GetName(ParamType::Value const);
	static wchar_t const * GetUnit(ParamType::Value const);

	template<class FUNC>
	static void Apply2GlobalParameters(FUNC const & func)
	{
		for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLastGlobal); ++i)
			func(static_cast<ParamType::Value>(i));
	}

	template<class FUNC>
	static void Apply2AllParameters(FUNC const & func)
	{
		for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLast); ++i)
			func(static_cast<ParamType::Value>(i));
	}

	friend wostream & operator<<(wostream &, ParamType::Value const);

private:
	Value m_value;
};
