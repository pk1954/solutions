// GenDisplayWindow.cpp
//
// Win32_history

module WinHistory:GenDisplayWindow;

import WinBasics;
import Win32_Util;
import BaseWindow;

GenDisplayWindow::GenDisplayWindow() 
  : BaseWindow()
{
}

void GenDisplayWindow::StartGenDisplayWindow(HWND const hwndParent)
{
    HWND const hwnd = StartBaseWindow
    (
        hwndParent,
        CS_OWNDC,
        L"GenDisplayWindow",
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE,
		nullptr,
		nullptr
   );
    MakeLayered(hwnd, TRUE, 0, 100);
};

void GenDisplayWindow::DisplayGenerationBelowCursor(HistGeneration const gen)
{
    m_genDisplay = gen;
    Invalidate(false);
}

void GenDisplayWindow::OnPaint()
{
    if (m_genDisplay.GetLong() >= 0)
    {
        wchar_t szBuffer[30];
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(&ps);
        FillBackground(hDC, CLR_BACK);
        StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(*szBuffer), L"%u", m_genDisplay.GetLong());
        SetTextAlign(hDC, TA_CENTER);
        SetTextColor(hDC, CLR_TEXT);
        TextOut(hDC, 25, 0, szBuffer, (int)wcslen(szBuffer));
        EndPaint(&ps);
    }
}

