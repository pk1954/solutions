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
#include "win32_NNetAppWindow.h"
#include "win32_NNetSimuController.h"

NNetSimuController::NNetSimuController
(
	WinManager * const pWinManager
) : 
	m_pAppWindow              ( nullptr ),
	m_pNNetWorkThreadInterface( nullptr ),
	m_pWinManager             ( pWinManager ),
    m_pDelay                  ( nullptr ),
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
}

void NNetSimuController::Initialize
( 
	NNetAppWindow           * const pAppWindow,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	Delay                   * const pDelay
)
{
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
	m_pAppWindow               = pAppWindow;
    m_pDelay                   = pDelay;
	m_hCrsrWait                = LoadCursor( NULL, IDC_WAIT );
}

bool NNetSimuController::ProcessUIcommand( int const wmId, LPARAM const lParam )
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

bool NNetSimuController::ProcessModelCommand( int const wmId, LPARAM const lParam )
{
	switch ( wmId )
	{

	default:
		return true;  // Some commands are handled by the framework controller
		break;
	}
}
