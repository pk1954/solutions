// Hertz.ixx
//
// Utilities

module;

#include <chrono>
#include <limits>

export module Hertz;

import NamedType;
import SaveCast;

using std::chrono::microseconds;
using std::numeric_limits;

export using Hertz = NamedType< unsigned long, struct Hertz_Parameter >;

export Hertz constexpr operator"" _Hertz(unsigned long long ull)
{
	return Hertz(Cast2UnsignedLong(ull));
}

export microseconds SpikeTimeMax(Hertz const freq)
{
	return (freq.GetValue() == 0) 
		? microseconds((numeric_limits<long long>::max)())
		: microseconds(1000000L / freq.GetValue());
}
