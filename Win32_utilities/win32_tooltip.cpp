// win32_tooltip.cpp
//
// win32_utilities

#include "stdafx.h"
#include <assert.h>
#include <windows.h>
#include <commctrl.h>
#include "win32_util.h"
#include "win32_tooltip.h"

HWND CreateRectToolTip
(
	HWND        const hwndParent,
	int         const idTool,
	PixelRect * const pRect,
	LPWSTR      const szText
)
{
	return CreateToolTipEx( hwndParent, idTool, TRUE, pRect, szText );
}

HWND CreateWindowToolTip
(
	HWND   const hwndParent,
	LPWSTR const szText
)
{
	return CreateToolTipEx( hwndParent, 0, FALSE, nullptr, szText );
}

HWND CreateStdToolTip
(
	HWND   const hwndParent,
	int    const idTool,
	LPWSTR const szText
)
{
	return CreateToolTipEx( hwndParent, idTool, FALSE, nullptr, szText );
}

HWND CreateBalloonToolTip
( 
	HWND   const hwndParent,
	int    const idTool,
	LPWSTR const szText
)
{
	return CreateToolTipEx( hwndParent, idTool, TRUE, nullptr, szText );
}

HWND CreateToolTipEx
( 
	HWND        const hwndParent,
	int         const idTool,
	BOOL        const bBalloon,
	PixelRect * const pRect,
	LPWSTR      const szText
)
{
	DWORD dwStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP; 
	if ( bBalloon )
		dwStyle |= TTS_BALLOON;

	HWND hwndTip  = CreateWindowEx
	(
		WS_EX_TOPMOST,                   // ex style
		TOOLTIPS_CLASS,                  // class name - defined in commctrl.h
		NULL,                            // dummy text
		dwStyle,                         // style
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT,  
		hwndParent,                      // parent
		NULL,                            // ID
		GetModuleHandle( nullptr ),      // instance
		NULL                             // no extra data
	);

	if (!hwndTip)
		return NULL;

	// Die Struktur TOOLINFO ist in CommCtrl.h definiert
	// Das Element lpReserved ist ab WIN_NTK Version 5.1 hinzugekommen,
	// sollte also zu Win10-Zeiten längst in der DLL vorhanden sein
	// Aus irgendeinem Grund funktioniert es aber nur mit der alten Version
	// Deshalb habe ich die Struktur kopiert und lpReserved auskommentiert
	// Vielleicht geht es mit einer neueren SDK-Version auch ohne diesen Trick

	struct TOOLINFO_REDEF
	{
		UINT      cbSize;
		UINT      uFlags;
		HWND      hwnd;
		UINT_PTR  uId;
		RECT      rect;
		HINSTANCE hinst;
		LPWSTR    lpszText;
		LPARAM    lParam;
//		void *lpReserved;
	};

	SetWindowPos
	(
		hwndTip, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
	);

	TOOLINFO_REDEF ti = { 0 };

	if ( pRect )
		ti.rect = Util::PixelRect2RECT( * pRect );
	else
		GetClientRect( hwndParent, &ti.rect );

	ti.cbSize   = sizeof(ti);
	ti.uFlags   = TTF_SUBCLASS;
	ti.hwnd     = hwndParent;
	ti.hinst    = GetModuleHandle( nullptr );
	ti.lpszText = szText;

	if ( idTool > 0 )
	{
		ti.uId = (UINT_PTR)GetDlgItem( hwndParent, idTool );
		ti.uFlags |= TTF_IDISHWND;
	}

	LRESULT res = SendMessage( hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti );
	assert( res > 0 );

	SendMessage( hwndTip, TTM_SETMAXTIPWIDTH, 0, 100 );

	return hwndTip;
}
