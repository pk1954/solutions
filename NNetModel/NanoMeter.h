// NNetCoord.h
//
// NNetModel

#pragma once

#include "util.h"
#include "NamedType.h"

using NanoMeter = NamedType< double, struct NanoMeter_Parameter >;

constexpr const NanoMeter operator"" _NanoMeter( const long double d )
{
	return NanoMeter( d );
}

NanoMeter const MAX_NANO_METER{ 1e10_NanoMeter };
