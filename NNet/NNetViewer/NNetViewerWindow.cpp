// NNetViewerWindow.ixx
//
// NNetViewer

module;

#include <memory>
#include <string>
#include <vector>
#include <Windows.h>

module NNetViewerWindow;

import Win32_Util_Resource;
import Win32_Util;
import ScriptFile;
import ScriptHook;
import IoUtil;
import NNetWin32;
import PanelPlatform;

using std::unique_ptr;
using std::make_unique;
using std::wstring;
using std::vector;

static int const IDM_ADD_SCAN          { 1024 };
static int const IDM_ADD_IMPORTED_SCAN { 1025 };

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

	m_upPanelPlatform = make_unique<PanelPlatform>(hwnd);

	m_statusBar.Start(hwnd);
	configureStatusBar();
	m_statusBar.Arrange(*this, *m_upPanelPlatform.get());

	Show(true);
}

NNetViewerWindow::~NNetViewerWindow() = default;

void NNetViewerWindow::configureStatusBar()
{
	int iPart = 0;
	HWND hwndAddButton = m_statusBar.AddButton(L" Add scan ", IDM_ADD_SCAN, BS_PUSHBUTTON);

	iPart = m_statusBar.NewPart();
	m_ScriptHook.Initialize(& m_statusBar, iPart);
	m_statusBar.ClearPart(iPart);
	m_statusBarDispFunctor.Initialize(& m_statusBar, iPart);
	m_statusMessagePart = iPart;

	m_statusBar.LastPart();
	m_statusBar.Show(true);
}

bool NNetViewerWindow::OnSize(PIXEL const width, PIXEL const height)
{
	m_statusBar.Arrange(*this, *m_upPanelPlatform.get());
	return true;
}

bool NNetViewerWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	switch (wmId)
	{
	case IDM_ADD_SCAN:
		NNetModelIO::Import
		(
			ScriptFile::AskForFileName(L"scan", L"Scan files", tFileMode::read), 
			NNetInputOutputUI::CreateNew(IDM_ADD_IMPORTED_SCAN, this)
		);
		return true;

	case IDM_ADD_IMPORTED_SCAN:
		{
	        unique_ptr<Model> upModel {	NNetModelIO::GetImportedModel() };
			m_upPanelPlatform->AddScan(move(upModel));
		}
		return true;

	case IDX_FILE_NOT_FOUND:  //no user command, only internal usage
		MessageBox(nullptr, L"Could not find scan file", L"Error", MB_OK);
		return true;

	case IDX_ERROR_IN_FILE:  //no user command, only internal usage
		MessageBox(nullptr, L"Error reading scan file\r\nSee main_trace.out for details", L"Error", MB_OK);
		return true;

	default:
		break;
	}

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

