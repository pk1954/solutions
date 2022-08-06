// Win32_Controls.cpp
//
// Toolbox\Win32_utilities

module;

#include "Windows.h"
#include "CommCtrl.h"

module Win32_Controls;

HWND CreateButton(HWND const hwndParent, wchar_t const * const text, int const x, int const y, int const w, int const h, INT_PTR const id, DWORD const dwStyle)
{
	DWORD style { BS_DEFPUSHBUTTON|WS_TABSTOP|WS_CHILD|WS_VISIBLE|dwStyle };
	HWND  hwnd  { 
		CreateWindow
		(
			WC_BUTTON, 
			text, 
			style, 
			x, y, w, h, 
			hwndParent, 
			reinterpret_cast<HMENU>(id), 
			GetModuleHandle(nullptr), 
			0
		) 
	};
	return hwnd;
}

HWND CreateOwnerDrawButton(HWND const hwndParent, int const x, int const y, int const w, int const h, INT_PTR const id)
{
	HWND hwnd  
	{ 
		CreateWindow
		(
			WC_BUTTON, 
			0,
			WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, 
			x, y, w, h, 
			hwndParent, 
			reinterpret_cast<HMENU>(id), 
			GetModuleHandle(nullptr), 
			0
		) 
	};
	return hwnd;
}

HWND CreateStaticField(HWND const hwndParent, wchar_t const * const text, int const iXpos, int const iYpos, int const iWidth, int const height)
{
	HWND hwnd = CreateWindow(WC_STATIC, text, WS_CHILD | WS_VISIBLE, iXpos, iYpos, iWidth, height, hwndParent, 0, GetModuleHandle(nullptr), 0);
	return hwnd;
}

HWND CreateEditField(HWND const hwndParent, int const iXpos, int const iYpos, int const iWidth, int const height)
{
	HWND hwnd = CreateWindow(WC_EDIT, 0, WS_CHILD|WS_VISIBLE|ES_RIGHT, iXpos, iYpos, iWidth, height, hwndParent, 0, GetModuleHandle(nullptr), 0);
	return hwnd;
}
