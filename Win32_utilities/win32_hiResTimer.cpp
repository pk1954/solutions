// win32_HiResTimer.cpp :
//
// win32_utilities

#include "stdafx.h"
#include <limits>     
#include "NamedType.h"
#include "win32_HiResTimer.h"

Hertz HiResTimer::m_frequency = Hertz( 0 );

// Gets the high-resolution timer's value
inline Ticks HiResTimer::readHiResTimer( ) const
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
    if ( m_frequency == Hertz( 0 ) )               // frequency is constant for given CPU
    {                                              // first time in application the constructor is called
        LARGE_INTEGER value;                       // frequency is acquired and stored for all HiResTimers
        (void)QueryPerformanceFrequency( &value );
        m_frequency = Hertz( value.QuadPart );
    }
}

void HiResTimer::Start( )
{
	assert( ! m_bStarted );
    m_ticksOnStart = readHiResTimer( );
	m_bStarted = true;
}

void HiResTimer::Stop( )
{
	Ticks const ticksActual = readHiResTimer( );
	Ticks const ticksDiff   = ticksActual - m_ticksOnStart;
	assert( m_bStarted );
	m_ticksAccumulated += ticksDiff;
	m_bStarted = false;
}

Ticks HiResTimer::MicroSecondsToTicks( microseconds const us )
{
	assert( us.count() < LLONG_MAX / m_frequency.GetValue() );
	return Ticks( (us.count() * m_frequency.GetValue()) / 1000000ull ); 
}

microseconds HiResTimer::TicksToMicroseconds( Ticks const ticks )
{
	assert( ticks.GetValue() < LLONG_MAX / 1000000ull );
	microseconds result
	(                                        // multiplication with 1,000,000 
		( ticks.GetValue() * 1000000ull )    // converts from seconds to microseconds.
		/ m_frequency.GetValue()             // multiply *before* division, otherwise
	);                                       // division would truncate too many signuficant digits

	return result;
}

microseconds HiResTimer::GetDuration( )
{
	assert( ! m_bStarted );

	microseconds result = TicksToMicroseconds( m_ticksAccumulated );
	m_ticksAccumulated = Ticks( 0 );

	return result;
}
