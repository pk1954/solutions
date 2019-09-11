// win32_NNetSimuMenu.cpp
//
// NNetSimu

#include "stdafx.h"
//#include "WinUser.h"
#include "Resource.h"
#include "win32_util.h"
#include "win32_winManager.h"
#include "win32_NNetSimuMenu.h"

void NNetSimuMenu::Initialize
( 
	HWND               const hwndApp, 
	WinManager const * const pWinManager
) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_pWinManager = pWinManager;

    SendMessage( hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NNETSIMU ) ) );
    SendMessage( hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL    ) ) );

	Util::SetApplicationTitle( hwndApp, IDS_APP_TITLE );

	BOOL bRes = SetMenu( hwndApp, LoadMenu( GetModuleHandle( nullptr ), MAKEINTRESOURCE( IDC_NNET_SIMU_MAIN ) ) );
	assert( bRes );

	m_hMenu = GetMenu( hwndApp );
}

void NNetSimuMenu::enableMenues( UINT const state )
{
	EnableMenuItem( m_hMenu, 1, state|MF_BYPOSITION ); 
	EnableMenuItem( m_hMenu, 2, state|MF_BYPOSITION ); 
}

void NNetSimuMenu::Start( )
{
	enableMenues( MF_ENABLED ); 
}

void NNetSimuMenu::Stop( )
{
	enableMenues( MF_GRAYED ); 
}

void NNetSimuMenu::AdjustVisibility( )
{
}
