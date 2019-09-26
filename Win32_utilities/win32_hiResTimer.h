// win32_hiResTimer.h : 
//
// win32_utilities

#pragma once

#include <chrono>
#include "windows.h"

using std::chrono::microseconds;

class HiResTimer
{
public:
    HiResTimer( );
    ~HiResTimer( ) {};

    void  Start( );
    void  Stop( );

	microseconds GetAndReset( );

private:

    static LONGLONG m_llFrequency;

    LONGLONG m_llTime;
    LONGLONG m_llAcc;
    LONGLONG m_llDivisor;

    LONGLONG readHiResTimer( ) const;
};
