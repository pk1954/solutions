// MoreTypes.h
//
// Utilities

#pragma once

#include <limits>
#include <chrono>
#include "util.h"
#include "NamedType.h"
#include "PointType.h"

using std::chrono::microseconds;

////////////// MicroSecs /////////////////////////////////////

using MicroSecs = NamedType< float, struct MicroSecs_Parameter >;

constexpr const MicroSecs operator"" _MicroSecs( const long double d )
{
	return MicroSecs( CastToFloat( d ) );
}

////////////// MicroMeter /////////////////////////////////////

using MicroMeter = NamedType< float, struct MicroMeter_Parameter >;

constexpr const MicroMeter operator"" _MicroMeter( const long double d )
{
	return MicroMeter( CastToFloat( d ) );
}

MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

////////////// mV /////////////////////////////////////

using mV = NamedType<float, struct mV_Parameter >;

constexpr const mV operator"" _mV( const long double d )
{ 
	return mV( CastToFloat( d ) );
}

////////////// PERCENT /////////////////////////////////////

using PERCENT = NamedType< short, struct PERCENT_Parameter >;

PERCENT constexpr operator"" _PERCENT( unsigned long long ull )
{
	return PERCENT( CastToShort( ull ) );
}

////////////// Hertz //////////////////////////////////////////

using Hertz = NamedType< unsigned long, struct Hertz_Parameter >;

Hertz constexpr operator"" _Hertz( unsigned long long ull )
{
	return Hertz( CastToUnsignedLong( ull ) );
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
	return fHertz( CastToFloat( dl ) );
}

static MicroSecs const PulseDuration( fHertz const freq )
{
	return (freq.GetValue() == 0) 
		? MicroSecs( (std::numeric_limits<float>::max)() )
		: MicroSecs( 1e6f / freq.GetValue() );
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

inline static MicroMeterPoint OrthoVector( MicroMeterPoint const & vect, MicroMeter const width )
{
	MicroMeter umHypot = Hypot( vect );
	assert( ! IsCloseToZero( umHypot.GetValue() ) );
	return MicroMeterPoint( vect.GetY(), - vect.GetX() ) * (width / umHypot);
}

inline bool IsCloseToZero( MicroMeterPoint const pnt )
{
	return IsCloseToZero( pnt.GetXvalue() ) && IsCloseToZero( pnt.GetYvalue() );
}

using NNetPointFunc     = std::function<void (MicroMeterPoint const)>;
using NNetPointBoolFunc = std::function<bool (MicroMeterPoint const)>;

////////////// meterPerSec /////////////////////////////////////

using meterPerSec = NamedType<float, struct meterPerSec_Parameter >;

constexpr const meterPerSec operator"" _meterPerSec( const long double d )
{ 
	return meterPerSec( CastToFloat( d ) );
}

static MicroMeter CoveredDistance( meterPerSec const speed, MicroSecs const time )
{
	return MicroMeter( speed.GetValue() * time.GetValue() );
}
