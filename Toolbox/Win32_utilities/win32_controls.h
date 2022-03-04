// win32_controls.h
//
// NNetWindows

#pragma once

#include "Windows.h"

HWND CreateButton     (HWND const, wchar_t  const * const, int const, int const, int const, int const, int const);
HWND CreateEditField  (HWND const,                         int const, int const, int const, int const);
HWND CreateStaticField(HWND const, wchar_t  const * const, int const, int const, int const, int const);
