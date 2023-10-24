// GenDisplayWindow.ixx
//
// Win32_history

module;

#include "Windows.h"
#include "HistoryGeneration.h"

export module GenDisplayWindow;

import BaseWindow;

export class GenDisplayWindow : public BaseWindow
{
public:
    GenDisplayWindow();
    virtual ~GenDisplayWindow() {};

    void StartGenDisplayWindow(HWND const);
    void DisplayGenerationBelowCursor(HistGeneration const);

private:
    void OnPaint() final;

    HistGeneration m_genDisplay;

    static COLORREF const CLR_BACK = RGB(255, 255, 0);
    static COLORREF const CLR_TEXT = RGB(255, 0, 0);
};

