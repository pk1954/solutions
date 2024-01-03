// win32_performanceWindow.ixx
//
// NNetWindows

module;

#include <string>
#include <Windows.h>

export module NNetWin32:PerformanceWindow;

import SlowMotionRatio;
import HiResTimer;
import TextWindow;
import NNetModel;
import :ComputeThread;

using std::wstring;

export class PerformanceWindow: public TextWindow
{
public:

    void Start
    (
        HWND const, 
        ComputeThread         * const, 
        SlowMotionRatio const * const, 
        HiResTimer            * const 
    );

    void Stop();
    void SetModelInterface(NNetModelReaderInterface * const);
    void PaintText(TextBuffer &) final;

private:
    ComputeThread                  * m_pComputeThread   { nullptr };
    NNetModelReaderInterface const * m_pNMRI            { nullptr };
    HiResTimer                     * m_pDisplayTimer    { nullptr };
    SlowMotionRatio          const * m_pSlowMotionRatio { nullptr };

    wstring GetCaption() const final { return L"PerformanceWindow"; }

    void printMicroSecLine(TextBuffer &, wchar_t const * const, fMicroSecs const) const;
    void printFloatLine   (TextBuffer &, wchar_t const * const, float const, wchar_t const * const) const;
    void printIntLine     (TextBuffer &, wchar_t const * const, int const) const;
};
