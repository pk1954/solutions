// GenDisplayWindow.cpp
//
// Win32_history

module WinHistLib:GenDisplayWindow;

import std;
import WinBasics;
import Win32_Util;
import BaseWindow;

using std::swprintf;
using std::wcslen;

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
    // TODO: Use TextBuffer or TextWindow?
        wchar_t szBuffer[30];
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(&ps);
//        FillBackground(hDC, CLR_BACK);
        swprintf(szBuffer, sizeof(szBuffer) / sizeof(*szBuffer), L"%ld", m_genDisplay.GetLong());
        SetTextAlign(hDC, TA_CENTER);
//        SetTextColor(hDC, CLR_TEXT);
        TextOutW(hDC, 25, 0, szBuffer, (int)wcslen(szBuffer));
        EndPaint(&ps);
    }
}

