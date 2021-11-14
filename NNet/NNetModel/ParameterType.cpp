// ParamType::Value.cpp
//
// NNetModel

#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include "ParameterType.h"

using std::unordered_map;

void ParamType::Apply2GlobalParameters(function<void(ParamType::Value const &)> const & func)
{
	for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLastGlobal); ++i)
		func(static_cast<ParamType::Value>(i));
}

void ParamType::Apply2AllParameters(function<void(ParamType::Value const &)> const & func)
{
	for (int i = 0; i <= static_cast<int>(ParamType::Value::tParameterLast); ++i)
		func(static_cast<ParamType::Value>(i));
}

wchar_t const * const ParamType::GetName(ParamType::Value const p)
{
	static unordered_map < ParamType::Value, wchar_t const * const > mapParam =
	{
		{ ParamType::Value::stimulusMaxFreq, L"StimulusMaxFreq"  },
		{ ParamType::Value::stimulusMaxTime, L"StimulusMaxTime"  },
		{ ParamType::Value::baseFrequency,   L"BaseFrequency"    },
		{ ParamType::Value::pulseSpeed,      L"PulseSpeed"       },
		{ ParamType::Value::pulseWidth,      L"PulseWidth"       },
		{ ParamType::Value::threshold,       L"Threshold"        },
		{ ParamType::Value::peakVoltage,     L"PeakVoltage"      },
    	{ ParamType::Value::refractPeriod,   L"RefractoryPeriod" },
	    { ParamType::Value::timeResolution,  L"TimeResolution"   },
		{ ParamType::Value::signalLoss,      L"SignalLoss"       },   // Legacy
		{ ParamType::Value::stdPulseRate,    L"StdPulseRate"     },   // Legacy
    	{ ParamType::Value::pulseRate,       L"PulseRate"        }    // Legacy
	};				  
	
	return mapParam.at(p);
}

wchar_t const * const ParamType::GetUnit(ParamType::Value const p)
{
	static unordered_map < ParamType::Value, wchar_t const * const > mapParam =
	{
		{ ParamType::Value::stimulusMaxFreq, L"Hz"    },
		{ ParamType::Value::stimulusMaxTime, L"µs"    },
		{ ParamType::Value::baseFrequency,   L"Hz"    },
		{ ParamType::Value::pulseSpeed,      L"m/sec" },
		{ ParamType::Value::pulseWidth,      L"µs"    },
		{ ParamType::Value::threshold,       L"mV"    },
		{ ParamType::Value::peakVoltage,     L"mV"    },
    	{ ParamType::Value::refractPeriod,   L"µs"    },
	    { ParamType::Value::timeResolution,  L"µs"    },
		{ ParamType::Value::signalLoss,      L"1/µm"  },  // Legacy
		{ ParamType::Value::stdPulseRate,    L"Hz"    },  // Legacy
		{ ParamType::Value::pulseRate,       L"Hz"    }   // Legacy
	};				  

	return mapParam.at(p);
}

wostream & operator <<(wostream & out, ParamType::Value const p)
{
	out << L" " << ParamType::GetName(p);
	return out;
}
