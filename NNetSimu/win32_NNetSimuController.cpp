// win32_NNetSimuController.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "win32_aboutBox.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetSimuWindow.h"
#include "win32_NNetSimuController.h"

NNetSimuController::NNetSimuController() :
	m_pAppWindow              ( nullptr ),
	m_pNNetWorkThreadInterface( nullptr ),
	m_pWinManager             ( nullptr ),
    m_pDelay                  ( nullptr ),
	m_pAppMenu                ( nullptr ),
	m_pStatusBar              ( nullptr ),
	m_hCrsrWait               ( 0 )
{ }

NNetSimuController::~NNetSimuController( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pAppWindow               = nullptr;
	m_pWinManager              = nullptr;
	m_pDelay                   = nullptr;
    m_pStatusBar               = nullptr;
	m_pAppMenu                 = nullptr;
}

void NNetSimuController::Initialize
( 
	NNetSimuWindow          * const pAppWindow,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	WinManager              * const pWinManager,
	Delay                   * const pDelay,
	NNetSimuMenu            * const pAppMenu
)
{
	Controller::Initialize( pAppWindow,  pNNetWorkThreadInterface );
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
	m_pAppWindow               = pAppWindow;
	m_pWinManager              = pWinManager;
    m_pDelay                   = pDelay;
	m_pAppMenu                 = pAppMenu;
	m_hCrsrWait                = LoadCursor( NULL, IDC_WAIT );
}

bool NNetSimuController::processUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	case IDM_FIT_ZOOM:
		//m_pGridWindow->Fit2Rect( );
		//m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		//m_pGridWindow->Zoom( wmId == IDM_ZOOM_IN );
		//m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
		break;

	case IDM_SET_ZOOM:
		//m_pGridWindow->SetFieldSize( PIXEL(CastToShort(lParam)));
		//m_pStatusBar->SetSizeTrackBar( PIXEL(CastToShort(lParam)) );
		break;

	case IDM_ZOOM_TRACKBAR:  // comes from trackbar in statusBar
		//(void)m_pGridWindow->SetFieldSize( PIXEL(CastToShort(lParam)) );
		break;

	case IDM_REFRESH:
		//m_pGridWindow->Refresh();
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

void NNetSimuController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
	
	if ( Controller::ProcessCommand( wmId, lParam ) )  // Some commands are handled by 
		return;                                        // the framework controller

	if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                             // but do not concern the model

    switch (wmId)
    {
	case IDM_RUN:
//		m_pEditorWindow->SendClick( IDM_MOVE );   // change edit mode to move
		m_pNNetWorkThreadInterface->PostRunGenerations( true );
		break;

	case IDM_RESET:
	{
	}
	break;

		default:
			assert( false );
	        break;
    }
}
