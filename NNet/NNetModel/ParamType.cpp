// ParamType.cpp
//
// NNetModel

module;

#include <iostream>
#include <unordered_map>

module NNetModel:ParamType;

import IoUtil;
import IoConstants;
import Types;

using std::unordered_map;
using std::wostream;

wchar_t const* ParamType::GetName(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map <ParamType::Value, wchar_t const* const> mapParam =
	{
		{ pulseFreqMax,   L"PulseFreqMax"    }, // maximum pulse frequency
		{ inputPeakTime,  L"StimulusMaxTime" }, // stimulus time til peak
		{ inputPeakVolt,  L"StimulusMaxVolt" }, // max amplitude of input connector stimuli
		{ inputPeakFreq,  L"StimulusMaxFreq" }, // max frequency of input connector stimuli
		{ inputBaseVolt,  L"InputBaseVolt"   }, // amplitude of normal input connector pulses
		{ inputBaseFreq,  L"InputBaseFreq"   }, // frequency of normal input connector pulses
		{ neuronPeakVolt, L"PeakVolt"        }, // peak output voltage when neuron triggers
		{ threshold,      L"Threshold"       }, // threshold voltage for neuron triggers
		{ synapseDelay,   L"Delay"           },
		{ pulseSpeed,     L"PulseSpeed"      },
		{ pulseWidth,     L"PulseWidth"      },
		{ timeResolution, L"TimeResolution"  },
		{ scanResolution, L"ScanResolution"  }
	};

	return mapParam.at(p);
}

wchar_t const* ParamType::GetUnit(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map <ParamType::Value, wchar_t const* const> mapParam =
	{
		{ pulseFreqMax,   TypeAttribute<fHertz>::unit.c_str() },
		{ inputPeakTime,  L"µs"    },
		{ inputPeakFreq,  TypeAttribute<fHertz>::unit.c_str() },
		{ inputPeakVolt,  L"mV"    },
		{ inputBaseFreq,  TypeAttribute<fHertz>::unit.c_str() },
		{ inputBaseVolt,  L"mV"    },
		{ neuronPeakVolt, L"mV"    },
		{ threshold,      L"mV"    },
		{ synapseDelay,   L"µs"    },
		{ pulseSpeed,     L"m/sec" },
		{ pulseWidth,     L"µs"    },
		{ timeResolution, L"µs"    },
		{ scanResolution, L"µm"    }
	};

	return mapParam.at(p);
}

wostream& operator<<(wostream& out, ParamType::Value const p)
{
	out << SPACE << ParamType::GetName(p);
	return out;
}
