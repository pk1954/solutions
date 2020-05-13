// win32_performanceWindow.h 
//
// EvoWindows

#pragma once

#include "win32_textWindow.h"

class NNetModelInterface;
class WorkThreadInterface;
class ActionTimer;

class PerformanceWindow: public TextWindow
{
public:
    //PerformanceWindow( );
    //~PerformanceWindow( ); 

    void Start( HWND const, NNetModelInterface const * const, WorkThreadInterface * const, ActionTimer * const );

	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
    WorkThreadInterface      * m_pWorkThreadInterface { nullptr };
    ActionTimer              * m_pAtDisplay               { nullptr };
    NNetModelInterface const * m_pModelInterface          { nullptr };

    void printMicroSecLine( TextBuffer &, wchar_t const * const, fMicroSecs const );
    void printFloatLine   ( TextBuffer &, wchar_t const * const, float const, wchar_t const * const );
    void printIntLine     ( TextBuffer &, wchar_t const * const, int const );
};
