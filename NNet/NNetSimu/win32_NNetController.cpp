// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "Analyzer.h"
#include "Preferences.h"
#include "SlowMotionRatio.h"
#include "NNetModelStorage.h"
#include "ComputeThread.h"
#include "AnimationThread.h"
#include "AutoOpen.h"
#include "win32_util.h"
#include "win32_sound.h"
#include "win32_script.h"
#include "win32_aboutBox.h"
#include "win32_NNetWindow.h"
#include "win32_winManager.h"
#include "win32_fatalError.h"
#include "win32_triggerSoundDlg.h"
#include "win32_stdDialogBox.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_NNetController.h"

void NNetController::Initialize
(
	NNetModelStorage         * const pStorage,
	NNetWindow               * const pNNetWindow,
	WinManager               * const pWinManager,
	NNetModelReaderInterface * const pModelReaderInterface,
	NNetModelWriterInterface * const pModelWriterInterface,
	ComputeThread            * const pComputeThread,
	SlowMotionRatio          * const pSlowMotionRatio,
	DisplayFunctor           * const func
) 
{
  	m_pStorage              = pStorage;
	m_pNNetWindow           = pNNetWindow;
	m_pWinManager           = pWinManager;
	m_pModelReaderInterface = pModelReaderInterface;
	m_pModelWriterInterface = pModelWriterInterface;
	m_pSlowMotionRatio      = pSlowMotionRatio;
	m_pComputeThread        = pComputeThread;
	m_pStatusBarDisplay     = func;
	m_hCrsrWait             = LoadCursor( NULL, IDC_WAIT );
	m_pAnimationThread      = new AnimationThread( );
}

NNetController::~NNetController( )
{
	delete m_pAnimationThread;

	m_pStorage              = nullptr;
	m_pNNetWindow           = nullptr;
	m_pWinManager           = nullptr;
	m_pModelReaderInterface = nullptr;
	m_pModelWriterInterface = nullptr;
	m_pSlowMotionRatio      = nullptr;
	m_pComputeThread        = nullptr;
	m_pStatusBarDisplay     = nullptr;
	m_hCrsrWait             = nullptr;
	m_pAnimationThread      = nullptr;
}

bool NNetController::HandleCommand( int const wmId, LPARAM const lParam, MicroMeterPoint const umPoint )
{
	bool bRes { false };

	if ( wmId == IDM_FATAL_ERROR )
	{
		FatalError::Happened( static_cast<long>(lParam), "unknown" );
	}

	try
	{
		if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
			return true;                        // but do not concern the model  
	}
	catch ( ... )
	{
		FatalError::Happened( 2, "processUIcommand" );
	}

	try
	{
		m_pComputeThread->StopComputation( );
		bRes = processModelCommand( wmId, lParam, umPoint );
		m_pComputeThread->RunComputation( );
	}
	catch ( ... )
	{
		FatalError::Happened( 3, "processModelCommand" );
	}

	return bRes;
}

