// win32_tooltip.h : 
//
// Win32_utilities

#pragma once

#include <windows.h>
#include <windowsx.h>
#include "PixelTypes.h"

HWND CreateToolTipEx     (HWND const, int const, bool const, PixelRect * const, LPCTSTR const);
HWND CreateStdToolTip    (HWND const, int const, LPCTSTR const);
HWND CreateWindowToolTip (HWND const, LPCTSTR const);
HWND CreateBalloonToolTip(HWND const, int const, LPCTSTR const);
HWND CreateRectToolTip   (HWND const, int const, PixelRect * const, LPCTSTR const);
