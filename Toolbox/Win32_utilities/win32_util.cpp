// Win32_Util.cpp :
//
// Toolbox\Win32_utilities

module;

#include <cassert>
#include <iomanip>
#include <time.h>
#include <string>
#include <iostream>
#include <Windows.h>

module Win32_Util;

using std::wstring;
using std::wostream;
using std::stringbuf;
using std::ostream;
using std::wcout;
using std::endl;
using std::stof;

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
    wstring wstrEdit;
    if (Evaluate(hwndEditField, wstrEdit))
    {
        ulValue = stoul(wstrEdit);
        return true;
    }

    return false;
}

bool Util::Evaluate(wstring & wstr, float & fValue)
{
    bool  bResult { false };

    for (wchar_t & c : wstr)  // change german decimal comma to
        if (c == L',')     // decimal point
            c = L'.';
    try
    {
        float fNewValue = stof(wstr);
        fValue = fNewValue;
        bResult = true;
    }
    catch (...)
    {
        MessageBeep(MB_ICONWARNING);
    }
    return bResult;
}

bool Util::Evaluate(HWND const hwndEditField, float & fValue)
{
    wstring wstrEdit;
    if (Evaluate(hwndEditField, wstrEdit))
    {
        if (Evaluate(wstrEdit, fValue))
        {
            SetEditField(hwndEditField, fValue);
            return true;
        }
    }

    return false;
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
    wstring const & wstrName,
    wstring const & wstrAdd
)
{
    wstring newTitle { wstrName };
    if (wstrAdd != L"")
        newTitle += L" -" + wstrAdd;
    SetWindowText(hwndApp, newTitle.c_str());
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
