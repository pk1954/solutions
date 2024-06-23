// Win32_Tooltip.cpp
//
// win32_utilities

module;

#include <cassert>
#include <string>
#include <memory>
#include <Windows.h>
#include <CommCtrl.h>

module Tooltip;

import Win32_PIXEL;

using std::wstring;
using std::unique_ptr;
using std::make_unique;

UP_TTIP ToolTip::CreateRectToolTip
(
	HWND      const  hwndParent,
	int       const  idTool,
	wstring   const& wstrText,
	PixelRect const& rect
)
{
	UP_TTIP upToolTip { createToolTip(hwndParent, idTool, true, wstrText) };
	upToolTip->Resize(rect);
	return upToolTip;
}

UP_TTIP ToolTip::CreateWindowToolTip
(
	HWND    const  hwndParent,
	wstring const& wstrText
)
{
	return createToolTip(hwndParent, 0, false, wstrText);
}

UP_TTIP ToolTip::CreateStdToolTip
(
	HWND    const  hwndParent,
	int     const  idTool,
	wstring const& wstrText
)
{
	return createToolTip(hwndParent, idTool, false, wstrText);
}

UP_TTIP ToolTip::CreateBalloonToolTip
(
	HWND    const  hwndParent,
	int     const  idTool,
	wstring const& wstrText
)
{
	return createToolTip(hwndParent, idTool, true, wstrText);
}

UP_TTIP ToolTip::createToolTip
(
	HWND    const  hwndParent,
	int     const  idTool,
	bool    const  bBalloon,
	wstring const& wstrText
)
{
	UP_TTIP upToolTip { make_unique<ToolTip>() };

	DWORD dwStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP; 
	if (bBalloon)
		dwStyle |= TTS_BALLOON;

	upToolTip->m_hwndToolTip = CreateWindowEx
	(
		WS_EX_TOPMOST,                   // ex style
		TOOLTIPS_CLASS,                  // class name - defined in commctrl.h
		NULL,                            // dummy text
		dwStyle,                         // style
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT,  
		hwndParent,                      // parent
		NULL,                            // ID
		GetModuleHandle(nullptr),        // instance
		NULL                             // no extra data
	);

	if (!upToolTip->m_hwndToolTip)
		return NULL;

	SetWindowPos
	(
		upToolTip->m_hwndToolTip, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
	);

	upToolTip->m_ti.cbSize   = sizeof(TOOLINFO_REDEF);
	upToolTip->m_ti.uFlags   = TTF_SUBCLASS;
	upToolTip->m_ti.hwnd     = hwndParent;
	upToolTip->m_ti.hinst    = GetModuleHandle(nullptr);
	upToolTip->m_ti.lpszText = wstrText.c_str();
	if (idTool > 0)
	{
		upToolTip->m_ti.uId = (UINT_PTR)GetDlgItem(hwndParent, idTool);
		upToolTip->m_ti.uFlags |= TTF_IDISHWND;
	}
	GetClientRect(upToolTip->m_ti.hwnd, static_cast<LPRECT>(&upToolTip->m_ti.rect));

	upToolTip->sendTTipMessage(TTM_ADDTOOL);
	SendMessage(upToolTip->m_hwndToolTip, TTM_SETMAXTIPWIDTH, 0, 200);

	return upToolTip;
}

void ToolTip::setRect(PixelRect const& rect)
{
    sendTTipMessage(TTM_GETTOOLINFO);
	m_ti.rect = PixelRect2RECT(rect);
	sendTTipMessage(TTM_NEWTOOLRECT);
}

LRESULT ToolTip::sendTTipMessage(UINT const msg, WPARAM const wParam)
{
	return SendMessage(m_hwndToolTip, msg, wParam, (LPARAM)&m_ti);
}

void ToolTip::SetDuration(MilliSecs const ms)
{
	SendMessage(m_hwndToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, ms.GetValue());
}

//void SetToolTipText(HWND const hwndToolTip, wstring const text)
//{
//	SendMessage(hwndToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&text.get);
//}