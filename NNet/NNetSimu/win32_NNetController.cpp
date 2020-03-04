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
	m_hCrsrWait               ( LoadCursor( NULL, IDC_WAIT ) ),
	m_bUnsavedChanges         ( false )
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

	case IDM_PERF_WINDOW:
	case IDM_CRSR_WINDOW:
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
	case IDM_SAVE_MODEL:
		if ( m_pStorage->SaveModel( ) )
			NNetAppMenu::SetAppTitle( m_pStorage->GetModelPath() );
		break;

	case IDM_SAVE_MODEL_AS:
		if ( m_pStorage->SaveModelAs( ) )
			NNetAppMenu::SetAppTitle( m_pStorage->GetModelPath() );
		break;

	case IDM_OPEN_MODEL:
		if ( m_pStorage->AskSave( ) && m_pStorage->AskModelFile( ) )
		{
			m_pNNetWorkThreadInterface->PostStopComputation();
			m_pNNetWorkThreadInterface->PostSendBack( IDM_READ_MODEL );
		}
		break;

	case IDM_READ_MODEL:
		{
			m_pStorage->Read( );
			Preferences::WritePreferences( m_pStorage->GetModelPath()  );
			NNetAppMenu::SetAppTitle( m_pStorage->GetModelPath() );
			m_pNNetWorkThreadInterface->PostResetTimer( );
			m_pNNetWorkThreadInterface->PostRunGenerations( true );
		}
		break;

	case IDM_NEW_MODEL:
		if ( m_pStorage->AskSave( ) )
		{
			m_pNNetWorkThreadInterface->PostResetModel( );
			m_pStorage->ResetModelPath( );
			NNetAppMenu::SetAppTitle( m_pStorage->GetModelPath() );
		}
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
		m_pNNetWorkThreadInterface->PostActionCommand( wmId, NO_SHAPE, NP_NULL );
		m_pNNetWorkThreadInterface->PostSendBack( IDM_ANALYZE_FINISHED );
		break;

	case IDM_ANALYZE_FINISHED:
		m_pNNetWindow->EmphasizeAnalyzeResult( );
		break;

	case IDM_SCRIPT_DIALOG:
		ScriptDialog( );
		break;

	default:
		return true;
	}

	return false;
}
