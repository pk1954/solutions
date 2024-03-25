// DescriptionWindow.cpp
//
// win32_utilities

module;

#include <memory>
#include <string>
#include <array>
#include <bit>
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

module NNetWin32:DescriptionWindow;

import Win32_Util_Resource;
import Preferences;

using std::array;
using std::bit_cast;
using std::wstring;
using std::wostream;
using std::make_unique;

static WORD const ID_EDIT_CTRL { 2 };

class WrapDescWinFontSize : public Wrapper
{
public:
    WrapDescWinFontSize
    (
        wstring const& wstrName,
        DescriptionWindow& descWin
    )
        : Wrapper(wstrName),
        m_descWin(descWin)
    {}

    void operator() (Script& script) const final
    {
        m_descWin.SetFontSize(script.ScrReadInt());
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << m_descWin.GetFontSize();
    }

private:
    DescriptionWindow& m_descWin;
};

static LRESULT CALLBACK OwnerDrawEditBox
(
    HWND     hwnd, 
    UINT     uMsg, 
    WPARAM   wParam, 
    LPARAM   lParam, 
    UINT_PTR,          // unused 
    DWORD_PTR dwRefData
)
{
    auto * const pDescWin = bit_cast<DescriptionWindow *>(dwRefData);
    switch (uMsg)
    {
    case WM_MOUSEWHEEL:
        if (LOWORD(wParam) & MK_CONTROL)
        {
            pDescWin->OnMouseWheel(wParam, lParam);
            return 1;
        }
        break;

    default: 
        break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

bool DescriptionWindow::SetFontSize(int const iSize)
{
    static int const MIN_SIZE { 12 };
    static int const MAX_SIZE { 36 };

    bool const bResult = (MIN_SIZE <= iSize) && (iSize <= MAX_SIZE);
    if (bResult)
    {
        m_iFontSize = iSize;
        fontSize();
    }
    return bResult;
}

void DescriptionWindow::Start(HWND const hwndParent)
{
    PixelRect rect(100_PIXEL, 100_PIXEL, 200_PIXEL, 200_PIXEL);

    HWND const hwndDlg = StartBaseWindow
    (
        hwndParent, 
        CS_OWNDC|CS_DBLCLKS,
        L"ModelDescWindow",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CAPTION|WS_SIZEBOX,
        & rect,
        nullptr
    );

    m_hwndEdit = CreateWindowEx
    (
        0,
        L"EDIT",                 // predefined class 
        nullptr,        
        WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL
        |ES_MULTILINE            // enables multi line edit 
        |ES_WANTRETURN           // Enter key -> CR
        |ES_AUTOHSCROLL          // horizontal scroll bar
        |ES_AUTOVSCROLL          // vertical scroll bar
        |ES_EX_ZOOMABLE,         // not working
        0, 0, 0, 0,              // set size in WM_SIZE message 
        hwndDlg,                 // parent window 
        (HMENU)ID_EDIT_CTRL,     // control id
        GetModuleHandle(nullptr), 
        nullptr
    );

    (void)SetWindowSubclass(m_hwndEdit, OwnerDrawEditBox, 0, (DWORD_PTR)this) ;

    Preferences::AddWrapper(make_unique<WrapDescWinFontSize>(L"DescWinFontSize", *this));
    fontSize();
}

void DescriptionWindow::fontSize()
{
    if (m_hFont)
        DeleteObject(m_hFont);

    m_hFont = CreateFontA
    (
        m_iFontSize,  //[in] int    cHeight,
        0,            //[in] int    cWidth,
        0,            //[in] int    cEscapement,
        0,            //[in] int    cOrientation,
        0,            //[in] int    cWeight,
        0,            //[in] DWORD  bItalic,
        0,            //[in] DWORD  bUnderline,
        0,            //[in] DWORD  bStrikeOut,
        0,            //[in] DWORD  iCharSet,
        0,            //[in] DWORD  iOutPrecision,
        0,            //[in] DWORD  iClipPrecision,
        0,            //[in] DWORD  iQuality,
        0,            //[in] DWORD  iPitchAndFamily,
        nullptr       //[in] LPCSTR pszFaceName
    );

    ::SendMessage(m_hwndEdit, WM_SETFONT, (WPARAM)m_hFont, true);
}

void DescriptionWindow::Stop()
{
    BaseWindow::Stop();
    DeleteObject(m_hFont);
}

void DescriptionWindow::ClearDescription()
{
    ::Edit_SetText(m_hwndEdit, L"");
}

void DescriptionWindow::SetDescription(wstring const & wstrDesc)
{
    ::Edit_SetText(m_hwndEdit, wstrDesc.c_str());
}

int DescriptionWindow::GetLineCount() const
{
    return Edit_GetLineCount(m_hwndEdit);
}

bool DescriptionWindow::GetDescriptionLine(int const iLineNr, wstring & wstrDst) const
{
    if (iLineNr < GetLineCount())
    {
        static const int BUFLEN { 1024 };
        alignas(int) array <wchar_t, BUFLEN> buffer;
        int iCharsRead { Edit_GetLine(m_hwndEdit, iLineNr, &buffer, BUFLEN) };
        wstrDst.clear();
        for (int i = 0; i < iCharsRead; ++i)  // copy line to wstrDst 
        {                                     // removing CR and LF characters
            wchar_t c { buffer[i] };
            if ((c != L'\r') &&(c != L'\n'))
                wstrDst += c;
        }
            
        return true;
    }
    return false;
}

bool DescriptionWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
    // control notifications

    if ((LOWORD(wParam) == ID_EDIT_CTRL) && (HIWORD(wParam) == EN_CHANGE))
    {
        m_bDirty = true;
        return true; 
    }

    // other commands

    switch (int const wmId = LOWORD(wParam)) 
    { 
    case IDM_SELECT_ALL:
        Edit_SetSel(m_hwndEdit, 0, -1); 
        return true; 

    case IDM_DELETE: 
        if (delChar())
            return true; 
        break;

    default:
        break;
    }

    return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

bool DescriptionWindow::delChar()
{
    DWORD dwSelStart { 0L };
    DWORD dwSelEnd   { 0L };
    ::SendMessage(m_hwndEdit, EM_GETSEL, (WPARAM)&dwSelStart, (LPARAM)&dwSelEnd);
    if (dwSelStart == dwSelEnd)
    {
        if (dwSelStart == ::Edit_GetTextLength(m_hwndEdit))  // if cursor is at end
            return false;                                     // nothing to delete
        Edit_SetSel(m_hwndEdit, dwSelStart, dwSelStart + 1); 
    }
    ::SendMessage(m_hwndEdit, WM_CLEAR, 0, 0); 
    return true;
}

bool DescriptionWindow::OnSize(PIXEL const width, PIXEL const height)
{
    MoveWindow
    (
        m_hwndEdit, 
        0, 0,               // starting x- and y-coordinates 
        width.GetValue(),   // width of client area 
        height.GetValue(),  // height of client area 
        TRUE                // repaint window 
   );

    return true;
}

void DescriptionWindow::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
    int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
    bool const bDirection { iDelta > 0 };
    bool       bResult    { true };

    for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
    {
        int iNewSize = m_iFontSize + (bDirection ? -1 : +1);
        bResult = SetFontSize(iNewSize);
    }
    if (!bResult)
        MessageBeep(MB_ICONWARNING);
}
