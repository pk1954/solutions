// win32_tooltip.h : 
//

#pragma once

#include <windows.h>
#include <windowsx.h>
#include "PixelTypes.h"

HWND CreateToolTipEx     ( HWND const, int const, BOOL const, PixelRect * const, LPCTSTR const );
HWND CreateStdToolTip    ( HWND const, int const, LPCTSTR const );
HWND CreateWindowToolTip ( HWND const, LPCTSTR const );
HWND CreateBalloonToolTip( HWND const, int const, LPCTSTR const );
HWND CreateRectToolTip   ( HWND const, int const, PixelRect * const, LPCTSTR const );
