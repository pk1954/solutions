// win32_appMenu.cpp
//
// Evolution

#include "stdafx.h"
#include "WinUser.h"
#include "Resource.h"
#include "GridDimensions.h"
#include "win32_util.h"
#include "win32_appMenu.h"

void AppMenu::Initialize( HWND const hwndApp ) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

    SendMessage( hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_EVOLUTION ) ) );
    SendMessage( hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL     ) ) );

	Util::SetApplicationTitle( hwndApp, IDS_APP_TITLE );

	BOOL bRes = SetMenu( hwndApp, LoadMenu( GetModuleHandle( nullptr ), MAKEINTRESOURCE( IDC_EVOLUTION_MAIN ) ) );
	assert( bRes );

	m_hMenu = GetMenu( hwndApp );
}

void AppMenu::enableMenues( UINT state )
{
	EnableMenuItem( m_hMenu, 1, state|MF_BYPOSITION ); 
	EnableMenuItem( m_hMenu, 2, state|MF_BYPOSITION ); 
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
	EnableMenuItem( m_hMenu, IDM_BACKWARDS,        state );
	EnableMenuItem( m_hMenu, IDM_GOTO_ORIGIN,      state );
	EnableMenuItem( m_hMenu, IDM_GOTO_DEATH,       state );
	EnableMenuItem( m_hMenu, IDM_RESET,            state );
	EnableMenuItem( m_hMenu, IDM_CHANGE_GRID_TYPE, state );
	EnableMenuItem( m_hMenu, IDM_RUN,              state );
	EnableMenuItem( m_hMenu, IDM_STOP,             bMode ? MF_ENABLED : MF_GRAYED );
}
