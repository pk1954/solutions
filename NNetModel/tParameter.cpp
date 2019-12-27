// tParameter.cpp
//
// NNetModel

#include "stdafx.h"
#include <unordered_map>
#include "tParameter.h"

using std::unordered_map;

void Apply2GlobalParameters( function<void(tParameter const &)> const & func )
{
	for ( int i = 0; i <= static_cast<int>(tParameter::tParameterLastGlobal); ++i )
		func( static_cast<tParameter>( i ) );
}

void Apply2AllParameters( function<void(tParameter const &)> const & func )
{
	for ( int i = 0; i <= static_cast<int>(tParameter::tParameterLast); ++i )
		func( static_cast<tParameter>( i ) );
}

wchar_t const * const GetParameterName( tParameter const p )
{
	static unordered_map < tParameter, wchar_t const * const > mapParam =
	{
		{ tParameter::pulseRate,     L"PulseRate"        },
		{ tParameter::pulseSpeed,    L"PulseSpeed"       },
		{ tParameter::pulseWidth,    L"PulseWidth"       },
		{ tParameter::signalLoss,    L"SignalLoss"       },
		{ tParameter::threshold,     L"Threshold"        },
		{ tParameter::peakVoltage,   L"PeakVoltage"      },
		{ tParameter::refractPeriod, L"RefractoryPeriod" }
	};				  

	return mapParam.at( p );
}

wchar_t const * const GetParameterUnit( tParameter const p )
{
	static unordered_map < tParameter, wchar_t const * const > mapParam =
	{
		{ tParameter::pulseRate,     L"Hz"    },
		{ tParameter::pulseSpeed,    L"m/sec" },
		{ tParameter::pulseWidth,    L"µs"    },
		{ tParameter::signalLoss,    L"1/µm"  },
		{ tParameter::threshold,     L"mV"    },
		{ tParameter::peakVoltage,   L"mV"    },
		{ tParameter::refractPeriod, L"µs"    }
	};				  

	return mapParam.at( p );
}
