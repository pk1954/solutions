// win32_HiResTimer.cpp :
//
// win32_utilities

#include "stdafx.h"
#include <limits>     
#include "NamedType.h"
#include "win32_HiResTimer.h"

Hertz HiResTimer::m_frequency = 0_Hertz;

// Gets the high-resolution timer's value
inline Ticks HiResTimer::ReadHiResTimer( ) const
{
    LARGE_INTEGER value;
    (void)QueryPerformanceCounter( & value );
    return Ticks( value.QuadPart );
}

HiResTimer::HiResTimer( ) :
    m_ticksAccumulated(Ticks( 0 )),
    m_ticksOnStart    (Ticks( 0 )),
	m_bStarted( false )
{
    if ( m_frequency == 0_Hertz )                  // frequency is constant for given CPU
    {                                              // first time in application the constructor is called
        LARGE_INTEGER value;                       // frequency is acquired and stored for all HiResTimers
        (void)QueryPerformanceFrequency( &value );
        m_frequency = Hertz( CastToUnsignedLong( value.QuadPart ) );
    }
}

void HiResTimer::Start( )
{
	if ( ! m_bStarted )
		m_ticksOnStart = ReadHiResTimer( );
	m_bStarted = true;
}

Ticks HiResTimer::GetTicksTilStart( )
{
	return ReadHiResTimer( ) - m_ticksOnStart;
}

void HiResTimer::Stop( )
{
	if ( m_bStarted )
		m_ticksAccumulated += GetTicksTilStart( );
	m_bStarted = false;
}

Ticks HiResTimer::MicroSecondsToTicks( microseconds const time )
{
	assert( time.count() < LLONG_MAX / m_frequency.GetValue() );
	ULONGLONG ullTime = static_cast<ULONGLONG>( time.count() );
	return Ticks( (ullTime * m_frequency.GetValue()) / MICROSECONDS_TO_SECONDS ); 
}

microseconds HiResTimer::TicksToMicroseconds( Ticks const ticks )
{
	assert( ticks.GetValue() < LLONG_MAX / MICROSECONDS_TO_SECONDS );
	ULONGLONG ullTicks = static_cast<ULONGLONG>( ticks.GetValue() );
	microseconds result
	(                                                  
		( ullTicks * MICROSECONDS_TO_SECONDS ) // converts from seconds to microseconds.
		/ m_frequency.GetValue()               // multiply *before* division, otherwise
	);                                         // division would truncate too many significant digits

	return result;
}

MicroSecs HiResTimer::TicksToMicroSecs( Ticks const ticks )
{
	assert( ticks.GetValue() < LLONG_MAX / MICROSECONDS_TO_SECONDS );
	ULONGLONG ullTicks = static_cast<ULONGLONG>( ticks.GetValue() );
	MicroSecs result
	( 
		CastToFloat
		(
			( ullTicks * MICROSECONDS_TO_SECONDS ) // converts from seconds to microseconds.
			/ m_frequency.GetValue()               // multiply *before* division, otherwise
		)                                          // division would truncate too many significant digits
	);                                         

	return result;
}

microseconds HiResTimer::GetDuration( )
{
	assert( ! m_bStarted );

	microseconds result = TicksToMicroseconds( m_ticksAccumulated );
	m_ticksAccumulated = Ticks( 0 );

	return result;
}

void HiResTimer::BusyWait( microseconds const us, Ticks & ticks )
{
	Ticks ticksToWait = MicroSecondsToTicks( us );
	if ( ticks == Ticks( 0 ) ) 
		ticks = ReadHiResTimer( );
	Ticks ticksTarget = ticks + ticksToWait;

	do
	{ 
		ticks = ReadHiResTimer( );
	} while ( ticks < ticksTarget );
}
