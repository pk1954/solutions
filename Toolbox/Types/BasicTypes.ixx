// BasicTypes.ixx
//
// Toolbox\Types

export module Types:BasicTypes;

import std;
import std.compat;
import :NamedType;

import Util;
import SaveCast;

using std::chrono::microseconds;
using std::numeric_limits;
using std::wstring;
using std::numbers::pi;
using std::streamsize;

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

////////////// Hertz ////////////////////////////////////////

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

////////////// Radian/Degrees ////////////////////////////////////

export using Radian  = NamedType<float, struct radian_Parameter>;
export using Degrees = NamedType<float, struct degrees_Parameter>;

export constexpr Radian  operator"" _Radian (const long double r) { return Radian(Cast2Float(r)); }
export constexpr Degrees operator"" _Degrees(const long double d) { return Degrees(Cast2Float(d)); }

export inline float const RADIAN_FACTOR     { static_cast<float>(180.0 / pi) };
export inline float const INV_RADIAN_FACTOR { 1.0f / RADIAN_FACTOR };

export Radian  Degrees2Radian(Degrees const d) { return Radian(d.GetValue() * INV_RADIAN_FACTOR); }
export Degrees Radian2Degrees(Radian  const r) { return Degrees(r.GetValue() * RADIAN_FACTOR); }

export Radian  Normalize(Radian  const& r) { return Radian(fmodf(fabsf(r.GetValue()), RADIAN_FACTOR * 2.0f)); }
export Degrees Normalize(Degrees const& d) { return Degrees(fmodf(fabsf(d.GetValue()), 360.0f)); }

export MicroMeter Cos(Radian const r) { return MicroMeter(cosf(r.GetValue())); }
export MicroMeter Sin(Radian const r) { return MicroMeter(sinf(r.GetValue())); }

///////////////// Cardinal points //////////////////////////////////

export enum class CardPoint
{
	north, south, west, east, northWest, northEast, southWest, southEast
};

export void Apply2CardPoints(auto const& func)
{
	func(CardPoint::north);
	func(CardPoint::south);
	func(CardPoint::west);
	func(CardPoint::east);
	func(CardPoint::northWest);
	func(CardPoint::northEast);
	func(CardPoint::southWest);
	func(CardPoint::southEast);
}

///////////////// Utilities //////////////////////////////////

export int     StepsOfThousand(float fValue);
export wstring GetUnitPrefix(int const);
export wstring Float2wstring(float fValue, streamsize prec = 2);

export template<typename T>
wstring Format2wstring(T const val, streamsize prec = 2)
{
	return Float2wstring(val.GetValue() / TypeAttribute<T>::factor, prec) + TypeAttribute<T>::unit;
}