bool NNetController::processUIcommand( int const wmId, LPARAM const lParam )
{
	switch ( wmId )
	{

	case IDM_PERF_WINDOW:
	case IDM_CRSR_WINDOW:
	case IDM_MINI_WINDOW:
	case IDM_PARAM_WINDOW:
	//case IDM_CONS_WINDOW:
		::SendMessage( m_pWinManager->GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pNNetWindow->ZoomStep( wmId == IDM_ZOOM_IN );
		break;

	case IDM_SET_ZOOM:
		m_pNNetWindow->Zoom( MicroMeter((float &)lParam) );
		break;

	case IDM_CENTER_MODEL:
		m_pNNetWindow->CenterModel( true );
		break;

	case IDM_SLOWER:
		if ( m_pSlowMotionRatio->IncRatio( ) )
			m_pComputeThread->Reset( );
		else
			MessageBeep( MB_ICONWARNING );
		break;

	case IDM_FASTER:
		if ( m_pSlowMotionRatio->DecRatio( ) )
			m_pComputeThread->Reset( );
		else
			MessageBeep( MB_ICONWARNING );
		break;

	case IDD_ARROWS_ON:
		m_pAnimationThread->SetTarget( Pipe::STD_ARROW_SIZE );
		break;

	case IDD_ARROWS_OFF:
		m_pAnimationThread->SetTarget( 0.0_MicroMeter );
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
		return false; // command has not been processed
	}

	return true;  // command has been processed
}

bool NNetController::changePulseRate( ShapeId const id, bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	fHertz const fOldValue { m_pModelReaderInterface->GetPulseFreq( id ) };
	if ( fOldValue.IsNull() )
		return false;
	fHertz const fNewValue = fOldValue + ( bDirection ? INCREMENT : -INCREMENT );
	m_pModelWriterInterface->SetPulseRate( id, fNewValue );
	return true;
}

void NNetController::pulseRateDlg( ShapeId const id )
{
	fHertz  const fOldValue { m_pModelReaderInterface->GetPulseFreq( id ) };
	if ( fOldValue.IsNull() )
		return;
	HWND    const hwndParent { m_pNNetWindow->GetWindowHandle() };
	wstring const header     { GetParameterName ( tParameter::pulseRate ) }; 
	wstring const unit       { GetParameterUnit ( tParameter::pulseRate ) };
	fHertz  const fNewValue  { StdDialogBox::Show( hwndParent, fOldValue.GetValue(), header, unit ) };
	if ( fNewValue != fOldValue )
		m_pModelWriterInterface->SetPulseRate( id, fNewValue );
}

void NNetController::triggerSoundDlg( ShapeId const id )
{
	ShapeType const type { m_pModelReaderInterface->GetShapeType(id) };
	if ( ! type.IsAnyNeuronType() )
		return;

	TriggerSoundDialog dialog
	( 
		m_pModelReaderInterface->HasTriggerSound( id ), 
		m_pModelReaderInterface->GetTriggerSoundFrequency( id ), 
		m_pModelReaderInterface->GetTriggerSoundDuration( id ) 
	);

	dialog.Show( m_pNNetWindow->GetWindowHandle() );

	m_pModelWriterInterface->SetTriggerSound
	( 
		id,
		dialog.IsTriggerSoundActive(),
		dialog.GetFrequency(),
		dialog.GetDuration ()
	);
}

bool NNetController::processModelCommand( int const wmId, LPARAM const lParam, MicroMeterPoint const umPoint )
{
	switch ( wmId )
	{
	case IDM_ASK_AND_SAVE_MODEL:
		return m_pStorage->AskAndSave( );

	case IDM_SAVE_MODEL:
		if ( m_pStorage->SaveModel( ) )
			Preferences::WritePreferences( m_pStorage->GetModelPath() );
		break;

	case IDM_SAVE_MODEL_AS:
		if ( m_pStorage->SaveModelAs( ) )
			Preferences::WritePreferences( m_pStorage->GetModelPath() );
		break;

	case IDM_OPEN_MODEL:
		if ( m_pStorage->AskAndSave( ) && m_pStorage->AskModelFile() )
		{
			m_pStorage->Read( );
			m_pModelWriterInterface->ResetTimer( );
			m_pNNetWindow->CenterModel( true );
		}
		break;

	case IDM_NEW_MODEL:
		if ( m_pStorage->AskAndSave( ) )
		{
			m_pModelWriterInterface->ResetModel( );
			m_pModelWriterInterface->ResetTimer( );
			m_pNNetWindow->CenterModel( true );
			m_pStorage->ResetModelPath( );
		}
		break;

	case IDM_PLUS:
		changePulseRate( m_pNNetWindow->GetHighlightedShapeId(), true );
		break;

	case IDM_MINUS:
		changePulseRate( m_pNNetWindow->GetHighlightedShapeId(), false );
		break;

	case IDM_COPY_SELECTION:
		m_pModelWriterInterface->CopySelection( );
		break;

	case IDM_REMOVE_SELECTION:
		m_pModelWriterInterface->DeleteSelection( );
		break;

	case IDM_REMOVE_BEEPERS:
		m_pModelWriterInterface->RemoveBeepers( );
		break;

	case IDM_SELECT_ALL_BEEPERS:
		m_pModelWriterInterface->SelectAllBeepers( );
		break;

	case IDM_MARK_SELECTION:
		m_pModelWriterInterface->MarkSelection( tBoolOp::opTrue );
		break;

	case IDM_UNMARK_SELECTION:
		m_pModelWriterInterface->MarkSelection( tBoolOp::opFalse );
		break;

	case IDD_PULSE_RATE:
		pulseRateDlg( ShapeId( CastToLong(lParam) ) );
		break;

	case IDD_TRIGGER_SOUND_DLG:
		triggerSoundDlg( ShapeId( CastToLong(lParam) ) );
		break;

	case IDM_NNET_REFRESH_RATE:
		m_pNNetWindow->PostMessage( WM_COMMAND, IDD_REFRESH_RATE_DIALOG, 0 );
		break;

	case IDM_RUN:
		m_pModelWriterInterface->ResetTimer( );
		if ( m_pStatusBarDisplay )
			(* m_pStatusBarDisplay)( wstring( L"" ) );
		return false;

	case IDD_CONNECT:
		Sound::Play( TEXT("SNAP_IN_SOUND") ); 
		m_pModelWriterInterface->Connect
		( 
			static_cast<ShapeId>( Util::UnpackLongA( lParam ) ), 
			static_cast<ShapeId>( Util::UnpackLongB( lParam ) ) 
		);
		break;

	case IDM_DELETE:   // keyboard: delete key
		if ( m_pNNetWindow->GetHighlightedShapeId() == NO_SHAPE )
			break;
		else 
			; // fall through

	case IDD_REMOVE_SHAPE:
		Sound::Play( TEXT("DISAPPEAR_SOUND") ); 
		m_pModelWriterInterface->RemoveShape( ShapeId( CastToLong(lParam) ) );
		break;

	case IDD_DISCONNECT:
		Sound::Play( TEXT("UNLOCK_SOUND") ); 
		m_pModelWriterInterface->Disconnect( ShapeId( CastToLong(lParam) ) );
		break;

	case IDD_CONVERT2NEURON:
		Sound::Play( TEXT("UNLOCK_SOUND") ); 
		m_pModelWriterInterface->Convert2Neuron( ShapeId( CastToLong(lParam) ) );
		break;

	case IDD_STOP_ON_TRIGGER:
		Sound::Play( TEXT("SNAP_IN_SOUND") ); 
		m_pModelWriterInterface->ToggleStopOnTrigger( ShapeId( CastToLong(lParam) ) );
		break;

	case IDD_CONVERT2INPUT_NEURON:
		Sound::Play( TEXT("SNAP_IN_SOUND") ); 
		m_pModelWriterInterface->Convert2InputNeuron( ShapeId( CastToLong(lParam) ) );
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
		m_pModelWriterInterface->ActionCommand( wmId, ShapeId( CastToLong(lParam) ), umPoint );
		break;

	case IDM_ANALYZE_LOOPS:
	case IDM_ANALYZE_ANOMALIES:
		{
			m_pModelWriterInterface->ActionCommand( wmId, NO_SHAPE, NP_NULL );
			MicroMeterRect rect { ModelAnalyzer::GetEnclosingRect() };
			if ( rect.IsNotEmpty() )
				m_pNNetWindow->CenterAndZoomRect( rect, 2.0f, true );
		}
		break;

	case IDM_DESELECT_ALL:
	case IDM_ESCAPE:
		m_pModelWriterInterface->SelectAll( tBoolOp::opFalse );
		ModelAnalyzer::Stop();
		break;

	case IDM_SELECT_SHAPE:
		m_pModelWriterInterface->SelectShape( ShapeId( CastToLong(lParam) ), tBoolOp::opTrue );
		break;

	case IDM_DESELECT_SHAPE:
		m_pModelWriterInterface->SelectShape( ShapeId( CastToLong(lParam) ), tBoolOp::opFalse );
		break;

	case IDM_SELECT_ALL:
		m_pModelWriterInterface->SelectAll( tBoolOp::opTrue );
		break;

	case IDM_SELECT_SUBTREE:
		m_pModelWriterInterface->SelectSubtree( ShapeId( CastToLong(lParam) ), tBoolOp::opTrue );
		break;

	case IDM_SCRIPT_DIALOG:
		ScriptDialog( );
		break;

	default:
		return false;
	}

	return true;
}
