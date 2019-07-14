// win32_tooltip.h : 
//

#pragma once

#include <windows.h>
#include <windowsx.h>
#include "PixelTypes.h"

HWND CreateToolTipEx     ( HWND const, int const, BOOL const, PixelRect * const, LPWSTR const );
HWND CreateStdToolTip    ( HWND const, int const, LPWSTR const );
HWND CreateBalloonToolTip( HWND const, int const, LPWSTR const );
HWND CreateRectToolTip   ( HWND const, int const, PixelRect * const, LPWSTR const );
