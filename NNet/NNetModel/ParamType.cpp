// ParamType.cpp
//
// NNetModel

module;

#include <iostream>
#include <unordered_map>

module NNetModel:ParamType;

import Types;

using std::unordered_map;
using std::wostream;

wchar_t const* ParamType::GetName(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map <ParamType::Value, wchar_t const* const> mapParam =
	{
		{ pulseFreqMax,     L"PulseFreqMax"     }, // maximum pulse frequency
		{ inputPeakTime,    L"StimulusMaxTime"  }, // stimulus time til peak
		{ inputPeakVolt,    L"StimulusMaxVolt"  }, // max amplitude of input connector stimuli
		{ inputPeakFreq,    L"StimulusMaxFreq"  }, // max frequency of input connector stimuli
		{ inputBaseVolt,    L"inputBaseVolt"    }, // amplitude of normal input connector pulses
		{ inputBaseFreq,    L"inputBaseFreq"    }, // frequency of normal input connector pulses
		{ neuronPeakVolt,   L"NeuronPeakVolt"   }, // peak output voltage when neuron triggers
		{ neuronThreshold,  L"NeuronThreshold"  }, // threshold voltage for neuron triggers
		{ synapseThreshold, L"SynapseThreshold" }, // threshold for synapse saturation blocking
		{ synapseDelay,     L"SynapseDelay"     },
		{ pulseSpeed,       L"PulseSpeed"       },
		{ pulseWidth,       L"PulseWidth"       },
		{ timeResolution,   L"TimeResolution"   }
	};

	return mapParam.at(p);
}

wchar_t const* ParamType::GetUnit(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map <ParamType::Value, wchar_t const* const> mapParam =
	{
		{ pulseFreqMax,     TypeAttribute<fHertz>::unit.c_str() },
		{ inputPeakTime,    L"탎"    },
		{ inputPeakFreq,    TypeAttribute<fHertz>::unit.c_str() },
		{ inputPeakVolt,    L"mV"    },
		{ inputBaseFreq,    TypeAttribute<fHertz>::unit.c_str() },
		{ inputBaseVolt,    L"mV"    },
		{ neuronPeakVolt,   L"mV"    },
		{ neuronThreshold,  L"mV"    },
		{ synapseThreshold, L"mV"    },
		{ synapseDelay,     L"탎"    },
		{ pulseSpeed,       L"m/sec" },
		{ pulseWidth,       L"탎"    },
		{ timeResolution,   L"탎"    }
	};

	return mapParam.at(p);
}

wostream& operator<<(wostream& out, ParamType::Value const p)
{
	out << L" " << ParamType::GetName(p);
	return out;
}
