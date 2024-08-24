// GenDisplayWindow.ixx
//
// Win32_history

export module WinHistory:GenDisplayWindow;

import WinBasics;
import BaseWindow;
import HistGeneration;

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

