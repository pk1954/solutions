// MoreTypes.h
//
// Utilities

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <chrono>
#include "util.h"
#include "NamedType.h"
#include "PointType.h"
#include "LineType.h"
#include "RectType.h"
#include "CircleType.h"
#include "EllipseType.h"

using std::chrono::microseconds;
using std::numeric_limits;
using std::wstring;
using std::hypotf;

////////////// fMicroSecs /////////////////////////////////////

using fMicroSecs = NamedType< float, struct MicroSecs_Parameter >;

constexpr const fMicroSecs operator"" _MicroSecs( const long double d )
{
	return fMicroSecs( Cast2Float( d ) );
}

////////////// MilliSecs /////////////////////////////////////

using MilliSecs = NamedType< unsigned long, struct MilliSecs_Parameter >;

constexpr const MilliSecs operator"" _MilliSecs( const unsigned long long d )
{
	return MilliSecs( Cast2UnsignedLong( d ) );
}

////////////// MicroMeter /////////////////////////////////////

using MicroMeter = NamedType< float, struct MicroMeter_Parameter >;

constexpr const MicroMeter operator"" _MicroMeter( const long double d )
{
	return MicroMeter( Cast2Float( d ) );
}

inline MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

// static MicroMeter const MicroMeter_NULL( std::numeric_limits<float>::infinity() );
																	   																	   
////////////// mV /////////////////////////////////////

using mV = NamedType<float, struct mV_Parameter >;

constexpr const mV operator"" _mV( const long double d )
{ 
	return mV( Cast2Float( d ) );
}

////////////// PERCENT /////////////////////////////////////

using PERCENT = NamedType< short, struct PERCENT_Parameter >;

PERCENT constexpr operator"" _PERCENT( unsigned long long ull )
{
	return PERCENT( Cast2Short( ull ) );
}

////////////// Hertz //////////////////////////////////////////

using Hertz = NamedType< unsigned long, struct Hertz_Parameter >;

Hertz constexpr operator"" _Hertz( unsigned long long ull )
{
	return Hertz( Cast2UnsignedLong( ull ) );
}

static microseconds const PulseDuration( Hertz const freq )
{
	return (freq.GetValue() == 0) 
		? microseconds( (numeric_limits<long long>::max)() )
		: microseconds( 1000000L / freq.GetValue() );
}

////////////// fHertz //////////////////////////////////////////

using fHertz = NamedType< float, struct fHertz_Parameter >;

fHertz constexpr operator"" _fHertz( long double dl )
{
	return fHertz( Cast2Float( dl ) );
}

static fMicroSecs const PulseDuration( fHertz const freq )
{
	return IsCloseToZero(freq.GetValue()) 
		? fMicroSecs( (numeric_limits<float>::max)() )
		: fMicroSecs( 1e6f / freq.GetValue() );
}

static fHertz const Frequency( fMicroSecs const us )
{
	return IsCloseToZero(us.GetValue())
		? fHertz( (numeric_limits<float>::max)() )
		: fHertz( 1e6f / us.GetValue() );
}

////////////// MicroMeterPnt /////////////////////////////////////

using MicroMeterPnt = PosType< MicroMeter >;

inline static MicroMeterPnt const NP_NULL( MicroMeterPnt::NULL_VAL() );   // compiler generates call!
inline static MicroMeterPnt const NP_ZERO( MicroMeterPnt::ZERO_VAL() );   // compiler generates call!

////////////// MicroMeterRect /////////////////////////////////////

using MicroMeterRect     = RectType< MicroMeter >;
using MicroMeterRectSize = SizeType < MicroMeter >;

MicroMeterPnt const MicroMeterRect::GetCenter() const { return (GetStartPoint() + GetEndPoint()) * 0.5f; }

////////////// MicroMeterLine          //////////////////////////

using MicroMeterLine = LineType<MicroMeter>;

////////////// MicroMeterCircle/Ellipse //////////////////////////

using MicroMeterCircle  = CircleType< MicroMeter >;
using MicroMeterEllipse = EllipseType< MicroMeter >;

////////////// meterPerSec /////////////////////////////////////

using meterPerSec = NamedType<float, struct meterPerSec_Parameter >;

constexpr const meterPerSec operator"" _meterPerSec( const long double d )
{ 
	return meterPerSec( Cast2Float( d ) );
}

static MicroMeter CoveredDistance( meterPerSec const speed, fMicroSecs const time )
{
	return MicroMeter( speed.GetValue() * time.GetValue() );
}

////////////// Radian/Degrees /////////////////////////////////////////

using Radian  = NamedType<float, struct radian_Parameter >;
using Degrees = NamedType<float, struct degrees_Parameter >;

constexpr const Radian  operator"" _Radian (const long double r) { return Radian (Cast2Float(r)); }
constexpr const Degrees operator"" _Degrees(const long double d) { return Degrees(Cast2Float(d)); }

float const RADIAN_FACTOR     { 180.0f/static_cast<float>(M_PI) };
float const INV_RADIAN_FACTOR { 1.0f/RADIAN_FACTOR };

static Radian  const Degrees2Radian(Degrees const d) { return Radian (d.GetValue() * INV_RADIAN_FACTOR); }
static Degrees const Radian2Degrees(Radian  const r) { return Degrees(r.GetValue() * RADIAN_FACTOR); }

static Radian  const Normalize(Radian  const & r) { return Radian ( fmod(r.GetValue(), RADIAN_FACTOR * 2.0f ) ); }
static Degrees const Normalize(Degrees const & d) { return Degrees( fmod(d.GetValue(), 360.0f ) ); }

static MicroMeter const Cos(Radian const r) { return MicroMeter( cos(r.GetValue()) ); } 
static MicroMeter const Sin(Radian const r) { return MicroMeter( sin(r.GetValue()) ); } 

static MicroMeterPnt const Radian2Vector(Radian const r)
{
	return MicroMeterPnt(Cos(r), Sin(r));
}

static Radian const Vector2Radian( MicroMeterPnt const & umPnt )
{
	return Radian( atan2(umPnt.GetYvalue(),umPnt.GetXvalue()) );
}

////////////// Formatting /////////////////////////////////////

void Format2wstring( fMicroSecs const, wstring & );
