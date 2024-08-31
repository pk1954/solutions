// GenDisplayWindow.ixx
//
// Win32_history

export module WinHistLib:GenDisplayWindow;

import WinBasics;
import BaseWindow;
import HistoryLib;

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

    // TODO: Use TextBuffer or TextWindow?
    static COLORREF const CLR_BACK = MakeRGB(255, 255, 0);
    static COLORREF const CLR_TEXT = MakeRGB(255, 0, 0);
};

