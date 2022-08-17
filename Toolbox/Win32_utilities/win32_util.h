// win32_util.h : 
//
// Toolbox\Win32_utilities

#pragma once

#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include "BasicTypes.h"

import BoolOp;
import NamedType;
import MoreTypes;
import PixelTypes;

using std::wostringstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;

namespace Util
{
    inline LONG_PTR GetUserDataPtr(HWND hwnd)
    { 
        return ::GetWindowLongPtr(hwnd, GWLP_USERDATA); 
    }

    inline void SetUserDataPtr(HWND hwnd, LONG_PTR ptr) 
    { 
        (void) ::SetWindowLongPtr(hwnd, GWLP_USERDATA, ptr); 
    }

    void SetEditField(HWND const hwndEditField, auto const value)
    {
        wostringstream m_wstrBuffer;
        m_wstrBuffer << value;
        ::SetWindowText(hwndEditField, m_wstrBuffer.str().c_str());
    }

    bool Evaluate(HWND const, wstring &);
    bool Evaluate(HWND const, float &);
    bool Evaluate(HWND const, unsigned long &);

    inline void SetText(HWND const hwnd, wchar_t const * const wstrText)
	{
		(void)::SendMessage(hwnd, WM_SETTEXT,	0, (LPARAM)(wstrText)	);
	}

    inline void Show(HWND const hwnd, bool const bStateOld, bool const bStateNew)
    {
        if (bStateNew != bStateOld)
            ShowWindow(hwnd, bStateNew ? SW_SHOW : SW_HIDE);
    }

    inline void Show(HWND const hwnd, bool const bStateNew)
    {
        Util::Show(hwnd, IsWindowVisible(hwnd), bStateNew);
    }

    inline bool Show(HWND const hwnd, tBoolOp const op)
    {
        bool const bStateOld = IsWindowVisible(hwnd);
        Util::Show(hwnd, ApplyOp2(bStateOld, op));
		return bStateOld;
    }

    inline void AddWindowStyle(HWND const hwnd, DWORD const dwStyle)
    {
        DWORD const dwOldStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        DWORD const dwNewStyle = dwOldStyle | dwStyle;
        SetWindowLong(hwnd, GWL_EXSTYLE, dwNewStyle);
    }

    inline void DeleteWindowStyle(HWND const hwnd, DWORD const dwStyle)
    {
        DWORD const dwOldStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        DWORD const dwNewStyle = dwOldStyle & ~dwStyle;
        LONG  const lRes       = SetWindowLong(hwnd, GWL_EXSTYLE, dwNewStyle);
        assert(lRes == dwNewStyle);
    }

    inline MONITORINFO GetMonitorInfo(HMONITOR hMonitor)
    {
        MONITORINFO monInfo;
        monInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &monInfo);
        return monInfo;
    }

	void MakeLayered(HWND const , bool const, COLORREF const, UINT const);

    DWORD     GetNrOfCPUs();
    ULONGLONG GetPhysicalMemory();
    wstring   GetCurrentDateAndTime();
    wstring   GetComputerName();
    wstring   GetUserName();
    wstring   GetCurrentDir();
    void      SetApplicationTitle(HWND const, int const, wstring const & = L"");
    void      StdOutConsole();
    bool      EscapeKeyPressed();
    HMENU     PopupMenu(HMENU const, LPCTSTR const);
    void      SetNotifyByPos(HMENU const);
    void      SetMenuItemData(HMENU const, UINT const, UINT_PTR const);
    UINT_PTR  GetMenuItemData(HMENU const, UINT const);
    void      AddMenu        (HMENU const, UINT const, UINT_PTR const, LPCWSTR const);
    void      InsertMenuItem (HMENU const, UINT const, UINT_PTR const, wstring const &);
};
