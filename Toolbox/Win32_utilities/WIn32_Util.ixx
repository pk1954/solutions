// Win32_Util.ixx
//
// Toolbox\Win32_utilities

module;

#include <tchar.h>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>

export module Win32_Util;

import BoolOp;
import Types;

using std::wostringstream;
using std::wstring;

export inline const LPCWSTR COMPILE_TIMESTAMP { _T(__DATE__) L" " _T(__TIME__) };

export inline const wstring BUILD_MODE
{ 
#ifndef NDEBUG
	L"debug"
#else
	L"release"
#endif
};

export inline LONG_PTR GetUserDataPtr(HWND hwnd)
{
    return ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

export inline LONG_PTR SetUserDataPtr(HWND hwnd, LONG_PTR ptr)
{
    return ::SetWindowLongPtr(hwnd, GWLP_USERDATA, ptr);
}

export inline LONG_PTR SetWindowStyle(HWND hwnd, LONG_PTR ptr)
{
    return ::SetWindowLongPtr(hwnd, GWL_STYLE, ptr);
}

export inline LONG_PTR SetWindowExStyle(HWND hwnd, LONG_PTR ptr)
{
    return ::SetWindowLongPtr(hwnd, GWL_EXSTYLE, ptr);
}

export inline LONG_PTR SetWindowId(HWND hwnd, LONG_PTR ptr)
{
    return ::SetWindowLongPtr(hwnd, GWLP_ID, ptr);
}

export LRESULT SendCommand(HWND const hwnd, WPARAM const wParam, LPARAM const lParam = 0) 
{ 
    return SendMessage(hwnd, WM_COMMAND, wParam, lParam); 
}

export void SetEditField(HWND const hwndEditField, auto const value)
{
    wostringstream m_wstrBuffer; 
    m_wstrBuffer << value;
    ::SetWindowText(hwndEditField, m_wstrBuffer.str().c_str());
}

export void Enable(HMENU hMenu, unsigned int const id, bool const bCrit)
{
    EnableMenuItem(hMenu, id, bCrit ? MF_ENABLED : MF_GRAYED);
}

export bool Evaluate(wstring &, float &);

export bool Evaluate(HWND const, wstring &);
export bool Evaluate(HWND const, float &);
export bool Evaluate(HWND const, unsigned long &);

export inline void SetText(HWND const hwnd, wchar_t const* const wstrText)
{
    (void)::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)(wstrText));
}

export inline void SetDlgItemState
(
    HWND           const hwnd, 
    wchar_t const* const wstrText,
    LONG_PTR       const id,
    bool           const bEnabled
)
{
	::SetWindowText(hwnd, wstrText);
	::SetWindowId  (hwnd, id);
	EnableWindow   (hwnd, bEnabled);
}

export inline void Show(HWND const hwnd, bool const bStateOld, bool const bStateNew)
{
    if (bStateNew != bStateOld)
        ShowWindow(hwnd, bStateNew ? SW_SHOW : SW_HIDE);
}

export inline void Show(HWND const hwnd, bool const bStateNew)
{
    ::Show(hwnd, IsWindowVisible(hwnd), bStateNew);
}

export inline bool Show(HWND const hwnd, tBoolOp const op)
{
    bool const bStateOld = IsWindowVisible(hwnd);
    ::Show(hwnd, ApplyOp2(bStateOld, op));
    return bStateOld;
}

export inline DWORD GetWindowStyle(HWND const hwnd)
{
    return GetWindowLong(hwnd, GWL_STYLE);
}

export inline DWORD GetWindowExStyle(HWND const hwnd)
{
    return GetWindowLong(hwnd, GWL_EXSTYLE);
}

export inline void AddWindowExStyle(HWND const hwnd, LONG_PTR const dwStyle)
{
    LONG_PTR const dwOldStyle = GetWindowExStyle(hwnd);
    LONG_PTR const dwNewStyle = dwOldStyle | dwStyle;
    SetWindowExStyle(hwnd, dwNewStyle);
}

export inline void DeleteWindowExStyle(HWND const hwnd, LONG_PTR const dwStyle)
{
    LONG_PTR const dwOldStyle = GetWindowExStyle(hwnd);
    LONG_PTR const dwNewStyle = dwOldStyle & ~dwStyle;
    LONG_PTR const dwRes       = SetWindowExStyle(hwnd, dwNewStyle);
    assert(dwRes == dwNewStyle);
}

export inline MONITORINFO GetMonitorInfo(HMONITOR hMonitor)
{
    MONITORINFO monInfo;
    monInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hMonitor, &monInfo);
    return monInfo;
}

export inline bool CtrlKeyDown()         { return GetAsyncKeyState(VK_CONTROL) & 0x01; }
export inline bool LeftMouseButtonDown() { return GetKeyState(VK_LBUTTON) & 0x8000; }

export void MakeLayered(HWND const, bool const, COLORREF const, UINT const);

export DWORD     GetNrOfCPUs();
export ULONGLONG GetPhysicalMemory();
export wstring   GetComputerName();
export wstring   GetUserName();
export wstring   GetClassName(HWND const);
export void      SetApplicationTitle(HWND const, wstring const&, wstring const & = L"");
export void      StdOutConsole();
export bool      EscapeKeyPressed();
export HMENU     PopupMenu(HMENU const, LPCTSTR const);
export void      SetNotifyByPos(HMENU const);
export void      SetMenuItemData(HMENU const, UINT const, UINT_PTR const);
export UINT_PTR  GetMenuItemData(HMENU const, UINT const);
export void      AddMenu(HMENU const, UINT const, UINT_PTR const, LPCWSTR const);
export void      CheckMenuItem(HMENU const, UINT const, bool const);
export void      ModifyMenuItem(HMENU const, UINT const, UINT const, LPCWSTR const);
export void      InsertMenuItem(HMENU const, UINT const, UINT_PTR const, wstring const&);
