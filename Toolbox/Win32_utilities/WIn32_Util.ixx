// Win32_Util.ixx
//
// Toolbox\Win32_utilities

module;

#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>

export module Win32_Util;

import BoolOp;
import NamedType;
import BasicTypes;
import MoreTypes;
import PixelTypes;

using std::wostringstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;

namespace Util
{
    export inline LONG_PTR GetUserDataPtr(HWND hwnd)
    {
        return ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    export inline void SetUserDataPtr(HWND hwnd, LONG_PTR ptr)
    {
        (void) ::SetWindowLongPtr(hwnd, GWLP_USERDATA, ptr);
    }

    export void SetEditField(HWND const hwndEditField, auto const value)
    {
        wostringstream m_wstrBuffer;
        m_wstrBuffer << value;
        ::SetWindowText(hwndEditField, m_wstrBuffer.str().c_str());
    }

    export bool Evaluate(HWND const, wstring &);
    export bool Evaluate(HWND const, float &);
    export bool Evaluate(HWND const, unsigned long &);

    export inline void SetText(HWND const hwnd, wchar_t const* const wstrText)
    {
        (void)::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)(wstrText));
    }

    export inline void Show(HWND const hwnd, bool const bStateOld, bool const bStateNew)
    {
        if (bStateNew != bStateOld)
            ShowWindow(hwnd, bStateNew ? SW_SHOW : SW_HIDE);
    }

    export inline void Show(HWND const hwnd, bool const bStateNew)
    {
        Util::Show(hwnd, IsWindowVisible(hwnd), bStateNew);
    }

    export inline bool Show(HWND const hwnd, tBoolOp const op)
    {
        bool const bStateOld = IsWindowVisible(hwnd);
        Util::Show(hwnd, ApplyOp2(bStateOld, op));
        return bStateOld;
    }

    export inline void AddWindowStyle(HWND const hwnd, DWORD const dwStyle)
    {
        DWORD const dwOldStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        DWORD const dwNewStyle = dwOldStyle | dwStyle;
        SetWindowLong(hwnd, GWL_EXSTYLE, dwNewStyle);
    }

    export inline void DeleteWindowStyle(HWND const hwnd, DWORD const dwStyle)
    {
        DWORD const dwOldStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        DWORD const dwNewStyle = dwOldStyle & ~dwStyle;
        LONG  const lRes = SetWindowLong(hwnd, GWL_EXSTYLE, dwNewStyle);
        assert(lRes == dwNewStyle);
    }

    export inline MONITORINFO GetMonitorInfo(HMONITOR hMonitor)
    {
        MONITORINFO monInfo;
        monInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &monInfo);
        return monInfo;
    }

    export void MakeLayered(HWND const, bool const, COLORREF const, UINT const);

    export DWORD     GetNrOfCPUs();
    export ULONGLONG GetPhysicalMemory();
    export wstring   GetCurrentDateAndTime();
    export wstring   GetComputerName();
    export wstring   GetUserName();
    export wstring   GetCurrentDir();
    export void      SetApplicationTitle(HWND const, wstring const&, wstring const & = L"");
    export void      StdOutConsole();
    export bool      EscapeKeyPressed();
    export HMENU     PopupMenu(HMENU const, LPCTSTR const);
    export void      SetNotifyByPos(HMENU const);
    export void      SetMenuItemData(HMENU const, UINT const, UINT_PTR const);
    export UINT_PTR  GetMenuItemData(HMENU const, UINT const);
    export void      AddMenu(HMENU const, UINT const, UINT_PTR const, LPCWSTR const);
    export void      InsertMenuItem(HMENU const, UINT const, UINT_PTR const, wstring const&);
};
