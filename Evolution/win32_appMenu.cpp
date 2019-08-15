// win32_appMenu.cpp
//
// Evolution

#include "stdafx.h"
#include "WinUser.h"
#include "config.h"
#include "resource.h"
#include "win32_util.h"
#include "win32_workThreadInterface.h"
#include "win32_winManager.h"
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

	EnableMenuItem( m_hMenu, IDM_GENERATION,       bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_BACKWARDS,        bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_GOTO_ORIGIN,      bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_GOTO_DEATH,       bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_RESET,            bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_CHANGE_GRID_TYPE, bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_RUN,              bRunning ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_STOP,             bRunning ? MF_ENABLED : MF_GRAYED );

	BOOL b = m_pWinManager->IsVisible( IDM_DISP_WINDOW );
	EnableMenuItem( m_hMenu, IDM_DISP_WINDOW, m_pWinManager->IsVisible( IDM_DISP_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_EDIT_WINDOW, m_pWinManager->IsVisible( IDM_EDIT_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_MAIN_WINDOW, m_pWinManager->IsVisible( IDM_MAIN_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_STAT_WINDOW, m_pWinManager->IsVisible( IDM_STAT_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_CRSR_WINDOW, m_pWinManager->IsVisible( IDM_CRSR_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_HIST_INFO  , m_pWinManager->IsVisible( IDM_HIST_INFO   ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_PERF_WINDOW, m_pWinManager->IsVisible( IDM_PERF_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_MINI_WINDOW, m_pWinManager->IsVisible( IDM_MINI_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_HIST_WINDOW, m_pWinManager->IsVisible( IDM_HIST_WINDOW ) ? MF_GRAYED : MF_ENABLED );
}
