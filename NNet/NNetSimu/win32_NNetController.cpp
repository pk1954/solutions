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
#include "NNetModel.h"
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
	NNetModel               * const pModel,
	NNetModelStorage        * const pStorage,
	NNetWindow              * const pNNetWindow,
	WinManager              * const pWinManager,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	SlowMotionRatio         * const pSlowMotionRatio
) 
  :	m_pNNetModel              ( pModel ),
	m_pStorage                ( pStorage ),
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
		m_pNNetWindow->ZoomKeepCrsrPos( MicroMeter((float &)lParam) );
		break;

	case IDM_CENTER_MODEL:
		m_pNNetWindow->CenterModel( );
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
		return m_pStorage->AskAndSave( * m_pNNetModel );

	case IDM_SAVE_MODEL:
		if ( m_pStorage->SaveModel( * m_pNNetModel ) )
			Preferences::WritePreferences( m_pStorage->GetModelPath(), m_pNNetWindow  );
		break;

	case IDM_SAVE_MODEL_AS:
		if ( m_pStorage->SaveModelAs( * m_pNNetModel ) )
			Preferences::WritePreferences( m_pStorage->GetModelPath(), m_pNNetWindow  );
		break;

	case IDM_OPEN_MODEL:
		if ( m_pStorage->AskAndSave( * m_pNNetModel ) && m_pStorage->AskModelFile( ) )
		{
			m_pNNetWorkThreadInterface->PostStopComputation();
			m_pNNetWorkThreadInterface->PostSendBack( IDM_READ_MODEL );
		}
		break;

	case IDM_READ_MODEL:
		{
			m_pStorage->Read( * m_pNNetModel );
			m_pNNetWorkThreadInterface->PostResetTimer( );
			m_pNNetWorkThreadInterface->PostRunGenerations( true );
		}
		break;

	case IDM_NEW_MODEL:
		if ( m_pStorage->AskAndSave( * m_pNNetModel ) )
		{
			m_pNNetWorkThreadInterface->PostResetModel( );
			m_pStorage->ResetModelPath( );
		}
		break;

	case IDM_PLUS:
		m_pNNetWindow->ChangePulseRate( m_pNNetWindow->GetHighlightedShapeId( ), true );
		break;

	case IDM_MINUS:
		m_pNNetWindow->ChangePulseRate( m_pNNetWindow->GetHighlightedShapeId( ), false );
		break;

	case IDD_PULSE_RATE:
		if ( m_pNNetWindow->PulseRateDlg( m_pNNetWindow->GetHighlightedShapeId( ) ) )
			m_bUnsavedChanges = true;
		break;

	case IDD_TRIGGER_SOUND_DLG:
		if ( m_pNNetWindow->TriggerSoundDlg( m_pNNetWindow->GetHighlightedShapeId( ) ) )
			m_bUnsavedChanges = true;
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

	case IDD_REMOVE_SHAPE:
		Sound::Play( TEXT("DISAPPEAR_SOUND") ); 
		m_pNNetWorkThreadInterface->PostRemoveShape( m_pNNetWindow->GetHighlightedShapeId( ) );
		m_pNNetWindow->ResetHighlightedShape();
		m_bUnsavedChanges = true;
		break;

	case IDD_DISCONNECT:
		Sound::Play( TEXT("UNLOCK_SOUND") ); 
		m_pNNetWorkThreadInterface->PostDisconnect( m_pNNetWindow->GetHighlightedShapeId( ) );
		m_pNNetWindow->ResetHighlightedShape();
		m_bUnsavedChanges = true;
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
		m_pNNetWorkThreadInterface->PostActionCommand( wmId, m_pNNetWindow->GetHighlightedShapeId( ), Util::Unpack2MicroMeterPoint(lParam) );
		break;

	case IDM_ANALYZE:
		//m_pWinManager->BringToTop( IDM_CONS_WINDOW );
		//SetFocus( m_pWinManager->GetHWND( IDM_APPL_WINDOW ) );
		m_pNNetWorkThreadInterface->PostActionCommand( wmId, NO_SHAPE, NP_NULL );
		m_pNNetWorkThreadInterface->PostSendBack( IDM_ANALYZE_FINISHED );
		break;

	case IDM_ANALYZE_FINISHED:
		m_pNNetWindow->AnalysisFinished( );
		break;

	case IDM_SCRIPT_DIALOG:
		ScriptDialog( );
		break;

	default:
		return true;
	}

	return false;
}
