// SelectionMenu.ixx
//
// NNetWindows

export module NNetWin32:SelectionMenu;

import WinBasics;
import BaseWindow;

export class SelectionMenu : public BaseWindow
{
public:
	void Start(HWND const);

	void SetBackgroundColorRef(COLORREF const col) final
	{
		m_colBackground = col;
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

	COLORREF m_colBackground { MakeRGB(0,0,0) };

	void OnPaint() final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
};
