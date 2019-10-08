// MoreTypes.h
//
// Utilities

#pragma once

#include <chrono>
#include "util.h"
#include "NamedType.h"
#include "PointType.h"

using std::chrono::microseconds;

////////////// MIcroMeter /////////////////////////////////////

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
	return microseconds( 1000000L / freq.GetValue() );
}

////////////// MicroMeterPoint /////////////////////////////////////

using MicroMeterPoint = PosType< MicroMeter >;

static MicroMeterPoint const NP_NULL( MicroMeterPoint::NULL_VAL() );   // compiler generates call!
static MicroMeterPoint const NP_ZERO( MicroMeterPoint::ZERO_VAL() );   // compiler generates call!

using NNetPointFunc     = std::function<void (MicroMeterPoint const)>;
using NNetPointBoolFunc = std::function<bool (MicroMeterPoint const)>;

////////////// meterPerSec /////////////////////////////////////

using meterPerSec = NamedType<float, struct meterPerSec_Parameter >;

constexpr const meterPerSec operator"" _meterPerSec( const long double d )
{ 
	return meterPerSec( CastToFloat( d ) );
}

static MicroMeter CoveredDistance( meterPerSec const speed, microseconds const time )
{
	return MicroMeter( speed.GetValue() * time.count( ) );
}

////////////// milliMeterPerSec - used in messages (LPARAM) ///////////////////////

using milliMeterPerSec = NamedType<long, struct milliMeterPerSec_Parameter>;

static milliMeterPerSec Convert2milliMeterPerSec( meterPerSec const mPs )
{
	return milliMeterPerSec( CastToLong( mPs.GetValue() * 1000.0f ) );
}

static meterPerSec Convert2meterPerSec( milliMeterPerSec const mmPs )
{
	return meterPerSec( static_cast<float>( mmPs.GetValue() / 1000.0f ) );
}
