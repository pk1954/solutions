// win32_appMenu.cpp
//
// Evolution

#include "stdafx.h"
#include "WinUser.h"
#include "config.h"
#include "Resource.h"
#include "win32_util.h"
#include "win32_workThreadInterface.h"
#include "win32_appMenu.h"

void AppMenu::Initialize( HWND const hwndApp, WorkThreadInterface const * const pWworkThreadInterface ) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_pWorkThreadInterface = pWworkThreadInterface;

    SendMessage( hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_EVOLUTION ) ) );
    SendMessage( hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL     ) ) );

	Util::SetApplicationTitle( hwndApp, IDS_APP_TITLE );

	BOOL bRes = SetMenu( hwndApp, LoadMenu( GetModuleHandle( nullptr ), MAKEINTRESOURCE( IDC_EVOLUTION_MAIN ) ) );
	assert( bRes );

	m_hMenu = GetMenu( hwndApp );
	HMENU hMenuView  = GetSubMenu( m_hMenu, 2 );
	HMENU hMenuWindows = GetSubMenu( hMenuView, 1 );

	HMENU hPopupMenu = hMenuWindows;
	{
		UINT  const STD_FLAGS    = MF_BYPOSITION | MF_STRING;
		HMENU const hMiniWinMenu = CreatePopupMenu();
		(void)AppendMenu( hMiniWinMenu, STD_FLAGS, IDM_MINI_WINDOW_AUTO, L"auto" );
		(void)AppendMenu( hMiniWinMenu, STD_FLAGS, IDM_MINI_WINDOW_ON,   L"on" );
		(void)AppendMenu( hMiniWinMenu, STD_FLAGS, IDM_MINI_WINDOW_OFF,  L"off" );
		(void)AppendMenu( hPopupMenu, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMiniWinMenu, L"Mini window" );
	}	
}

void AppMenu::enableMenues( UINT state )
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

	HMENU const hMenu = m_hMenu;
	{
		tOnOffAuto const onOffAuto = Config::GetConfigValueOnOffAuto( Config::tId::miniGridDisplay );
		EnableMenuItem( hMenu, IDM_MINI_WINDOW_AUTO, ((onOffAuto == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
		EnableMenuItem( hMenu, IDM_MINI_WINDOW_ON,   ((onOffAuto == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
		EnableMenuItem( hMenu, IDM_MINI_WINDOW_OFF,  ((onOffAuto == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
	}	
}
