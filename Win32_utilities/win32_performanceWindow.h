// win32_performanceWindow.h 
//
// win32_utilities

#pragma once

#include "win32_delay.h"
#include "win32_hiResTimer.h"
#include "win32_textWindow.h"

class PerformanceWindow: public TextWindow
{
public:
    PerformanceWindow( );
    ~PerformanceWindow( ) {}; 

    void Start( HWND const, Delay &, std::function<bool()> const );

    void DisplayStart( )
    {
        m_hrtimDisplay.Start( );
    };

    void DisplayStop( )
    {
        m_hrtimDisplay.Stop( );
        m_dwDisplayTime = m_hrtimDisplay.Get( );
    };

    virtual void DoPaint( TextBuffer & );

private:
    void printLine( TextBuffer &, wchar_t const * const, DWORD const, wchar_t const * const );

	Delay    * m_pDelay;
    HANDLE     m_hTimer;
    HiResTimer m_hrtimDisplay;
    DWORD      m_dwDisplayTime;      // in milliseconds
};
