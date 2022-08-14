// Hertz.h
//
// Utilities

#pragma once

#include <limits>
#include <chrono>

import NamedType;
import SaveCast;

using std::chrono::microseconds;
using std::numeric_limits;

using Hertz = NamedType< unsigned long, struct Hertz_Parameter >;

Hertz constexpr operator"" _Hertz(unsigned long long ull)
{
	return Hertz(Cast2UnsignedLong(ull));
}

static microseconds SpikeTimeMax(Hertz const freq)
{
	return (freq.GetValue() == 0) 
		? microseconds((numeric_limits<long long>::max)())
		: microseconds(1000000L / freq.GetValue());
}
