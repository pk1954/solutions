// win32_HiResTimer.cpp :
//

#include "stdafx.h"
#include "win32_HiResTimer.h"

LONGLONG HiResTimer::m_llFrequency = 0;

// Gets the high-resolution timer's value
inline LONGLONG HiResTimer::readHiResTimer( ) const
{
    LARGE_INTEGER value;
    (void)QueryPerformanceCounter( &value );

    return static_cast<LONGLONG>( value.QuadPart );
}

HiResTimer::HiResTimer( )
{
    m_llAcc = 0;
    m_llTime = 0;
    m_llDivisor = 0;

    if ( m_llFrequency == 0 )
    {
        LARGE_INTEGER value;
        (void)QueryPerformanceFrequency( &value );
        m_llFrequency = static_cast<LONGLONG>( value.QuadPart );
    }
}

void HiResTimer::Start( )
{
    m_llTime = readHiResTimer( );
}

void HiResTimer::Stop( )
{
    LONGLONG const llDiff = readHiResTimer( ) - m_llTime;
    m_llAcc += llDiff;
    m_llDivisor += m_llFrequency;
}

DWORD HiResTimer::Get( )
{
    DWORD const dwAverage = static_cast<DWORD>( ( m_llDivisor == 0 ) ? 0 : ( m_llAcc * 1000000 ) / m_llDivisor );

    m_llAcc = 0;
    m_llDivisor = 0;

    return dwAverage;
}


