// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include <unordered_map>
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

using std::unordered_map;

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
		break;

	case IDM_OPEN_MODEL:
		m_pStorage->OpenModel( );
		break;

	case IDM_NEW_MODEL:
		break;

	case IDD_PULSE_RATE:
		m_pNNetWindow->ParameterDlg( tParameter::pulseRate );
		break;

	case IDM_RUN:
		m_pNNetWorkThreadInterface->PostResetTimer( );
		return true;

	case IDD_CONNECT:
		m_pNNetWorkThreadInterface->PostConnect( );
		break;

	case IDD_REMOVE_SHAPE:
		if ( m_pNNetWorkThreadInterface->HighlightedShapeCanBeDeleted() )
			m_pNNetWorkThreadInterface->PostRemoveShape( );
		else
			MessageBeep( MB_ICONWARNING );
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

	case IDD_SPLIT_PIPELINE:
		m_pNNetWorkThreadInterface->PostSplitPipeline( Util::Unpack2MicroMeterPoint( lParam ) );
		break;

	case IDD_INSERT_NEURON:
		m_pNNetWorkThreadInterface->PostInsertNeuron( Util::Unpack2MicroMeterPoint( lParam ) );
		break;

	case IDD_ADD_OUTPUT_NEURON:
		m_pNNetWorkThreadInterface->PostAddOutputNeuron( Util::Unpack2MicroMeterPoint( lParam ) );
		break;

	case IDD_ADD_INPUT_NEURON:
		m_pNNetWorkThreadInterface->PostAddInputNeuron( Util::Unpack2MicroMeterPoint( lParam ) );
		break;

	default:
		return true;
	}

	return false;
}
