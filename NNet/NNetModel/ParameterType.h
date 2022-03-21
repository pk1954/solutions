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
		inputPeakTime,
		inputPeakFreq,
		inputPeakVolt,
		inputBaseFreq,
		inputBaseVolt,
		neuronPeakVolt,   // peak voltage for neuron output
		threshold,        // trigger voltage for neurons
		pulseSpeed,
		pulseWidth,
		refractPeriod,
		timeResolution,
		filterSize,
		tParameterLastGlobal = filterSize,
		baseFrequency,  // legacy  
		pulseRate,      // legacy
		signalLoss,     // legacy
		stdPulseRate,   // legacy
		tParameterLast = stdPulseRate
	};

	static wchar_t const * GetName(ParamType::Value const);
	static wchar_t const * GetUnit(ParamType::Value const);

	static void Apply2GlobalParameters(auto const & func)
	{
		for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLastGlobal); ++i)
			func(static_cast<ParamType::Value>(i));
	}

	static void Apply2AllParameters(auto const & func)
	{
		for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLast); ++i)
			func(static_cast<ParamType::Value>(i));
	}

	friend wostream & operator<<(wostream &, ParamType::Value const);

private:
	Value m_value;
};
