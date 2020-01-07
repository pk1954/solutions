// win32_performanceWindow.h 
//
// EvoWindows

#pragma once

#include "win32_textWindow.h"

class NNetWorkThreadInterface;

class PerformanceWindow: public TextWindow
{
public:
    PerformanceWindow( );
    ~PerformanceWindow( ) {}; 

    void Start( HWND const, NNetWorkThreadInterface * const );

	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
    NNetWorkThreadInterface * m_pNNetWorkThreadInterface;

	void printLine( TextBuffer &, wchar_t const * const, double const );
};
