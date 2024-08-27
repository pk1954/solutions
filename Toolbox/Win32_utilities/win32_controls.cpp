// Win32_Controls.cpp
//
// Toolbox\Win32_utilities

module Win32_Controls;

import WinBasics;

HWND CreateButton
(
	HWND            const hwndParent, 
	wchar_t const * const text, 
	int             const x, 
	int             const y, 
	int             const w, 
	int             const h, 
	INT_PTR         const id, 
	DWORD           const dwStyle
)
{
	DWORD style { BS_DEFPUSHBUTTON|WS_TABSTOP|WS_CHILD|WS_VISIBLE|dwStyle };
	HWND  hwnd  { 
					CreateWindowExW
					(
						0,
						WC_BUTTON, 
						text, 
						style, 
						x, y, w, h, 
						hwndParent, 
						reinterpret_cast<HMENU>(id), 
						GetModuleHandleW(nullptr), 
						0
					) 
				};
	return hwnd;
}

HWND CreateCheckBox
(
	HWND            const hwndParent, 
	wchar_t const * const text, 
	int             const x, 
	int             const y, 
	int             const w, 
	int             const h, 
	INT_PTR         const id, 
	DWORD           const dwStyle
)
{
	DWORD style { BS_AUTOCHECKBOX|WS_TABSTOP|WS_CHILD|WS_VISIBLE|dwStyle };
	HWND  hwnd  { 
					CreateWindowExW
					(
						0, 
						WC_BUTTON,
						text,
						style, 
						x, y, w, h, 
						hwndParent, 
						reinterpret_cast<HMENU>(id), 
						GetModuleHandleW(nullptr), 
						0
					) 
				};
	return hwnd;
}

HWND CreateOwnerDrawButton(HWND const hwndParent, int const x, int const y, int const w, int const h, INT_PTR const id)
{
	HWND hwnd  
	{ 
		CreateWindowExW
		(
			0, 
			WC_BUTTON, 
			0,
			WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, 
			x, y, w, h, 
			hwndParent, 
			reinterpret_cast<HMENU>(id), 
			GetModuleHandleW(nullptr), 
			0
		) 
	};
	return hwnd;
}

HWND CreateStaticField
(
	HWND            const hwndParent, 
	wchar_t const * const text, 
	int             const iXpos, 
	int             const iYpos, 
	int             const iWidth, 
	int             const height,
	DWORD           const dwStyle
)
{
	HWND hwnd = CreateWindowExW(0, WC_STATIC, text, WS_CHILD|WS_VISIBLE|dwStyle, iXpos, iYpos, iWidth, height, hwndParent, 0, GetModuleHandleW(nullptr), 0);
	return hwnd;
}

HWND CreateEditField(HWND const hwndParent, int const iXpos, int const iYpos, int const iWidth, int const height)
{
	HWND hwnd = CreateWindowExW(0, WC_EDIT, 0, WS_CHILD|WS_VISIBLE|ES_RIGHT, iXpos, iYpos, iWidth, height, hwndParent, 0, GetModuleHandleW(nullptr), 0);
	return hwnd;
}
