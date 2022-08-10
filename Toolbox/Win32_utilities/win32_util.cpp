// win32_util.cpp :
//

#include <iomanip>
#include <time.h>
#include "Script.h"
#include "win32_util.h"

using std::wostream;
using std::stringbuf;
using std::ostream;

bool Util::Evaluate(HWND const hwndEditField, wstring & wstrValue)
{	
    static int const BUFLEN { 20 };
    wchar_t wBuffer[BUFLEN];

    if (GetWindowText(hwndEditField, wBuffer, BUFLEN))
    {
        wstrValue = wBuffer;
        return true;
    }

    return false;
}

bool Util::Evaluate(HWND const hwndEditField, unsigned long & ulValue)
{	
    static int const BUFLEN { 20 };
    wchar_t wBuffer[BUFLEN];

    if (GetWindowText(hwndEditField, wBuffer, BUFLEN))
    {
        wstring wstrEdit(wBuffer);
        ulValue = stoul(wstrEdit);
        return true;
    }

    return false;
}

bool Util::Evaluate(HWND const hwndEditField, float & fValue)
{
    static int const BUFLEN = 20;
    static wchar_t wBuffer[BUFLEN];

    float fNewValue { fValue };
    bool  bResult   { false };

    if (GetWindowText(hwndEditField, wBuffer, BUFLEN))
    {
        wstring wstrEdit(wBuffer);

        for (auto & c : wstrEdit)  // change german decimal comma to
            if (c == L',')         // decimal point
                c = L'.';
        try
        {
            fNewValue = stof(wstrEdit);
            fValue = fNewValue;
            bResult = true;
        } catch(...)
        {
            MessageBeep(MB_ICONWARNING);
        }
    }

    SetEditField(hwndEditField, fValue);
    return bResult;
}

RECT Util::ScrReadRECT(Script & script)
{
    RECT rect;
    rect.left   = script.ScrReadLong();
    rect.top    = script.ScrReadLong();
    rect.right  = script.ScrReadLong();
    rect.bottom = script.ScrReadLong();
    return rect;
}

wostream & Util::operator << (wostream & out, RECT const & rect)
{
    out << rect.left << L' ' << rect.top << L' ' << rect.right << L' ' << rect.bottom;
    return out;
}

// CalcWindowRect: Calculates the required size of the window rectangle, based on the desired client-rectangle size. 

PixelRect Util::CalcWindowRect(PixelRect pixRect, DWORD const dwStyle)
{
	RECT rect = Util::PixelRect2RECT(pixRect);
	(void)AdjustWindowRect(&rect, dwStyle, false);	
	pixRect = Util::RECT2PixelRect(rect);
	return pixRect;
}

void Util::AdjustRight(HWND const hwnd, PIXEL const pixYpos)
{
    HWND  const hwndParent     = GetParent(hwnd);
    PIXEL const pixWidthParent = GetClientWindowWidth(hwndParent);
    PIXEL const pixWidth       = GetWindowWidth(hwnd);
    PIXEL const pixHeight      = GetWindowHeight(hwnd);
    MoveWindow(hwnd, (pixWidthParent - pixWidth), pixYpos, pixWidth, pixHeight, true);
    (void)BringWindowToTop(hwnd);
}

void Util::AdjustLeft(HWND const hwnd, PIXEL const pixYpos)
{
	PixelRectSize pnt = GetWindowSize(hwnd);
    MoveWindow(hwnd, 0_PIXEL, pixYpos, pnt.GetX(), pnt.GetY(), true);
    (void)BringWindowToTop(hwnd);
}

bool Util::MoveWindowAbsolute  // move window to given screen coordinates and set size
(
	HWND      const   hwnd,
	PixelRect const & pixRect,
	bool      const   bRepaint
)
{
	HWND const hwndParent { GetAncestor(hwnd, GA_PARENT) };
	PixelPoint pixPoint{ pixRect.GetStartPoint() };
	bool       bRes;

	if (hwndParent)
		pixPoint = Screen2Client(hwndParent, pixPoint);

	bRes = MoveWindow(hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint);
	
	if (GetWindowSize(hwnd) != pixRect.GetSize())   // can happen in strange situations
		bRes = MoveWindow(hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint);

	return bRes;
}

bool Util::MoveWindowAbsolute  // move window to given screen coordinates 
(
	HWND       const   hwnd,
	PixelPoint const & pixPos,
	bool       const   bRepaint
)
{
	return MoveWindowAbsolute(hwnd, PixelRect{ pixPos, GetWindowSize(hwnd) }, bRepaint);
}

void Util::MakeLayered(HWND const hwnd, bool const bMode, COLORREF const crKey, UINT const uiAlpha)
{
    if (bMode)
        AddWindowStyle(hwnd, WS_EX_LAYERED);
    else
        DeleteWindowStyle(hwnd, WS_EX_LAYERED);

    bool const bRes = SetLayeredWindowAttributes(hwnd, crKey, (255 * uiAlpha) / 100, (crKey == 0) ? LWA_ALPHA : LWA_COLORKEY);
    assert(bRes);
}

DWORD Util::GetNrOfCPUs(void)
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo); 
    return siSysInfo.dwNumberOfProcessors; 
}

