// SelectionMenu.ixx
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>

export module NNetWin32:SelectionMenu;

import BaseWindow;

export class SelectionMenu : public BaseWindow
{
public:
	void Start(HWND const);
	void Stop();

	COLORREF SetBackgroundColorRef(COLORREF const col) final
	{
		COLORREF const colOld { m_colBackground };
		m_colBackground = col;
		return colOld;
	}

	COLORREF GetBackgroundColorRef() const final 
	{ 
		return m_colBackground; 
	}

private:
	HWND m_hwndParent   { nullptr };
	HWND m_hwndDeselect { nullptr };
	HWND m_hwndCopy     { nullptr };
	HWND m_hwndDelete   { nullptr };

	COLORREF m_colBackground { RGB(0,0,0) };

	void OnPaint() final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
};
