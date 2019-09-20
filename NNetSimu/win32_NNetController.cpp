// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "win32_aboutBox.h"
#include "win32_NNetAppWindow.h"
#include "win32_NNetWindow.h"
#include "win32_NNetEditor.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetController.h"

NNetController::NNetController
(
	WinManager       * const pWinManager,
	NNetWindow       * const pNNetWindow,
	NNetEditorWindow * const pNNetEditorWindow
) : 
	m_pAppWindow              ( nullptr ),
	m_pNNetWorkThreadInterface( nullptr ),
	m_pWinManager             ( pWinManager ),
    m_pDelay                  ( nullptr ),
	m_pStatusBar              ( nullptr ),
	m_pNNetWindow             ( pNNetWindow ),
	m_pNNetEditorWindow       ( pNNetEditorWindow ),
	m_hCrsrWait               ( 0 )
{ }

NNetController::~NNetController( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pAppWindow               = nullptr;
	m_pWinManager              = nullptr;
	m_pDelay                   = nullptr;
    m_pStatusBar               = nullptr;
}

void NNetController::Initialize
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

bool NNetController::ProcessUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	case IDM_FIT_ZOOM:
		//m_pNNetWindow->Fit2Rect( );
		//m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		//m_pNNetWindow->Zoom( wmId == IDM_ZOOM_IN );
		//m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
		break;

	case IDM_SET_ZOOM:
		//m_pNNetWindow->SetFieldSize( PIXEL(CastToShort(lParam)));
		//m_pStatusBar->SetSizeTrackBar( PIXEL(CastToShort(lParam)) );
		break;

	case IDM_ZOOM_TRACKBAR:  // comes from trackbar in statusBar
		//m_pNNetWindow->SetFieldSize( PIXEL(CastToShort(lParam)) );
		break;

	case IDM_REFRESH:
		//m_pNNetWindow->Refresh();
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

bool NNetController::ProcessModelCommand( int const wmId, LPARAM const lParam )
{
	switch ( wmId )
	{

	default:
		return true;  // Some commands are handled by the framework controller
		break;
	}
}
