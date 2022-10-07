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

wchar_t const * ParamType::GetName(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map <ParamType::Value, wchar_t const * const> mapParam =
	{
		{ inputPeakTime,  L"StimulusMaxTime"  }, // stimulus time til peak
		{ inputPeakFreq,  L"StimulusMaxFreq"  }, // stimulus maximum frequency
		{ inputPeakVolt,  L"StimulusMaxVolt"  }, // max amplitude of input connector stimuli
		{ inputBaseFreq,  L"inputBaseFreq"    }, // frequency of normal input connector pulses
		{ inputBaseVolt,  L"inputBaseVolt"    }, // amplitude of normal input connector pulses
		{ neuronPeakVolt, L"PeakVoltage"      }, // peak voltage when neuron triggers
		{ pulseSpeed,     L"PulseSpeed"       },
		{ spikeWidth,     L"SpikeWidth"       },
		{ threshold,      L"Threshold"        },
    	{ refractPeriod,  L"RefractoryPeriod" },
	    { timeResolution, L"TimeResolution"   }
	};				  
	
	return mapParam.at(p);
}

wchar_t const * ParamType::GetUnit(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map <ParamType::Value, wchar_t const * const> mapParam =
	{
		{ inputPeakFreq,   L"Hz"    },
		{ inputPeakTime,   L"탎"    },
		{ inputPeakVolt,   L"mV"    },
		{ inputBaseVolt,   L"mV"    },
		{ pulseSpeed,      L"m/sec" },
		{ spikeWidth,      L"탎"    },
		{ threshold,       L"mV"    },
		{ neuronPeakVolt,  L"mV"    },
    	{ refractPeriod,   L"탎"    },
	    { timeResolution,  L"탎"    }
	};				  

	return mapParam.at(p);
}

wostream & operator <<(wostream & out, ParamType::Value const p)
{
	out << L" " << ParamType::GetName(p);
	return out;
}
