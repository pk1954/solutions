// NNetViewerWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <Windows.h>

module NNetViewerWindow;

import Win32_Util_Resource;
import IoUtil;

using std::wstring;

NNetViewerWindow::~NNetViewerWindow() = default;

void NNetViewerWindow::Start()
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

	m_statusBar.Start(hwnd);
	Show(true);
}

bool NNetViewerWindow::OnSize(PIXEL const width, PIXEL const height)
{
	return true;
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

