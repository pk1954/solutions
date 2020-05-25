// win32_performanceWindow.h 
//
// EvoWindows

#pragma once

#include "win32_textWindow.h"

class NNetModelInterface;
class SlowMotionRatio;
class ComputeThread;
class ActionTimer;

class PerformanceWindow: public TextWindow
{
public:
    //PerformanceWindow( );
    //~PerformanceWindow( ); 

    void Start
    ( 
        HWND const, 
        NNetModelInterface const * const, 
        ComputeThread            * const, 
        SlowMotionRatio    const * const, 
        ActionTimer              * const 
    );

	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
    ComputeThread         * m_pComputeThread   { nullptr };
    NNetModelInterface    * m_pModelInterface  { nullptr };
    ActionTimer           * m_pAtDisplay       { nullptr };
    SlowMotionRatio const * m_pSlowMotionRatio { nullptr };

    void printMicroSecLine( TextBuffer &, wchar_t const * const, fMicroSecs const );
    void printFloatLine   ( TextBuffer &, wchar_t const * const, float const, wchar_t const * const );
    void printIntLine     ( TextBuffer &, wchar_t const * const, int const );
};
