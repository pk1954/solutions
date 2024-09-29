// SelectionMenu.cpp
//
// NNetWindows

module NNetWin32:SelectionMenu;

import Win32_Util_Resource;
import Win32_Controls;
import WinBasics;
import NNetModel;
import Win32_Util;
import Direct2D;
import Resource;

void SelectionMenu::Start(HWND const hwndParent)
{
	PixelRect const pixRect(0_PIXEL, 0_PIXEL, 290_PIXEL, 230_PIXEL);

	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		L"ClassSelectionMenu",
		WS_CHILD | WS_CAPTION | WS_OVERLAPPED,
		&pixRect,
		nullptr
	);

	SetWindowText(L"Selection");

	m_hwndDeselect = CreateButton(hwnd, L"Deselect [ESC]", 10, 10, 250, 20, IDM_DESELECT);
	m_hwndCopy     = CreateButton(hwnd, L"Copy [Strg+C]",  10, 40, 250, 20, IDM_COPY_SELECTION);
	m_hwndDelete   = CreateButton(hwnd, L"Delete [Entf]",  10, 70, 250, 20, IDM_DELETE_SELECTION);
	
	CreateStaticField(hwnd, L" Move  [SHIFT+left mouse key]",       10, 100, 250, 20);
	CreateStaticField(hwnd, L" Rotate [SHIFT+CTRL+left mouse key]", 10, 130, 250, 20);
	CreateStaticField(hwnd, L" Size    [SHIFT+mouse wheel]",        10, 160, 250, 20);

	m_colBackground = Convert2COLORREF(NNetColors::INT_SELECTED);
}

bool SelectionMenu::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	return SendCommand2Parent(wParam, 0);
}

void SelectionMenu::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, m_colBackground);
	(void)EndPaint(&ps);
}
