// win32_NNetAppMenu.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "Pipeline.h"
#include "win32_util.h"
#include "win32_winManager.h"
#include "win32_NNetAppMenu.h"

void NNetAppMenu::Initialize
( 
	HWND                        const hwndApp, 
	WorkThreadInterface const * const pWorkThreadInterface,
	WinManager          const * const pWinManager
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

void NNetAppMenu::enableMenues( UINT const state )
{
	EnableMenuItem( m_hMenu, 1, state|MF_BYPOSITION ); 
	EnableMenuItem( m_hMenu, 2, state|MF_BYPOSITION ); 
}

void NNetAppMenu::Start( )
{
	enableMenues( MF_ENABLED ); 
}

void NNetAppMenu::Stop( )
{
	enableMenues( MF_GRAYED ); 
}

void NNetAppMenu::AdjustVisibility( )
{
	EnableMenuItem( m_hMenu, IDM_CRSR_WINDOW,  m_pWinManager->IsVisible( IDM_CRSR_WINDOW )  ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_PARAM_WINDOW, m_pWinManager->IsVisible( IDM_PARAM_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_ARROWS_OFF, (Pipeline::GetArrowSize() != Pipeline::STD_ARROW_SIZE) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_ARROWS_ON,  (Pipeline::GetArrowSize() == Pipeline::STD_ARROW_SIZE) ? MF_GRAYED : MF_ENABLED );
}
