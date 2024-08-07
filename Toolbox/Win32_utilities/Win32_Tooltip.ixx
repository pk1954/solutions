// Win32_Tooltip.ixx
//
// Win32_utilities

module;

#include <Windows.h>
#include <windowsx.h>

export module Tooltip;

import std;
import WinBasics;

class ToolTip;

using std::wstring;
using std::unique_ptr;

export using UP_TTIP = unique_ptr<ToolTip>;

import Types;

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
	LPCTSTR   lpszText;
	LPARAM    lParam;
//	void *lpReserved;
};

export class ToolTip
{
public:

static UP_TTIP CreateStdToolTip    (HWND const, int const, wstring const&);
static UP_TTIP CreateWindowToolTip (HWND const,            wstring const&);
static UP_TTIP CreateBalloonToolTip(HWND const, int const, wstring const&);
static UP_TTIP CreateRectToolTip   (HWND const, int const, wstring const&, PixelRect const&);

void Resize(PixelRect const& rect) { setRect(rect); }
void Resize()                      { setRect(GetClPixelRect(m_ti.hwnd)); }

void SetDuration(MilliSecs const);

private:
	HWND           m_hwndToolTip { 0 };
	TOOLINFO_REDEF m_ti          { 0 };

	static UP_TTIP createToolTip(HWND const, int const, bool const, wstring const&);

	void    setRect(PixelRect const&);
	LRESULT sendTTipMessage(UINT const, WPARAM const = 0);
};