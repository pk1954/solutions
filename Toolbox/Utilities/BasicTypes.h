// BasicTypes.h
//
// Utilities

#pragma once

#include <string>
#include <limits>
#include <chrono>
#include <numbers>

import NamedType;
import SaveCast;

using std::numbers::pi;
using std::chrono::microseconds;
using std::numeric_limits;
using std::wstring;

////////////// fMicroSecs /////////////////////////////////////

using fMicroSecs = NamedType< float, struct MicroSecs_Parameter >;

constexpr fMicroSecs operator"" _MicroSecs(const long double d)
{
	return fMicroSecs(Cast2Float(d));
}

template <> const wstring TypeAttribute<fMicroSecs>::unit   = L"s";
template <> const float   TypeAttribute<fMicroSecs>::factor = 1.e6f;

////////////// MilliSecs /////////////////////////////////////

using MilliSecs = NamedType< unsigned long, struct MilliSecs_Parameter >;

constexpr MilliSecs operator"" _MilliSecs(const unsigned long long d)
{
	return MilliSecs(Cast2UnsignedLong(d));
}

////////////// MicroMeter /////////////////////////////////////

using MicroMeter = NamedType< float, struct MicroMeter_Parameter >;

constexpr MicroMeter operator"" _MicroMeter(const long double d)
{
	return MicroMeter(Cast2Float(d));
}

template <> const wstring TypeAttribute<MicroMeter>::unit   = L"m";
template <> const float   TypeAttribute<MicroMeter>::factor = 1.e6f;

inline MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

////////////// mV /////////////////////////////////////

using mV = NamedType<float, struct mV_Parameter >;

constexpr mV operator"" _mV(const long double d)
{ 
	return mV(Cast2Float(d));
}

template <> const wstring TypeAttribute<mV>::unit   = L"V";
template <> const float   TypeAttribute<mV>::factor = 1.e3f;

////////////// PERCENT /////////////////////////////////////

using PERCENT = NamedType< short, struct PERCENT_Parameter >;

PERCENT constexpr operator"" _PERCENT(unsigned long long ull)
{
	return PERCENT(Cast2Short(ull));
}
