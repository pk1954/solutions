// win32_appMenu.cpp
//
// Evolution

#include "stdafx.h"
#include "WinUser.h"
#include "Resource.h"
#include "GridDimensions.h"
#include "win32_util.h"
#include "win32_winManager.h"
#include "win32_status.h"
#include "win32_appMenu.h"

AppMenu::AppMenu
( 
	HWND const hwndApp,
	WinManager * const pWinManager,
	StatusBar  * const pStatusBar
) :	m_hwndApp    ( hwndApp ),
	m_pWinManager( pWinManager ),
	m_pStatusBar ( pStatusBar )
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

    SendMessage( m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_EVOLUTION ) ) );
    SendMessage( m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL     ) ) );

	Util::SetApplicationTitle( m_hwndApp, IDS_APP_TITLE );

	SetMenu( m_hwndApp, LoadMenu( GetModuleHandle( nullptr ), MAKEINTRESOURCE( IDC_EVOLUTION_MAIN ) ) );

	m_hMenu = GetMenu( m_hwndApp );
}

void AppMenu::enableMenues( UINT state )
{
	HMENU hMenuApp = GetMenu( m_hwndApp );
	EnableMenuItem( hMenuApp, 1, state|MF_BYPOSITION ); 
	EnableMenuItem( hMenuApp, 2, state|MF_BYPOSITION ); 
}

void AppMenu::Start( )
{
	enableMenues( MF_ENABLED ); 

	if ( GridDimensions::GetNrOfNeigbors() == 6 )
		EnableMenuItem( m_hMenu, IDD_TOGGLE_STRIP_MODE, MF_GRAYED );  // strip mode looks ugly in heaxagon mode
}

void AppMenu::Stop( )
{
	enableMenues( MF_GRAYED ); 

}

void AppMenu::RunMode( BOOL const bMode )
{
	UINT state = bMode ? MF_GRAYED : MF_ENABLED;

	EnableMenuItem( m_hMenu, IDM_GENERATION,       state );
	EnableMenuItem( m_hMenu, IDM_RUN,              state );
	EnableMenuItem( m_hMenu, IDM_BACKWARDS,        state );
	EnableMenuItem( m_hMenu, IDM_GOTO_ORIGIN,      state );
	EnableMenuItem( m_hMenu, IDM_GOTO_DEATH,       state );
	EnableMenuItem( m_hMenu, IDM_RESET,            state );
	EnableMenuItem( m_hMenu, IDM_RESTART,          state );
	EnableMenuItem( m_hMenu, IDM_CHANGE_GRID_TYPE, state );

	m_pWinManager->Show( IDM_HIST_WINDOW, BoolOp( ! bMode ) );
	m_pStatusBar->SetRunMode( bMode );
}

