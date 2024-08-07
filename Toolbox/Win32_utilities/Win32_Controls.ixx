// Win32_Controls.ixx
//
// Toolbox\Win32_utilities

module;

#include <Windows.h>

export module Win32_Controls;

export HWND CreateOwnerDrawButton(HWND const,                        int const, int const, int const, int const, INT_PTR const);
export HWND CreateButton         (HWND const, wchar_t const * const, int const, int const, int const, int const, INT_PTR const, DWORD const = 0);
export HWND CreateCheckBox       (HWND const, wchar_t const * const, int const, int const, int const, int const, INT_PTR const, DWORD const = 0);
export HWND CreateEditField      (HWND const,                        int const, int const, int const, int const);
export HWND CreateStaticField    (HWND const, wchar_t const * const, int const, int const, int const, int const, DWORD const = ES_LEFT);
