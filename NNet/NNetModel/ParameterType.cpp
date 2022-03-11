// ParamType::Value.cpp
//
// NNetModel

#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include "ParameterType.h"

using std::unordered_map;

wchar_t const * ParamType::GetName(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map < ParamType::Value, wchar_t const * const > mapParam =
	{
		{ inputPeakTime,  L"StimulusMaxTime"  }, // stimulus time til peak
		{ inputPeakFreq,  L"StimulusMaxFreq"  }, // stimulus maximum frequency
		{ inputPeakVolt,  L"StimulusMaxVolt"  }, // max amplitude of input connector stimuli
		{ inputBaseFreq,  L"inputBaseFreq"    }, // frequency of normal input connector pulses
		{ inputBaseVolt,  L"inputBaseVolt"    }, // amplitude of normal input connector pulses
		{ neuronPeakVolt, L"PeakVoltage"      }, // peak voltage when neuron triggers
		{ pulseSpeed,     L"PulseSpeed"       },
		{ pulseWidth,     L"PulseWidth"       },
		{ threshold,      L"Threshold"        },
    	{ refractPeriod,  L"RefractoryPeriod" },
	    { timeResolution, L"TimeResolution"   },
		{ baseFrequency,  L"BaseFrequency"    },   // Legacy
		{ signalLoss,     L"SignalLoss"       },   // Legacy
		{ stdPulseRate,   L"StdPulseRate"     },   // Legacy
    	{ pulseRate,      L"PulseRate"        }    // Legacy
	};				  
	
	return mapParam.at(p);
}

wchar_t const * ParamType::GetUnit(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map < ParamType::Value, wchar_t const * const > mapParam =
	{
		{ inputPeakFreq,   L"Hz"    },
		{ inputPeakTime,   L"µs"    },
		{ inputPeakVolt,   L"mV"    },
		{ inputBaseVolt,   L"mV"    },
		{ baseFrequency,   L"Hz"    },
		{ pulseSpeed,      L"m/sec" },
		{ pulseWidth,      L"µs"    },
		{ threshold,       L"mV"    },
		{ neuronPeakVolt,  L"mV"    },
    	{ refractPeriod,   L"µs"    },
	    { timeResolution,  L"µs"    },
		{ signalLoss,      L"1/µm"  },  // Legacy
		{ stdPulseRate,    L"Hz"    },  // Legacy
		{ pulseRate,       L"Hz"    }   // Legacy
	};				  

	return mapParam.at(p);
}

wostream & operator <<(wostream & out, ParamType::Value const p)
{
	out << L" " << ParamType::GetName(p);
	return out;
}
