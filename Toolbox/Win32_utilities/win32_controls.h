// win32_controls.h
//
// NNetWindows

#pragma once

#include "Windows.h"

HWND CreateOwnerDrawButton(HWND const,                        int const, int const, int const, int const, INT_PTR const);
HWND CreateButton         (HWND const, wchar_t const * const, int const, int const, int const, int const, INT_PTR const, DWORD const = 0);
HWND CreateEditField      (HWND const,                        int const, int const, int const, int const);
HWND CreateStaticField    (HWND const, wchar_t const * const, int const, int const, int const, int const);
