// NNetViewerWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <vector>
#include <Windows.h>

module NNetViewerWindow;

import Win32_Util_Resource;
import Win32_Util;
import IoUtil;
import PanelPlatform;

using std::unique_ptr;
using std::wstring;
using std::vector;

NNetViewerWindow::NNetViewerWindow()
{
	HWND hwnd = StartBaseWindow
	(
		nullptr,
		CS_HREDRAW | CS_VREDRAW,
		L"ClassScanViewerWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	::SetApplicationTitle(hwnd, L"Scan viewer");

	m_statusBar.Start(hwnd);
	configureStatusBar();
	m_statusBar.Arrange(*this, m_panelPlatform);

	Show(true);
}

NNetViewerWindow::~NNetViewerWindow() = default;

void NNetViewerWindow::configureStatusBar()
{
	int iPart = 0;
	m_statusBarDispFunctor.Initialize(& m_statusBar, iPart);
	m_statusMessagePart = iPart;
	m_statusBar.LastPart();
}

bool NNetViewerWindow::UserProc
(
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	//switch (message)
	//{

	//default:
	//	break;
	//}

	return BaseWindow::UserProc(message, wParam, lParam);
}

bool NNetViewerWindow::OnSize(PIXEL const width, PIXEL const height)
{
	m_statusBar.Arrange(*this, m_panelPlatform);
	return true;
}

void NNetViewerWindow::OnClose()
{
	Stop();
}

void NNetViewerWindow::OnNotify(WPARAM const wParam, LPARAM const lParam)
{
}

void NNetViewerWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
}

bool NNetViewerWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	switch (wmId)
	{
	case IDX_FILE_NOT_FOUND:  //no user command, only internal usage
		MessageBox(nullptr, L"Could not find scan file", L"Error", MB_OK);
		return true;

	case IDX_ERROR_IN_FILE:  //no user command, only internal usage
		MessageBox(nullptr, L"Error reading scan file\r\nSee main_trace.out for details", L"Error", MB_OK);
		return true;

	default:
		return true;
	}

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

