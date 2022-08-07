// Win32_Tooltip.ixx
//
// Win32_utilities

module;

#include <Windows.h>
#include <windowsx.h>
#include "PixelTypes.h"

export module Tooltip;

export HWND CreateToolTipEx     (HWND const, int const, bool const, PixelRect const * const, LPCTSTR const);
export HWND CreateStdToolTip    (HWND const, int const, LPCTSTR const);
export HWND CreateWindowToolTip (HWND const, LPCTSTR const);
export HWND CreateBalloonToolTip(HWND const, int const, LPCTSTR const);
export HWND CreateRectToolTip   (HWND const, int const, PixelRect const * const, LPCTSTR const);
