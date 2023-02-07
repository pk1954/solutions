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

private:
	HWND m_hwndParent   { nullptr };
	HWND m_hwndDeselect { nullptr };
	HWND m_hwndCopy     { nullptr };
	HWND m_hwndDelete   { nullptr };

	void OnPaint() final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
};
