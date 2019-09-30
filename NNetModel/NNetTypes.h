// NNetTypes.h
//
// NNetModel

#pragma once

#include <chrono>
#include "util.h"
#include "NamedType.h"

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

mV const BASE_POTENTIAL( 32.0_mV );

using std::chrono::microseconds;

microseconds const TIME_RESOLUTION = microseconds( 1 );
