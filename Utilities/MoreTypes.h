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

using MicroMeter = NamedType< double, struct MicroMeter_Parameter >;

constexpr const MicroMeter operator"" _MicroMeter( const long double d )
{
	return MicroMeter( d );
}

MicroMeter const MAX_MICRO_METER{ 1e7_MicroMeter };  // 10 meter

////////////// NanoMeter /////////////////////////////////////

using NanoMeter = NamedType< double, struct NanoMeter_Parameter >;

constexpr const NanoMeter operator"" _NanoMeter( const long double d )
{
	return NanoMeter( d );
}

MicroMeter const MAX_NANO_METER{ 1e10_MicroMeter };  // 10 meter

////////////// mV /////////////////////////////////////

using mV = NamedType<double, struct mV_Parameter >;

constexpr const mV operator"" _mV( const long double d )
{ 
	return mV( d );
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

using meterPerSec = NamedType<double, struct meterPerSec_Parameter >;

constexpr const meterPerSec operator"" _meterPerSec( const long double d )
{ 
	return meterPerSec( d );
}

static MicroMeter CoveredDistance( meterPerSec const speed, microseconds const time )
{
	return MicroMeter( speed.GetValue() * time.count( ) );
}

////////////// milliMeterPerSec - used in messages (LPARAM) ///////////////////////

using milliMeterPerSec = NamedType<long, struct milliMeterPerSec_Parameter >;

static milliMeterPerSec Convert2milliMeterPerSec( meterPerSec const mPs )
{
	return milliMeterPerSec( CastToLong( mPs.GetValue() * 1000.0 ) );
}

static meterPerSec Convert2meterPerSec( milliMeterPerSec const mmPs )
{
	return meterPerSec( static_cast<double>( mmPs.GetValue() / 1000.0 ) );
}

/////////// TODO: move constants to application

mV const BASE_POTENTIAL( 0.0_mV );

microseconds const TIME_RESOLUTION = microseconds( 100 );

NanoMeter const MINIMUM_PIXEL_SIZE =     100.0_NanoMeter;
NanoMeter const DEFAULT_PIXEL_SIZE =    1000.0_NanoMeter;  
NanoMeter const MAXIMUM_PIXEL_SIZE = 2000000.0_NanoMeter;  // 2 Meter
