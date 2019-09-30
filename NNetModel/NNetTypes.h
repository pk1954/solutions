// NNetTypes.h
//
// NNetModel

#pragma once

#include <chrono>
#include "util.h"
#include "NamedType.h"

using std::chrono::microseconds;

using MicroMeter = NamedType< double, struct MicroMeter_Parameter >;

constexpr const MicroMeter operator"" _MicroMeter( const long double d )
{
	return MicroMeter( d );
}

MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

using NanoMeter = NamedType< double, struct NanoMeter_Parameter >;

constexpr const NanoMeter operator"" _NanoMeter( const long double d )
{
	return NanoMeter( d );
}

MicroMeter const MAX_NANO_METER{ 1e10_MicroMeter };  // 10 meter

using mV = NamedType<double, struct mV_Parameter >;

constexpr const mV operator"" _mV( const long double d )
{ 
	return mV( d );
}

using meterPerSec = NamedType<double, struct meterPerSec_Parameter >;

constexpr const meterPerSec operator"" _meterPerSec( const long double d )
{ 
	return meterPerSec( d );
}

meterPerSec const STD_IMPULSE_SPEED( 20._meterPerSec );

static MicroMeter CoveredDistance( meterPerSec const speed, microseconds const time )
{
	return MicroMeter( speed.GetValue() * time.count( ) );
}

mV const BASE_POTENTIAL( 32.0_mV );

microseconds const TIME_RESOLUTION = microseconds( 1 );
