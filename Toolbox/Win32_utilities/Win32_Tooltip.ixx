// Win32_Tooltip.ixx
//
// Win32_utilities

module;

#include <string>
#include <Windows.h>
#include <windowsx.h>

export module Tooltip;

import Types;

using std::wstring;

export HWND CreateToolTipEx     (HWND const, int const, bool const, PixelRect const * const, wstring const);
export HWND CreateStdToolTip    (HWND const, int const, wstring const);
export HWND CreateWindowToolTip (HWND const,            wstring const);
export HWND CreateBalloonToolTip(HWND const, int const, wstring const);
export HWND CreateRectToolTip   (HWND const, int const, PixelRect const * const, wstring const);
