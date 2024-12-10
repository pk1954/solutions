// BlokusAppWindow.cpp
//
// BlokusApp

module BlokusAppWindow;

import IoUtil;
import Win32_Util_Resource;
import WinManager;
import Resource;

using std::wstring;

BlokusAppWindow::BlokusAppWindow(wstring const &wstrProductName, MessagePump &pump)
{
	m_pwstrProductName = &wstrProductName;
	m_aboutBox.SetProductName(wstrProductName);

	WinManager::Initialize();
	WinCommand::Initialize(&m_cmdStack);
	m_cmdStack.Initialize(&m_matchObservable);

	m_hwndApp   = StartBaseWindow(  nullptr, L"ClassAppWindow",         WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, nullptr, nullptr);
	m_tournamentWindow.Initialize(m_hwndApp, L"ClasseTournamentWindow", WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CAPTION| WS_SIZEBOX);
	m_mainWindow      .Start(m_hwndApp, m_sound);
	m_statusBar       .Start(m_hwndApp);
	m_appMenu         .Start(m_hwndApp);
	m_undoRedoMenu    .Start(m_hwndApp, &m_cmdStack);
	m_tournamentWindow.Start(&m_tournament);

	WinManager::AddWindow(L"IDM_APPL_WINDOW",       RootWinId(IDM_APPL_WINDOW      ), m_hwndApp,                     true,  true );
	WinManager::AddWindow(L"IDM_STATUS_BAR",        RootWinId(IDM_STATUS_BAR       ), m_statusBar.GetWindowHandle(), false, false);
	WinManager::AddWindow(L"IDM_MAIN_WINDOW",       RootWinId(IDM_MAIN_WINDOW      ), m_mainWindow,                  true,  false);
	WinManager::AddWindow(L"IDM_TOURNAMENT_WINDOW", RootWinId(IDM_TOURNAMENT_WINDOW), m_hwndApp,                     true,  true );

	if (! WinManager::GetWindowConfiguration())
		::Show(m_hwndApp, true);

	BlokusPreferences::m_bShowContactPnts.RegisterObserver(m_mainWindow);
	BlokusPreferences::m_bShowCornerCells.RegisterObserver(m_mainWindow);
	BlokusPreferences::m_bShowAnimation  .RegisterObserver(m_mainWindow);
	Preferences::m_bSound                .RegisterObserver(m_appMenu);
	m_tournament                         .RegisterObserver(m_tournamentWindow);
	m_matchObservable                    .RegisterObserver(m_mainWindow);
	m_matchObservable                    .RegisterObserver(m_undoRedoMenu);
	configureStatusBar();

	m_tournamentWindow.Move(PixelRect{ 200_PIXEL, 0_PIXEL, 550_PIXEL, 250_PIXEL }, true);

	m_mainWindow.Show(true);
	m_statusBar .Show(true);

	m_appMenu.Notify(true);
	Show(true);
	Preferences::ReadPreferences();
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
		if (wParam == false)
			m_appMenu.Notify(true);
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
	WinManager::StoreWindowConfiguration();
	BaseWindow::OnClose();
}

void BlokusAppWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
	m_mainWindow.OnChar(wParam, lParam);
}

bool BlokusAppWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LoWord(wParam);
	
	switch (wmId)
	{
	case IDM_ABOUT:
		m_aboutBox.Show(m_mainWindow.GetWindowHandle());
		return true;

	case IDM_UNDO:
		if (!m_cmdStack.UndoStackCommand())
			m_sound.WarningSound();
		return true;

	case IDM_REDO:
		if (!m_cmdStack.RedoStackCommand())
			m_sound.WarningSound();
		return true;

	case IDD_CONTACT_PNTS:
		BlokusPreferences::m_bShowContactPnts.Toggle();
		Preferences::WritePreferences();
		break;

	case IDD_CORNER_CELLS:
		BlokusPreferences::m_bShowCornerCells.Toggle();
		Preferences::WritePreferences();
		break;

	case IDD_ANIMATION:
		BlokusPreferences::m_bShowAnimation.Toggle();
		Preferences::WritePreferences();
		break;

	case IDD_START_TOURNAMENT:
		m_tournamentWindow.Show(true);
		m_tournamentWindow.BringWindowToTop();
		m_tournament.Start(100);
		break;

	case IDD_SOUND:
		Preferences::m_bSound.Toggle();
		break;

	case IDM_EXIT:
		PostMsg(WM_CLOSE, 0, 0);
		return true;

	default:
		return true;
	}

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
