// BasicTypes.ixx
//
// Utilities

module;

#include <string>
#include <limits>
#include <chrono>

export module BasicTypes;

import Util;
import NamedType;
import SaveCast;

using std::chrono::microseconds;
using std::numeric_limits;
using std::wstring;

////////////// MilliSecs /////////////////////////////////////

export using MilliSecs = NamedType< unsigned long, struct MilliSecs_Parameter >;

export constexpr MilliSecs operator"" _MilliSecs(const unsigned long long d)
{
	return MilliSecs(Cast2UnsignedLong(d));
}

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

////////////// mV ////////////////////////////////////////

export using mV = NamedType<float, struct mV_Parameter >;

export constexpr mV operator"" _mV(const long double d)
{
	return mV(Cast2Float(d));
}

export template <> const wstring TypeAttribute<mV>::unit = L"V";
export template <> const float   TypeAttribute<mV>::factor = 1.e3f;

////////////// fHertz ////////////////////////////////////////

export using fHertz = NamedType< float, struct fHertz_Parameter >;

export fHertz constexpr operator"" _fHertz(long double dl)
{
	return fHertz(Cast2Float(dl));
}

export template <> const wstring TypeAttribute<fHertz>::unit = L"Hz";
export template <> const float   TypeAttribute<fHertz>::factor = 1.f;

export fMicroSecs PulseDuration(fHertz const freq)
{
	return IsCloseToZero(freq.GetValue())
		? fMicroSecs((numeric_limits<float>::max)())
		: fMicroSecs(1e6f / freq.GetValue());
}

export fHertz Frequency(fMicroSecs const us)
{
	return IsCloseToZero(us.GetValue())
		? fHertz((numeric_limits<float>::max)())
		: fHertz(1e6f / us.GetValue());
}
