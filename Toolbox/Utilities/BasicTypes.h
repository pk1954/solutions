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

////////////// MicroMeter /////////////////////////////////////

using MicroMeter = NamedType< float, struct MicroMeter_Parameter >;

constexpr MicroMeter operator"" _MicroMeter(const long double d)
{
	return MicroMeter(Cast2Float(d));
}

template <> const wstring TypeAttribute<MicroMeter>::unit   = L"m";
template <> const float   TypeAttribute<MicroMeter>::factor = 1.e6f;

inline MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

