// win32_NNetAppMenu.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "Pipe.h"
#include "win32_util.h"
#include "NNetModelStorage.h"
#include "ComputeThread.h"
#include "AutoOpen.h"
#include "win32_sound.h"
#include "win32_winManager.h"
#include "NNetModelWriterInterface.h"
#include "win32_NNetAppMenu.h"

void NNetAppMenu::Initialize
( 
	HWND                             const hwndApp,
	ComputeThread            const * const pComputeThread,
	NNetModelWriterInterface const * const pModel,
	WinManager               const * const pWinManager
) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );

	m_hwndApp              = hwndApp;
	m_pComputeThread       = pComputeThread;
	m_pModel = pModel;
	m_pWinManager          = pWinManager;

    SendMessage( m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NNETSIMU ) ) );
    SendMessage( m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL    ) ) );

	SetAppTitle( L"", false );

	bool bRes = SetMenu( m_hwndApp, LoadMenu( hInstance, MAKEINTRESOURCE( IDC_NNET_SIMU_MAIN ) ) );
	assert( bRes );

	m_hMenu = GetMenu( hwndApp );
}

void NNetAppMenu::SetAppTitle( wstring const wstrAdd, bool const bUnsavedChanges )
{
	Util::SetApplicationTitle( m_hwndApp, IDS_APP_TITLE, wstrAdd + (bUnsavedChanges ? L" * " : L"") );
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
	bool const bRunning = m_pComputeThread->IsRunning();

	EnableMenuItem( m_hMenu, IDM_FORWARD, bRunning ? MF_GRAYED  : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_RESET,   bRunning ? MF_GRAYED  : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_RUN,     bRunning ? MF_GRAYED  : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_STOP,    bRunning ? MF_ENABLED : MF_GRAYED );

	EnableMenuItem( m_hMenu, IDM_CRSR_WINDOW,  m_pWinManager->IsVisible( IDM_CRSR_WINDOW  ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_MINI_WINDOW,  m_pWinManager->IsVisible( IDM_MINI_WINDOW  ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_PARAM_WINDOW, m_pWinManager->IsVisible( IDM_PARAM_WINDOW ) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDM_PERF_WINDOW,  m_pWinManager->IsVisible( IDM_PERF_WINDOW  ) ? MF_GRAYED : MF_ENABLED );
	//EnableMenuItem( m_hMenu, IDM_CONS_WINDOW,  m_pWinManager->IsVisible( IDM_CONS_WINDOW  ) ? MF_GRAYED : MF_ENABLED );

	EnableMenuItem( m_hMenu, IDD_ARROWS_OFF, (Pipe::GetArrowSize() != Pipe::STD_ARROW_SIZE) ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_ARROWS_ON,  (Pipe::GetArrowSize() == Pipe::STD_ARROW_SIZE) ? MF_GRAYED : MF_ENABLED );

	EnableMenuItem( m_hMenu, IDD_SOUND_ON,    Sound::IsOn() ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_SOUND_OFF, ! Sound::IsOn() ? MF_GRAYED : MF_ENABLED );

	EnableMenuItem( m_hMenu, IDD_AUTO_OPEN_ON,    AutoOpen::IsOn() ? MF_GRAYED : MF_ENABLED );
	EnableMenuItem( m_hMenu, IDD_AUTO_OPEN_OFF, ! AutoOpen::IsOn() ? MF_GRAYED : MF_ENABLED );
}
