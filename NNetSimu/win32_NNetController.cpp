// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "SlowMotionRatio.h"
#include "NNetModelStorage.h"
#include "win32_util.h"
#include "win32_simulationControl.h"
#include "win32_zoomControl.h"
#include "win32_aboutBox.h"
#include "win32_NNetWindow.h"
#include "win32_winManager.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetController.h"

NNetController::NNetController
(
	NNetModelStorage        * const pStorage,
	NNetWindow              * const pNNetWindow,
	WinManager              * const pWinManager,
	StatusBar               * const pStatusBar,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	SlowMotionRatio         * const pSlowMotionRatio
) 
  :	m_pStorage                ( pStorage ),
	m_pNNetWindow             ( pNNetWindow ),
	m_pWinManager             ( pWinManager ),
	m_pStatusBar              ( pStatusBar ),
	m_pNNetWorkThreadInterface( pNNetWorkThreadInterface ),
	m_pSlowMotionRatio        ( pSlowMotionRatio ),
	m_hCrsrWait               ( LoadCursor( NULL, IDC_WAIT ) )
{
}

NNetController::~NNetController( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pStorage                 = nullptr;
	m_pWinManager              = nullptr;
	m_pSlowMotionRatio         = nullptr;
    m_pStatusBar               = nullptr;
}

bool NNetController::ProcessUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	case IDM_CRSR_WINDOW:
		SendMessage( m_pWinManager->GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
		break;

	case IDM_PARAM_WINDOW:
		SendMessage( m_pWinManager->GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pNNetWindow->Zoom( wmId == IDM_ZOOM_IN );
		break;

	case IDM_SET_ZOOM:
		m_pNNetWindow->SetPixelSize( MicroMeter((float &)lParam) );
		break;

	case IDM_REFRESH:
		m_pNNetWindow->Notify( lParam != 0 );
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
	case IDM_SAVE_MODEL:
		m_pStorage->SaveModel( );
		break;

	case IDM_SAVE_MODEL_AS:
		m_pStorage->SaveModelAs( );
		break;

	case IDM_OPEN_MODEL:
		m_pStorage->OpenModel( );
		break;

	case IDM_NEW_MODEL:
		break;

	case IDD_PULSE_RATE:
		m_pNNetWindow->PulseRateDlg( m_pNNetWindow->GetHighlightedShapeId( ) );
		break;

	case IDM_RUN:
		m_pNNetWorkThreadInterface->PostResetTimer( );
		return true;

	case IDD_CONNECT:
		m_pNNetWorkThreadInterface->PostConnect
		( 
			static_cast<ShapeId>( Util::UnpackLongA( lParam ) ), 
			static_cast<ShapeId>( Util::UnpackLongB( lParam ) ) 
		);
		break;

	case IDD_REMOVE_SHAPE:
		m_pNNetWorkThreadInterface->PostRemoveShape( m_pNNetWindow->GetHighlightedShapeId( ) );
		m_pNNetWindow->ResetHighlightedShape();
		break;

	case IDM_SLOWER:
		if ( m_pSlowMotionRatio->IncRatio( ) )
			m_pNNetWorkThreadInterface->PostSlowMotionChanged( );
		else
			MessageBeep( MB_ICONWARNING );
		break;

	case IDM_FASTER:
		if ( m_pSlowMotionRatio->DecRatio( ) )
			m_pNNetWorkThreadInterface->PostSlowMotionChanged( );
		else
			MessageBeep( MB_ICONWARNING );
		break;

	case IDD_INSERT_NEURON:
	case IDD_ADD_NEURON:
	case IDD_ADD_INPUT_NEURON:
	case IDD_ADD_OUTGOING:
	case IDD_ADD_INCOMING:
	case IDD_SPLIT_PIPELINE:
		m_pNNetWorkThreadInterface->PostActionCommand( wmId, m_pNNetWindow->GetHighlightedShapeId( ), lParam );
		break;

	default:
		return true;
	}

	return false;
}
