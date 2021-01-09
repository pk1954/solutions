// ParameterType::Value.cpp
//
// NNetModel

#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include "ParameterType.h"

using std::unordered_map;

void ParameterType::Apply2GlobalParameters( function<void(ParameterType::Value const &)> const & func )
{
	for ( int i = 0; i <= static_cast<int>(ParameterType::Value::tParameterLastGlobal); ++i )
		func( static_cast<ParameterType::Value>( i ) );
}

void ParameterType::Apply2AllParameters( function<void(ParameterType::Value const &)> const & func )
{
	for ( int i = 0; i <= static_cast<int>(ParameterType::Value::tParameterLast); ++i )
		func( static_cast<ParameterType::Value>( i ) );
}

wchar_t const * const ParameterType::GetName( ParameterType::Value const p )
{
	static unordered_map < ParameterType::Value, wchar_t const * const > mapParam =
	{
		{ ParameterType::Value::pulseRate,      L"PulseRate"        },
		{ ParameterType::Value::pulseSpeed,     L"PulseSpeed"       },
		{ ParameterType::Value::pulseWidth,     L"PulseWidth"       },
		{ ParameterType::Value::signalLoss,     L"SignalLoss"       },
		{ ParameterType::Value::threshold,      L"Threshold"        },
		{ ParameterType::Value::peakVoltage,    L"PeakVoltage"      },
    	{ ParameterType::Value::refractPeriod,  L"RefractoryPeriod" },
	    { ParameterType::Value::timeResolution, L"TimeResolution"   }
	};				  
	
	return mapParam.at( p );
}

wchar_t const * const ParameterType::GetUnit( ParameterType::Value const p )
{
	static unordered_map < ParameterType::Value, wchar_t const * const > mapParam =
	{
		{ ParameterType::Value::pulseRate,      L"Hz"    },
		{ ParameterType::Value::pulseSpeed,     L"m/sec" },
		{ ParameterType::Value::pulseWidth,     L"µs"    },
		{ ParameterType::Value::signalLoss,     L"1/µm"  },
		{ ParameterType::Value::threshold,      L"mV"    },
		{ ParameterType::Value::peakVoltage,    L"mV"    },
    	{ ParameterType::Value::refractPeriod,  L"µs"    },
	    { ParameterType::Value::timeResolution, L"µs"    }
	};				  

	return mapParam.at( p );
}

wostream & operator << ( wostream & out, ParameterType::Value const p )
{
	out << L" " << ParameterType::GetName( p );
	return out;
}
