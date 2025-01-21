// Win32_Controls.ixx
//
// Toolbox\Win32_utilities

export module Win32_Controls;

import WinBasics;

export HWND CreateDropDownList   (HWND const,                        int const, int const, int const, int const, INT_PTR const, DWORD const);
export HWND CreateOwnerDrawButton(HWND const,                        int const, int const, int const, int const, INT_PTR const);
export HWND CreateButton         (HWND const, wchar_t const * const, int const, int const, int const, int const, INT_PTR const, DWORD const = 0);
export HWND CreateCheckBox       (HWND const, wchar_t const * const, int const, int const, int const, int const, INT_PTR const, DWORD const = 0);
export HWND CreateEditField      (HWND const,                        int const, int const, int const, int const);
export HWND CreateStaticField    (HWND const, wchar_t const * const, int const, int const, int const, int const, DWORD const = ES_LEFT);
