// win32_NNetAppMenu.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "Pipeline.h"
#include "win32_util.h"
#include "NNetModelStorage.h"
#include "win32_winManager.h"
#include "win32_NNetAppMenu.h"

HWND NNetAppMenu::m_hwndApp { nullptr };

void NNetAppMenu::Initialize
( 
	HWND                        const hwndApp, 
	WorkThreadInterface const * const pWorkThreadInterface,
	WinManager          const * const pWinManager
) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_hwndApp     = hwndApp;
	m_pWinManager = pWinManager;

    SendMessage( m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NNETSIMU ) ) );
    SendMessage( m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL    ) ) );

	SetAppTitle( );

	BOOL bRes = SetMenu( m_hwndApp, LoadMenu( hInstance, MAKEINTRESOURCE( IDC_NNET_SIMU_MAIN ) ) );
	assert( bRes );

	m_hMenu = GetMenu( hwndApp );
}

void NNetAppMenu::SetAppTitle( wstring const wstrAdd )
{
	Util::SetApplicationTitle( m_hwndApp, IDS_APP_TITLE, wstrAdd );
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
