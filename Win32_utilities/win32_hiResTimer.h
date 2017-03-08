// win32_hiResTimer.h : 
//

#pragma once

class HiResTimer
{
public:
    HiResTimer( );
    ~HiResTimer( ) {};

    void  Start( );
    void  Stop( );
    DWORD Get( );

private:

    static LONGLONG m_llFrequency;

    LONGLONG m_llTime;
    LONGLONG m_llAcc;
    LONGLONG m_llDivisor;

    LONGLONG readHiResTimer( ) const;
};
