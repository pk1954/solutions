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
		{ stimulusMaxFreq, L"StimulusMaxFreq"  },
		{ stimulusMaxTime, L"StimulusMaxTime"  },
		{ baseFrequency,   L"BaseFrequency"    },
		{ pulseSpeed,      L"PulseSpeed"       },
		{ pulseWidth,      L"PulseWidth"       },
		{ threshold,       L"Threshold"        },
		{ peakVoltage,     L"PeakVoltage"      },
    	{ refractPeriod,   L"RefractoryPeriod" },
	    { timeResolution,  L"TimeResolution"   },
		{ signalLoss,      L"SignalLoss"       },   // Legacy
		{ stdPulseRate,    L"StdPulseRate"     },   // Legacy
    	{ pulseRate,       L"PulseRate"        }    // Legacy
	};				  
	
	return mapParam.at(p);
}

wchar_t const * ParamType::GetUnit(ParamType::Value const p)
{
	using enum ParamType::Value;
	static unordered_map < ParamType::Value, wchar_t const * const > mapParam =
	{
		{ stimulusMaxFreq, L"Hz"    },
		{ stimulusMaxTime, L"µs"    },
		{ baseFrequency,   L"Hz"    },
		{ pulseSpeed,      L"m/sec" },
		{ pulseWidth,      L"µs"    },
		{ threshold,       L"mV"    },
		{ peakVoltage,     L"mV"    },
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
