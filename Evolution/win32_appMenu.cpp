// win32_appMenu.cpp
//
// Evolution

#include "stdafx.h"
#include "WinUser.h"
#include "config.h"
#include "resource.h"
#include "win32_util.h"
#include "win32_util_menus.h"
#include "win32_workThreadInterface.h"
#include "win32_winManager.h"
#include "win32_menus.h"
#include "win32_appMenu.h"

void AppMenu::Initialize
( 
	HWND                        const hwndApp, 
	WorkThreadInterface const * const pWworkThreadInterface,
	WinManager          const * const pWinManager
) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_pWorkThreadInterface = pWworkThreadInterface;
	m_pWinManager          = pWinManager;

    SendMessage( hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_EVOLUTION ) ) );
    SendMessage( hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL     ) ) );

	Util::SetApplicationTitle( hwndApp, IDS_APP_TITLE );

	BOOL bRes = SetMenu( hwndApp, LoadMenu( GetModuleHandle( nullptr ), MAKEINTRESOURCE( IDC_EVOLUTION_MAIN ) ) );
	assert( bRes );

	m_hMenu = GetMenu( hwndApp );
	HMENU hMenuView  = GetSubMenu( m_hMenu, 2 );
	HMENU hMenuWindows = GetSubMenu( hMenuView, 1 );

	//AddMiniWinMenu( hMenuWindows );
	//m_pWinManager->GetBaseWindow( IDM_HIST_WINDOW )->AddWinMenu( hMenuWindows, L"Hist window" );
	//Util::AddPerfWinMenu( hMenuWindows );
}

void AppMenu::enableMenues( UINT const state )
{
	EnableMenuItem( m_hMenu, 1, state|MF_BYPOSITION ); 
	EnableMenuItem( m_hMenu, 2, state|MF_BYPOSITION ); 
}

void AppMenu::Start( BOOL const bHexMode )
{
	enableMenues( MF_ENABLED ); 

	if ( bHexMode )
		EnableMenuItem( m_hMenu, IDD_TOGGLE_STRIP_MODE, MF_GRAYED );  // strip mode looks ugly in hexagon mode
}

void AppMenu::Stop( )
{
	enableMenues( MF_GRAYED ); 
}

void AppMenu::AdjustVisibility( )
{
	BOOL const bRunning = m_pWorkThreadInterface->IsRunning();
	UINT const state    = bRunning ? MF_GRAYED : MF_ENABLED;

	EnableMenuItem( m_hMenu, IDM_GENERATION,       state );
	EnableMenuItem( m_hMenu, IDM_BACKWARDS,        state );
	EnableMenuItem( m_hMenu, IDM_GOTO_ORIGIN,      state );
	EnableMenuItem( m_hMenu, IDM_GOTO_DEATH,       state );
	EnableMenuItem( m_hMenu, IDM_RESET,            state );
	EnableMenuItem( m_hMenu, IDM_CHANGE_GRID_TYPE, state );
	EnableMenuItem( m_hMenu, IDM_RUN,              state );
	EnableMenuItem( m_hMenu, IDM_STOP,             bRunning ? MF_ENABLED : MF_GRAYED );

	disableIfVisible( IDM_DISP_WINDOW );
	disableIfVisible( IDM_EDIT_WINDOW );
	disableIfVisible( IDM_MAIN_WINDOW );
	disableIfVisible( IDM_STAT_WINDOW );
	disableIfVisible( IDM_CRSR_WINDOW );
	disableIfVisible( IDM_HIST_INFO   );
	disableIfVisible( IDM_PERF_WINDOW );
	disableIfVisible( IDM_MINI_WINDOW );
	disableIfVisible( IDM_HIST_WINDOW );

	AdjustMiniWinMenu( m_hMenu );
	AdjustPerfWinMenu( m_hMenu );
	m_pWinManager->GetBaseWindow( IDM_HIST_WINDOW )->AdjustWinMenu( m_hMenu );
}
