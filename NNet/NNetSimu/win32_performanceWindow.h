// win32_performanceWindow.h 
//
// EvoWindows

#pragma once

#include "win32_textWindow.h"

class NNetWorkThreadInterface;
class ActionTimer;

class PerformanceWindow: public TextWindow
{
public:
    PerformanceWindow( );
    ~PerformanceWindow( ); 

    void Start( HWND const, NNetWorkThreadInterface * const, ActionTimer * const );

	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
    NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
    ActionTimer             * m_pAtDisplay;

    void printMicroSecLine( TextBuffer &, wchar_t const * const, fMicroSecs const );
    void printFloatLine ( TextBuffer &, wchar_t const * const, float const, wchar_t const * const );
};
