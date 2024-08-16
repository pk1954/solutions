// Status.cpp
//
// Win32_utilities

module;

#include <Windows.h>
#include <CommCtrl.h>

module StatusBar;

import std;
import Tooltip;
import Types;
import RunTime;

using std::to_wstring;
using std::wstring;

static LRESULT __stdcall OwnerDrawStatusBar
(
	HWND      hwnd, 
	UINT      uMsg, 
	WPARAM    wParam, 
	LPARAM    lParam, 
	[[maybe_unused]] UINT_PTR  uIdSubclass, 
	DWORD_PTR dwRefData
)
{
	StatusBar const * const pStatusBar = (StatusBar *)dwRefData;
	switch (uMsg)
	{

	case WM_COMMAND:
		pStatusBar->PostCommand2Parent(LOWORD(wParam));
		return true;

	default: 
		break;
	}

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void StatusBar::Start(HWND const hwndParent)
{
	HWND hwndStatus = CreateWindow
	(
		STATUSCLASSNAME, 
		nullptr, 
		WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HEIGHT.GetValue(),
		hwndParent,
		nullptr, 
		GetModuleHandle(nullptr), 
		nullptr
	); 

	SetWindowHandle(hwndStatus);

	(void)SetWindowSubclass(hwndStatus, OwnerDrawStatusBar, 0, (DWORD_PTR)this);

	m_pixBorderX      = static_cast<PIXEL>(GetSystemMetrics(SM_CXSIZEFRAME)) + 10_PIXEL;
	m_pixBorderY      = static_cast<PIXEL>(GetSystemMetrics(SM_CYSIZEFRAME));
	m_pixClientHeight = HEIGHT - m_pixBorderY;
}

void StatusBar::AddCustomControl
(
	int const width  // in number of characters
)
{
	static auto const FONT_WIDTH = 9_PIXEL;
	m_pixPosX += FONT_WIDTH * width;
}

int StatusBar::NewPart()
{
	m_pixPosX += m_pixBorderX;
	m_statWidths.push_back(m_pixPosX);
	m_pixPosX += m_pixBorderX;
	return static_cast<int>(m_statWidths.size());
}

void StatusBar::LastPart()
{
	NewPart();
	m_statWidths.push_back(-1_PIXEL ); // Stop
	SendMsg(SB_SETPARTS, m_statWidths.size(), (LPARAM)(m_statWidths.data()));
}

LRESULT StatusBar::UserProc
(
	UINT   const uMsg, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	return DefSubclassProc(GetWindowHandle(), uMsg, wParam, lParam);
}

HWND StatusBar::addControl
(
    LPCTSTR const lpClassName,
    LPCTSTR const lpWindowName,
	int     const width,
    DWORD   const dwStyle,
    int     const iMenu
)
{
	int   const iWidth   { width ? width : static_cast<int>(wcslen(lpWindowName)) };
	PIXEL const pixWidth { PIXEL(iWidth * 9) };
    HWND  const hwnd     
	{ 
		CreateWindow
		(
			lpClassName,                     // class name 
			lpWindowName,                    // title (caption) 
			WS_CHILD | WS_VISIBLE | dwStyle, // style 
			m_pixPosX.GetValue(),            // x position
			m_pixBorderY.GetValue(),         // y position 
			pixWidth.GetValue(),             // width
			m_pixClientHeight.GetValue(),    // height
			GetWindowHandle(),               // parent window 
			(HMENU)(UINT_PTR)iMenu,          // control identifier 
			GetModuleHandle(nullptr),        // instance 
			nullptr                          // no WM_CREATE parameter 
		)
	};
	m_pixPosX += pixWidth;
	return hwnd;
}

HWND StatusBar::AddStaticControl(LPCTSTR const lpWindowName)
{
	HWND hwnd = addControl(WC_STATIC, lpWindowName, 0, 0, 0);
	return hwnd;
}

HWND StatusBar::AddStaticControl(int const width)
{
	HWND hwnd = addControl(WC_STATIC, L"", width, 0, 0);
	return hwnd;
}

HWND StatusBar::AddButton(LPCTSTR const lpWindowName, int const iMenu, DWORD const dwStyle)
{ 
	HWND hwnd = addControl(WC_BUTTON, lpWindowName, 0, dwStyle, iMenu);
	return hwnd;
}

HWND StatusBar::AddTrackBar(int const iMenu)
{ 
	HWND hwnd = addControl(TRACKBAR_CLASS, L"   Trackbar Control   ", 0, WS_TABSTOP|WS_BORDER|TBS_NOTICKS, iMenu);
	return hwnd;
};

void StatusBar::DisplayInPart(int const iPart, wstring const & wstrLine) const
{
	SendMsg(SB_SETTEXT, iPart, (LPARAM)(wstrLine.c_str()));
}

void StatusBar::ClearPart(int const iPart) const
{
	SendMsg(SB_SETTEXT, iPart, (LPARAM)L"");
}

void StatusBar::ReadProgressReport(int const iPart, Script * pScript) const
{
	long const lPercentage { pScript->GetPercentRead() };
	DisplayInPart
	(
		iPart, 
		L"Reading " +
		pScript->GetActPath() + 
		L" : " + 
		to_wstring(lPercentage) + 
		L'%'
	);
}

void StatusBar::WriteProgressReport(int const iPart, wstring const & msg) const
{
	DisplayInPart(iPart, L"Writing " + msg);
}

void StatusBar::Arrange
(
	RootWindow const& winParent,
	RootWindow const& winMain
)
{
	::ArrangeVertical(&winMain, this);
}
