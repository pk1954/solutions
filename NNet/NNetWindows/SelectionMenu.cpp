// SelectionMenu.cpp
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>
#include "Resource.h"

module SelectionMenu;

import Win32_Controls;
import NNetModel;
import Win32_Util;

void SelectionMenu::Start(HWND const hwndParent)
{
	PixelRect const pixRect(0_PIXEL, 0_PIXEL, 200_PIXEL, 140_PIXEL);

	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_DROPSHADOW,
		L"ClassSelectionMenu",
		WS_CHILD | WS_CAPTION | WS_OVERLAPPED,
		&pixRect,
		nullptr
	);

	SetWindowText(L"Selection");

	m_hwndParent = hwndParent;

	m_hwndDeselect = CreateButton(hwnd, L"Deselect [ESC]", 10, 10, 160, 20, IDM_DESELECT);
	m_hwndCopy     = CreateButton(hwnd, L"Copy [Strg+C]",  10, 40, 160, 20, IDM_COPY_SELECTION);
	m_hwndDelete   = CreateButton(hwnd, L"Delete [Entf]",  10, 70, 160, 20, IDM_DELETE_SELECTION);
}

void SelectionMenu::Stop()
{
	DestroyWindow();
}

bool SelectionMenu::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	return ::SendMessage(m_hwndParent, WM_COMMAND, wParam, 0);
}

void SelectionMenu::OnPaint()
{
	static auto const CLR_BACKGROUND { Util::GetColorRef(NNetColors::INT_SELECTED) };
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, CLR_BACKGROUND);
	(void)EndPaint(&ps);
}
