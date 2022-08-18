// BasicTypes.ixx
//
// Utilities

module;

#include <string>
#include <limits>
#include <chrono>

export module BasicTypes;

import NamedType; 
import SaveCast;

using std::chrono::microseconds;
using std::numeric_limits;
using std::wstring;

////////////// fMicroSecs /////////////////////////////////////

export using fMicroSecs = NamedType< float, struct MicroSecs_Parameter >;

export constexpr fMicroSecs operator"" _MicroSecs(const long double d)
{
	return fMicroSecs(Cast2Float(d));
}

export template <> const wstring TypeAttribute<fMicroSecs>::unit = L"s";
export template <> const float   TypeAttribute<fMicroSecs>::factor = 1.e6f;

////////////// MicroMeter /////////////////////////////////////

export using MicroMeter = NamedType< float, struct MicroMeter_Parameter >;

export constexpr MicroMeter operator"" _MicroMeter(const long double d)
{
	return MicroMeter(Cast2Float(d));
}

export template <> const wstring TypeAttribute<MicroMeter>::unit = L"m";
export template <> const float   TypeAttribute<MicroMeter>::factor = 1.e6f;

export inline MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

