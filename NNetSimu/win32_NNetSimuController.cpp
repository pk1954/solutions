// win32_NNetSimuController.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "win32_aboutBox.h"
#include "win32_NNetSimuWindow.h"
#include "win32_NNetSimuController.h"

NNetSimuController::NNetSimuController() :
    m_bTrace      ( TRUE ),
    m_pTraceStream( nullptr ),
	m_pAppWindow  ( nullptr ),
	m_pWinManager ( nullptr ),
    m_pDelay      ( nullptr ),
	m_pAppMenu    ( nullptr ),
	m_hCrsrWait   ( 0 )
{ }

NNetSimuController::~NNetSimuController( )
{
    m_pTraceStream = nullptr;
	m_pAppWindow   = nullptr;
	m_pWinManager  = nullptr;
	m_pDelay       = nullptr;
    m_pStatusBar   = nullptr;
	m_pAppMenu     = nullptr;
}

void NNetSimuController::Initialize
( 
	NNetSimuWindow      * const pAppwindow,
    std::wostream       *       pTraceStream,
	WinManager          * const pWinManager,
	Delay               * const pDelay,
	NNetSimuMenu        * const pAppMenu
)
{
	m_pTraceStream = pTraceStream;
	m_pAppWindow   = pAppwindow;
	m_pWinManager  = pWinManager;
    m_pDelay       = pDelay;
	m_pAppMenu     = pAppMenu;
	m_hCrsrWait    = LoadCursor( NULL, IDC_WAIT );
}

bool NNetSimuController::processUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

void NNetSimuController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
	
	if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                             // but do not concern the model

    switch (wmId)
    {
		case IDM_ABOUT:
			ShowAboutBox( m_pAppWindow->GetWindowHandle( ) );
			break;

		case IDM_EXIT:
			PostMessage( m_pAppWindow->GetWindowHandle( ), WM_CLOSE, 0, 0 );
			break;

		default:
			assert( false );
	        break;
    }
}
