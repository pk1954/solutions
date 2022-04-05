// win32_performanceWindow.h 
//
// EvoWindows

#pragma once

#include "win32_textWindow.h"

class NNetModelReaderInterface;
class SlowMotionRatio;
class ComputeThread;
class ActionTimer;

class PerformanceWindow: public TextWindow
{
public:

    void Start
    (
        HWND const, 
        ComputeThread * const, 
        SlowMotionRatio const * const, 
        ActionTimer * const 
   );

	void Stop();
    void SetModelInterface(NNetModelReaderInterface * const);
    void DoPaint(TextBuffer &) final;

private:
    ComputeThread                  * m_pComputeThread   { nullptr };
    NNetModelReaderInterface const * m_pNMRI            { nullptr };
    ActionTimer                    * m_pDisplayTimer    { nullptr };
    SlowMotionRatio          const * m_pSlowMotionRatio { nullptr };

    wstring GetTitle() const final { return L"PerformanceWindow"; }

    void printMicroSecLine(TextBuffer &, wchar_t const * const, fMicroSecs const) const;
    void printFloatLine   (TextBuffer &, wchar_t const * const, float const, wchar_t const * const) const;
    void printIntLine     (TextBuffer &, wchar_t const * const, int const) const;
};