ULONGLONG Util::GetPhysicalMemory()  // in bytes
{
    ULONGLONG ramKB;
    (void)GetPhysicallyInstalledSystemMemory(&ramKB);   // get physical memory in KB
    return ramKB * 1024;                                // compute number of bytes
}

wstring Util::GetCurrentDateAndTime()
{
    __time64_t long_time;  
    _time64(& long_time); 
    static int const SIZE { 128 };
    wchar_t buffer[SIZE];
    errno_t err = _wctime64_s(buffer, SIZE, & long_time);
    assert(! err);
    return wstring(buffer);
}

wstring Util::GetComputerName()
{
    static int const SIZE { 128 };
    wchar_t buffer[SIZE];
    DWORD   bufCharCount { SIZE };
    ::GetComputerName(buffer, & bufCharCount);
    return wstring(buffer);
}

wstring Util::GetUserName()
{
    static int const SIZE { 128 };
    wchar_t buffer[SIZE];
    DWORD   bufCharCount { SIZE };
    ::GetUserName(buffer, & bufCharCount);
    return wstring(buffer);
}

void Util::SetApplicationTitle
(
	HWND    const   hwndApp, 
	int     const   iResource,
    wstring const & wstrAdd
)
{
    static int const MAX_LOADSTRING = 100;
    static TCHAR szTitle[ MAX_LOADSTRING ];			// Titelleistentext
    (void)LoadString(GetModuleHandle(nullptr), iResource, szTitle, MAX_LOADSTRING);
    wstring newTitle { szTitle };
    if (wstrAdd != L"")
        newTitle += L" -" + wstrAdd;
    SetWindowText(hwndApp, newTitle.c_str());
}

wstring Util::GetCurrentDir()
{
    static wchar_t szBuffer[MAX_PATH];
    DWORD const dwRes = GetCurrentDirectory(MAX_PATH, szBuffer);
    assert(dwRes > 0);
    return wstring(szBuffer);
}

void Util::StdOutConsole()
{
    FILE  * fp;
    bool    bRes = AllocConsole();
    errno_t res  = _wfreopen_s(&fp, L"CONOUT$", L"w", stdout);
    wcout << L"Console started" << endl;
    HWND hwnd = ::GetConsoleWindow();
    if (hwnd != NULL)
    {
        LONG style = GetWindowLong(hwnd , GWL_STYLE);
        style = style & ~(WS_SYSMENU);
        SetWindowLong(hwnd, GWL_STYLE, style);
    }	
}

HMENU Util::PopupMenu(HMENU const hMenuParent, LPCTSTR const text)
{
    HMENU hMenuPopup = CreatePopupMenu();
    ::AppendMenu(hMenuParent, MF_POPUP, (UINT_PTR)hMenuPopup, text);
    return hMenuPopup;
}

void Util::SetNotifyByPos(HMENU const hMenu)  // activate MNS_NOTIFYBYPOS -> WM_MENUECOMMAND will be send
{
    MENUINFO m_mi = { 0 };
    m_mi.cbSize = sizeof(m_mi);
    m_mi.fMask = MIM_STYLE;
    m_mi.dwStyle = MNS_NOTIFYBYPOS;
    SetMenuInfo(hMenu, &m_mi);
}

void Util::SetMenuItemData
(
    HMENU    const hMenu,
    UINT     const uiIndex,
    UINT_PTR const data
)
{
    MENUITEMINFO m_mii = { 0 };
    m_mii.cbSize = sizeof(m_mii);
    m_mii.fMask = MIIM_DATA;
    m_mii.dwItemData = data;
    bool bRes = SetMenuItemInfo(hMenu, uiIndex, true, &m_mii);
    assert(bRes);
}

UINT_PTR Util::GetMenuItemData
(
    HMENU const hMenu,
    UINT  const uiIndex
)
{
    MENUITEMINFO m_mii = { 0 };
    m_mii.cbSize = sizeof(m_mii);
    m_mii.fMask = MIIM_DATA;
    bool bRes = GetMenuItemInfo(hMenu, uiIndex, true, &m_mii);
    assert(bRes);
    return m_mii.dwItemData;
}

void Util::AddMenu
(
    HMENU    const hMenu,
    UINT     const uFlags,
    UINT_PTR const uIDNewItem,
    LPCWSTR  const lpNewItem
)
{
    ::AppendMenu(hMenu, uFlags, uIDNewItem, lpNewItem);
    SetMenuItemData(hMenu, GetMenuItemCount(hMenu) - 1, uIDNewItem);
}

void Util::InsertMenuItem
(
    HMENU    const   hMenu,
    UINT     const   uiPos,  // zero based position in menue
    UINT_PTR const   uIDNewItem,
    wstring  const & lpNewItem
)
{
    MENUITEMINFO m_mii = { 0 };
    m_mii.cbSize = sizeof(m_mii);
    m_mii.fMask = MIIM_TYPE|MIIM_DATA|MIIM_STRING;
    m_mii.dwItemData = uIDNewItem;
    m_mii.dwTypeData = const_cast<LPTSTR>(lpNewItem.c_str());
    ::InsertMenuItem(hMenu, uiPos, true, &m_mii);
}
