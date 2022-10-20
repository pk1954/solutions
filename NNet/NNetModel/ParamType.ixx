// ParamType.ixx
//
// NNetModel

module;

#include <iostream>

export module NNetModel:ParamType;

using std::wostream;

export class ParamType
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
		neuronThreshold,  // trigger voltage for neurons
		synapseThreshold, // blocking voltage for synapses
		synapseBlockTime, // blocking time for synapses
		pulseSpeed,
		spikeWidth,
		refractPeriod,
		timeResolution,
		tParameterLastGlobal = timeResolution,
		tParameterLast = timeResolution
	};

	static wchar_t const* GetName(ParamType::Value const);
	static wchar_t const* GetUnit(ParamType::Value const);

	static void Apply2GlobalParameters(auto const& func)
	{
		for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLastGlobal); ++i)
			func(static_cast<ParamType::Value>(i));
	}

	static void Apply2AllParameters(auto const& func)
	{
		for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLast); ++i)
			func(static_cast<ParamType::Value>(i));
	}

	friend wostream& operator<<(wostream&, ParamType::Value const);

private:
	Value m_value;
};
