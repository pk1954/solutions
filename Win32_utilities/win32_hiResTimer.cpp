// win32_HiResTimer.cpp :
//
// win32_utilities

#include "stdafx.h"
#include "NamedType.h"
#include "win32_HiResTimer.h"

Hertz HiResTimer::m_frequency = Hertz( 0 );

// Gets the high-resolution timer's value
inline Ticks  HiResTimer::readHiResTimer( ) const
{
    LARGE_INTEGER value;
    (void)QueryPerformanceCounter( & value );
    return Ticks( value.QuadPart );
}

HiResTimer::HiResTimer( )
{
    m_ticksAccumulated = Ticks( 0 );
    m_ticksOnStart     = Ticks( 0 );

    if ( m_frequency == Hertz( 0 ) )               // frequency is constant for given CPU
    {                                              // first time in application the constructor is called
        LARGE_INTEGER value;                       // frequency is acquired and stored for all HiResTimers
        (void)QueryPerformanceFrequency( &value );
        m_frequency = Hertz( value.QuadPart );
    }
}

void HiResTimer::Start( )
{
    m_ticksOnStart = readHiResTimer( );
}

void HiResTimer::Stop( )
{
    Ticks const llDiff = readHiResTimer( ) - m_ticksOnStart;
    m_ticksAccumulated += llDiff;
}

microseconds HiResTimer::GetDuration( )
{
	return microseconds
	       (                                                 // multiplication with 1000000 
		      ( m_ticksAccumulated.GetValue() * 1000000 ) /  // converts from seconds to microseconds.
			  m_frequency.GetValue()                         // multiply before division, otherwise
		   );                                                // division would truncate too many signuficant digits
}
