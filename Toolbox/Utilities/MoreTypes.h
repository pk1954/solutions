// MoreTypes.h
//
// Utilities

#pragma once

#include <limits>
#include <chrono>
#include "util.h"
#include "NamedType.h"
#include "PointType.h"
#include "RectType.h"
#include "CircleType.h"

using std::chrono::microseconds;
using std::wstring;

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

MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

// static MicroMeter const MicroMeter_NULL( std::numeric_limits<float>::infinity() );
																	   																	   
inline bool IsCloseToZero( MicroMeter const um )
{
	return IsCloseToZero( um.GetValue() );
}

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
		? microseconds( (std::numeric_limits<long long>::max)() )
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
	return (freq.GetValue() == 0) 
		? fMicroSecs( (std::numeric_limits<float>::max)() )
		: fMicroSecs( 1e6f / freq.GetValue() );
}

static fHertz const Frequency( fMicroSecs const us )
{
	return (us.GetValue() == 0 )
		? fHertz( (std::numeric_limits<float>::max)() )
		: fHertz( 1e6f / us.GetValue() );
}

////////////// MicroMeterPoint /////////////////////////////////////

using MicroMeterPoint = PosType< MicroMeter >;

static MicroMeterPoint const NP_NULL( MicroMeterPoint::NULL_VAL() );   // compiler generates call!
static MicroMeterPoint const NP_ZERO( MicroMeterPoint::ZERO_VAL() );   // compiler generates call!

inline static MicroMeter Hypot( MicroMeterPoint const pt ) 
{ 
	return MicroMeter( std::hypotf(pt.GetXvalue(), pt.GetYvalue() ) );
};

inline static MicroMeter Distance( MicroMeterPoint const & npA, MicroMeterPoint const & npB )
{
	return Hypot( npA - npB );
}

inline static MicroMeterPoint OrthoVector( MicroMeterPoint const & vect, MicroMeter const length )
{
	MicroMeter umHypot = Hypot( vect );
	assert( ! IsCloseToZero( umHypot ) );
	return MicroMeterPoint( vect.GetY(), - vect.GetX() ) * (length / umHypot);
}

inline bool IsCloseToZero( MicroMeterPoint const pnt )
{
	return IsCloseToZero( pnt.GetX() ) && IsCloseToZero( pnt.GetY() );
}

using NNetPointFunc     = function<void (MicroMeterPoint const)>;
using NNetPointBoolFunc = function<bool (MicroMeterPoint const)>;

////////////// MicroMeterRect /////////////////////////////////////

using MicroMeterRect     = RectType< MicroMeter >;
using MicroMeterRectSize = SizeType < MicroMeter >;

MicroMeterPoint const MicroMeterRect::GetCenter( ) const { return ( GetStartPoint() + GetEndPoint() ) * 0.5f; }

////////////// MicroMeterCircle /////////////////////////////////////

using MicroMeterCircle = CircleType< MicroMeter >;

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

////////////// Formatting /////////////////////////////////////

wstring Format2wstring( fMicroSecs const );
