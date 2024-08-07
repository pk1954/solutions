// WinBasics.ixx
//
// Toolbox\WinBasics

module;

#include <Windows.h>

export module WinBasics;

export import :Win32_PIXEL;

export using HWND    = HWND;
export using LRESULT = LRESULT;
export using WPARAM  = WPARAM;
export using LPARAM  = LPARAM;
export using UINT    = UINT;

export using ::IsWindowVisible;
