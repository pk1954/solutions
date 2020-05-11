// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "Preferences.h"
#include "SlowMotionRatio.h"
#include "DisplayFunctor.h"
#include "NNetModelStorage.h"
#include "AutoOpen.h"
#include "win32_util.h"
#include "win32_sound.h"
#include "win32_script.h"
#include "win32_simulationControl.h"
#include "win32_zoomControl.h"
#include "win32_aboutBox.h"
#include "win32_NNetWindow.h"
#include "win32_winManager.h"
#include "win32_NNetAppMenu.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetController.h"

NNetController::NNetController
(
	NNetModelStorage        * const pStorage,
	NNetWindow              * const pNNetWindow,
	WinManager              * const pWinManager,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	SlowMotionRatio         * const pSlowMotionRatio
) 
  :	m_pStorage                ( pStorage ),
	m_pNNetWindow             ( pNNetWindow ),
	m_pWinManager             ( pWinManager ),
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
}

bool NNetController::ProcessUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	case IDM_PERF_WINDOW:
	case IDM_CRSR_WINDOW:
	case IDM_PARAM_WINDOW:
	//case IDM_CONS_WINDOW:
		SendMessage( m_pWinManager->GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pNNetWindow->Zoom( wmId == IDM_ZOOM_IN );
		break;

	case IDM_SET_ZOOM:
		m_pNNetWindow->Zoom( MicroMeter((float &)lParam) );
		break;

	case IDM_CENTER_MODEL:
		m_pNNetWindow->CenterModel( true );
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

	case IDD_ARROWS_ON:
		m_pNNetWindow->ShowDirectionArrows( true );
		break;

	case IDD_ARROWS_OFF:
		m_pNNetWindow->ShowDirectionArrows( false );
		break;

	case IDD_SOUND_ON:
		Sound::On();
		break;

	case IDD_SOUND_OFF:
		Sound::Off();
		break;

	case IDD_AUTO_OPEN_ON:
		AutoOpen::On();
		break;

	case IDD_AUTO_OPEN_OFF:
		AutoOpen::Off();
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
	case IDM_ASK_AND_SAVE_MODEL:
		return m_pStorage->AskAndSave( );

	case IDM_SAVE_MODEL:
		if ( m_pStorage->SaveModel( ) )
			Preferences::WritePreferences( m_pStorage->GetModelPath(), m_pNNetWindow  );
		break;

	case IDM_SAVE_MODEL_AS:
		if ( m_pStorage->SaveModelAs( ) )
			Preferences::WritePreferences( m_pStorage->GetModelPath(), m_pNNetWindow  );
		break;

	case IDM_OPEN_MODEL:
		if ( m_pStorage->AskAndSave( ) && m_pStorage->AskModelFile( ) )
		{
			m_pNNetWorkThreadInterface->PostStopComputation();
			m_pNNetWorkThreadInterface->PostSendBack( IDM_READ_MODEL );
		}
		break;

	case IDM_READ_MODEL:
		{
			m_pStorage->Read( );
			m_pNNetWorkThreadInterface->PostResetTimer( );
			m_pNNetWorkThreadInterface->PostRunGenerations( true );
		}
		break;

	case IDM_NEW_MODEL:
		if ( m_pStorage->AskAndSave( ) )
		{
			m_pNNetWorkThreadInterface->PostResetModel( );
			m_pStorage->ResetModelPath( );
		}
		break;

	case IDM_PLUS:
		m_pNNetWindow->ChangePulseRate( true );
		break;

	case IDM_MINUS:
		m_pNNetWindow->ChangePulseRate( false );
		break;

	case IDM_COPY_SELECTION:
		m_pNNetWorkThreadInterface->PostCopySelection( );
		break;

	case IDM_REMOVE_SELECTION:
		m_pNNetWorkThreadInterface->PostDeleteSelection( );
		break;

	case IDM_REMOVE_BEEPERS:
		m_pNNetWorkThreadInterface->PostRemoveBeepers( );
		break;

	case IDM_SELECT_ALL_BEEPERS:
		m_pNNetWorkThreadInterface->PostSelectAllBeepers( );
		break;

	case IDM_MARK_SELECTION:
	case IDM_UNMARK_SELECTION:
		m_pNNetWorkThreadInterface->PostMarkSelection( BoolOp(wmId == IDM_MARK_SELECTION) );
		break;

	case IDD_PULSE_RATE:
		m_pNNetWindow->PulseRateDlg( ShapeId( CastToLong(lParam) ) );
		break;

	case IDD_TRIGGER_SOUND_DLG:
		m_pNNetWindow->TriggerSoundDlg( ShapeId( CastToLong(lParam) ) );
		break;

	case IDM_NNET_REFRESH_RATE:
		m_pNNetWindow->PostMessage( WM_COMMAND, IDD_REFRESH_RATE_DIALOG, 0 );
		break;

	case IDM_RUN:
		m_pNNetWorkThreadInterface->PostResetTimer( );
		if ( m_pDisplayFunctor )
			(* m_pDisplayFunctor)( wstring( L"" ) );
		return true;

	case IDD_CONNECT:
		Sound::Play( TEXT("SNAP_IN_SOUND") ); 
		m_pNNetWorkThreadInterface->PostConnect
		( 
			static_cast<ShapeId>( Util::UnpackLongA( lParam ) ), 
			static_cast<ShapeId>( Util::UnpackLongB( lParam ) ) 
		);
		break;

	case IDM_DELETE:
		if ( m_pNNetWindow->GetHighlightedShapeId() == NO_SHAPE )
			break;
		else 
			; // fall through

	case IDD_REMOVE_SHAPE:
		Sound::Play( TEXT("DISAPPEAR_SOUND") ); 
		m_pNNetWorkThreadInterface->PostRemoveShape( ShapeId( CastToLong(lParam) ) );
		m_pNNetWindow->ResetHighlightedShape();
		break;

	case IDD_DISCONNECT:
		Sound::Play( TEXT("UNLOCK_SOUND") ); 
		m_pNNetWorkThreadInterface->PostDisconnect( ShapeId( CastToLong(lParam) ) );
		m_pNNetWindow->ResetHighlightedShape();
		break;

	case IDD_CONVERT2NEURON:
		Sound::Play( TEXT("UNLOCK_SOUND") ); 
		m_pNNetWorkThreadInterface->PostConvert2Neuron( ShapeId( CastToLong(lParam) ) );
		m_pNNetWindow->ResetHighlightedShape();
		break;

	case IDD_CONVERT2INPUT_NEURON:
		Sound::Play( TEXT("SNAP_IN_SOUND") ); 
		m_pNNetWorkThreadInterface->PostConvert2InputNeuron( ShapeId( CastToLong(lParam) ) );
		m_pNNetWindow->ResetHighlightedShape();
		break;

	case IDD_INSERT_NEURON:
	case IDD_APPEND_NEURON:
	case IDD_APPEND_INPUT_NEURON:
	case IDD_NEW_NEURON:
	case IDD_NEW_INPUT_NEURON:
	case IDD_ADD_OUTGOING2KNOT:
	case IDD_ADD_INCOMING2KNOT:
	case IDD_ADD_OUTGOING2PIPE:
	case IDD_ADD_INCOMING2PIPE:
		m_pNNetWorkThreadInterface->PostActionCommand( wmId, ShapeId( CastToLong(lParam) ), Util::Unpack2MicroMeterPoint(lParam) );
		break;

	case IDM_ANALYZE_LOOPS:
	case IDM_ANALYZE_ANOMALIES:
		m_pNNetWorkThreadInterface->PostActionCommand( wmId, NO_SHAPE, NP_NULL );
		m_pNNetWorkThreadInterface->PostSendBack( IDM_ANALYZE_FINISHED );
		break;

	case IDM_ANALYZE_FINISHED:
		m_pNNetWindow->AnalysisFinished( );
		break;

	case IDM_DESELECT_ALL:
	case IDM_ESCAPE:
		m_pNNetWorkThreadInterface->PostSelectAll( tBoolOp::opFalse );
		break;

	case IDM_SELECT_SHAPE:
		m_pNNetWorkThreadInterface->PostSelectShape( ShapeId( CastToLong(lParam) ), tBoolOp::opTrue );
		break;

	case IDM_DESELECT_SHAPE:
		m_pNNetWorkThreadInterface->PostSelectShape( ShapeId( CastToLong(lParam) ), tBoolOp::opFalse );
		break;

	case IDM_SELECT_ALL:
		m_pNNetWorkThreadInterface->PostSelectAll( tBoolOp::opTrue );
		break;

	case IDM_SELECT_SUBTREE:
		m_pNNetWorkThreadInterface->PostSelectSubtree( ShapeId( CastToLong(lParam) ), tBoolOp::opTrue );
		break;

	case IDM_SCRIPT_DIALOG:
		ScriptDialog( );
		break;

	default:
		return true;
	}

	return false;
}
