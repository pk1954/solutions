// EvoAppMenu.cpp
//
// Evolution

export module EvoAppMenu;

import Resource;
import EvoCoreLib;
import WorkThreadInterface;
import WinBasics;
import WinManager;
import EvoAppMenu;

void EvoAppMenu::Initialize
(
	HWND                        const hwndApp, 
	WorkThreadInterface const * const pWworkThreadInterface,
	WinManager          const * const pWinManager
) 
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);

	m_pWorkThreadInterface = pWworkThreadInterface;
	m_pWinManager          = pWinManager;

    SendMessage(hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EVOLUTION)));
    SendMessage(hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL    )));

	Util::SetApplicationTitle(hwndApp, IDS_APP_TITLE);

	bool bRes = SetMenu(hwndApp, LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDC_EVOLUTION_MAIN)));
	Assert(bRes);

	m_hMenu = GetMenu(hwndApp);
}

void EvoAppMenu::enableMenues(UINT const state)
{
	EnableMenuItem(m_hMenu, 1, state|MF_BYPOSITION); 
	EnableMenuItem(m_hMenu, 2, state|MF_BYPOSITION); 
}

void EvoAppMenu::Start()
{
	enableMenues(MF_ENABLED); 

	if (GridDimensions::IsHexMode())
		EnableMenuItem(m_hMenu, IDD_TOGGLE_STRIP_MODE, MF_GRAYED);  // strip mode looks ugly in hexagon mode
}

void EvoAppMenu::Stop()
{
	enableMenues(MF_GRAYED); 
}

void EvoAppMenu::AdjustVisibility()
{
	bool const bRunning = m_pWorkThreadInterface->IsRunning();

	EnableMenuItem(m_hMenu, IDM_FORWARD,          bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_BACKWARDS,        bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_GOTO_ORIGIN,      bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_GOTO_DEATH,       bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_RESET,            bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_CHANGE_GRID_TYPE, bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_RUN,              bRunning ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_STOP,             bRunning ? MF_ENABLED : MF_GRAYED);

	bool b = m_pWinManager->IsVisible(IDM_DISP_WINDOW);
	EnableMenuItem(m_hMenu, IDM_DISP_WINDOW, m_pWinManager->IsVisible(IDM_DISP_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_EDIT_WINDOW, m_pWinManager->IsVisible(IDM_EDIT_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_MAIN_WINDOW, m_pWinManager->IsVisible(IDM_MAIN_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_STAT_WINDOW, m_pWinManager->IsVisible(IDM_STAT_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_CRSR_WINDOW, m_pWinManager->IsVisible(IDM_CRSR_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_HIST_INFO  , m_pWinManager->IsVisible(IDM_HIST_INFO  ) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_PERF_WINDOW, m_pWinManager->IsVisible(IDM_PERF_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_MINI_WINDOW, m_pWinManager->IsVisible(IDM_MINI_WINDOW) ? MF_GRAYED : MF_ENABLED);
	EnableMenuItem(m_hMenu, IDM_HIST_WINDOW, m_pWinManager->IsVisible(IDM_HIST_WINDOW) ? MF_GRAYED : MF_ENABLED);
}
