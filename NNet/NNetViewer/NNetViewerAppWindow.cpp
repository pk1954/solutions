// NNetViewerAppWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <Windows.h>

module NNetViewerAppWindow;

import Win32_Util_Resource;
import IoUtil;
import WinManager;

using std::wstring;

NNetViewerAppWindow::NNetViewerAppWindow(wstring const & wstrProductName)
{
	m_aboutBox.SetProductName(wstrProductName);
	DefineUtilityWrapperFunctions();

	WinManager::Initialize();
};

NNetViewerAppWindow::~NNetViewerAppWindow() = default;

void NNetViewerAppWindow::Start(MessagePump& pump)
{
	m_hwndApp = StartBaseWindow
	(
		nullptr,
		CS_HREDRAW | CS_VREDRAW,
		L"ClassNNetViewerAppWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	m_statusBar.Start(m_hwndApp);
}

void NNetViewerAppWindow::Stop()
{
	BaseWindow::Stop();
	WinManager::RemoveAll();
}

bool NNetViewerAppWindow::OnSize(PIXEL const width, PIXEL const height)
{
	return true;
}

bool NNetViewerAppWindow::UserProc
(
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	switch (message)
	{

	case WM_ENTERMENULOOP:
		if (wParam == false)
			m_appMenu.Notify(true);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return BaseWindow::UserProc(message, wParam, lParam);
}

void NNetViewerAppWindow::OnClose()
{
	Stop();
}

void NNetViewerAppWindow::OnNotify(WPARAM const wParam, LPARAM const lParam)
{
}

void NNetViewerAppWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
}

bool NNetViewerAppWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	switch (wmId)
	{
	case 0:             // unclear origin
		return true;

	case IDM_ABOUT:
		m_aboutBox.Show(GetWindowHandle());
		return true;

	case IDM_DOCU:
		ShellExecute(0, 0, L"https://nnetsimu.miraheze.org/wiki/Main_Page ", 0, 0, SW_SHOW);
		return true;

	case IDM_EXIT:
		PostMessage(WM_CLOSE, 0, 0);
		return true;

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

