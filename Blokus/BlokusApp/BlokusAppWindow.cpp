// BlokusAppWindow.cpp
//
// Blokus

module BlokusAppWindow;

import std;
import WinBasics;
import Win32_Util_Resource;
import MessagePump;
import BlokusCore;

using std::wstring;

BlokusAppWindow::BlokusAppWindow(wstring const &wstrProductName, MessagePump &pump)
{
	m_pwstrProductName = &wstrProductName;
	m_aboutBox.SetProductName(wstrProductName);
	m_hwndApp = StartBaseWindow
	(
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
	m_mainWindow.Start(m_hwndApp);
	m_statusBar .Start(m_hwndApp);
	configureStatusBar();
	m_mainWindow.Show(true);
	m_statusBar .Show(true);
	Show(true);
}

bool BlokusAppWindow::OnSize(PIXEL const width, PIXEL const height)
{
	::ArrangeVertical(&m_mainWindow, &m_statusBar);
	return true;
}

bool BlokusAppWindow::UserProc
(
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	if (message == WM_ENTERMENULOOP)
	{
		//if (wParam == false)
		//	m_appMenu.Notify(true);

	}
	else if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	return BaseWindow::UserProc(message, wParam, lParam);
}

void BlokusAppWindow::configureStatusBar()
{
	int iPart = 0;
	m_ScriptHook.Initialize(& m_statusBar, iPart);
	m_statusBar.ClearPart(iPart);
	m_statusBarDispFunctor.Initialize(& m_statusBar, iPart);
	m_statusMessagePart = iPart;
	m_statusBar.LastPart();
	::ArrangeVertical(&m_mainWindow, &m_statusBar);
}

void BlokusAppWindow::OnClose()
{
	BaseWindow::OnClose();
}

void BlokusAppWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
}

bool BlokusAppWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LoWord(wParam);
	
	switch (wmId)
	{
	case IDM_ABOUT:
		m_aboutBox.Show(m_mainWindow.GetWindowHandle());
		return true;

	case IDM_EXIT:
		PostMsg(WM_CLOSE, 0, 0);
		return true;

	default:
		return true;
	}

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
