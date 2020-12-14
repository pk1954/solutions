// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include <exception>
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "Analyzer.h"
#include "Preferences.h"
#include "SlowMotionRatio.h"
#include "NNetModelStorage.h"
#include "ComputeThread.h"
#include "CommandStack.h"
#include "SignalInterface.h"
#include "AutoOpen.h"
#include "win32_util.h"
#include "win32_sound.h"
#include "win32_script.h"
#include "win32_aboutBox.h"
#include "win32_MainWindow.h"
#include "win32_winManager.h"
#include "win32_fatalError.h"
#include "win32_triggerSoundDlg.h"
#include "win32_stdDialogBox.h"
#include "win32_monitorWindow.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelCommands.h"
#include "win32_monitorWindow.h"
#include "win32_NNetController.h"

void NNetController::Initialize
(
    NNetModelStorage         * const pStorage,
    MainWindow               * const pMainWindow,
    WinManager               * const pWinManager,
    NNetModelReaderInterface * const pModelReaderInterface,
    NNetModelCommands        * const pModelCommands,
    ComputeThread            * const pComputeThread,
    SlowMotionRatio          * const pSlowMotionRatio,
    DisplayFunctor           * const func,
    Sound                    * const pSound,
    Preferences              * const pPreferences,
    CommandStack             * const pCommandStack,
    MonitorWindow            * const pMonitorWindow,
    Param                    * const pParam,
    Observable               * const pDynamicModelObservable
) 
{
    m_pStorage                = pStorage;
    m_pMainWindow             = pMainWindow;
    m_pWinManager             = pWinManager;
    m_pModelReaderInterface   = pModelReaderInterface;
    m_pModelCommands          = pModelCommands;
    m_pSlowMotionRatio        = pSlowMotionRatio;
    m_pComputeThread          = pComputeThread;
    m_pStatusBarDisplay       = func;
    m_pSound                  = pSound;
    m_pPreferences            = pPreferences;
    m_pCommandStack           = pCommandStack;
    m_pMonitorWindow          = pMonitorWindow;
    m_pParam                  = pParam;
    m_pDynamicModelObservable = pDynamicModelObservable;
    m_hCrsrWait               = LoadCursor( NULL, IDC_WAIT );
}

NNetController::~NNetController( )
{
    m_pStorage              = nullptr;
    m_pMainWindow           = nullptr;
    m_pWinManager           = nullptr;
    m_pModelReaderInterface = nullptr;
    m_pModelCommands        = nullptr;
    m_pSlowMotionRatio      = nullptr;
    m_pComputeThread        = nullptr;
    m_pStatusBarDisplay     = nullptr;
    m_hCrsrWait             = nullptr;
    m_pSound                = nullptr;
    m_pPreferences          = nullptr;
    m_pCommandStack         = nullptr;
    m_pMonitorWindow        = nullptr;
}

bool NNetController::HandleCommand( int const wmId, LPARAM const lParam, MicroMeterPoint const umPoint )
{
    bool bRes { false };

    if ( wmId == IDM_FATAL_ERROR )
    {
        FatalError::Happened( static_cast<long>(lParam), "unknown" );
    }

    if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
        return true;                        // but do not concern the model  

    m_pComputeThread->LockComputation( );
    bRes = processModelCommand( wmId, lParam, umPoint );
    m_pComputeThread->ReleaseComputationLock( );

    return bRes;
}

bool NNetController::processUIcommand( int const wmId, LPARAM const lParam )
{
    switch ( wmId )
    {

    case IDM_PERF_WINDOW:
    case IDM_CRSR_WINDOW:
    case IDM_DESC_WINDOW:
    case IDM_MINI_WINDOW:
    case IDM_MONITOR_WINDOW:
    case IDM_PARAM_WINDOW:
        ::SendMessage( m_pWinManager->GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
        break;

    case IDM_CENTER_MODEL:
        m_pComputeThread->LockComputation( );
        m_pMainWindow->CenterModel( );
        break;

    case IDM_SLOWER:
        if ( ! m_pSlowMotionRatio->IncRatio( ) )
            MessageBeep( MB_ICONWARNING );
        break;

    case IDM_FASTER:
        if ( ! m_pSlowMotionRatio->DecRatio( ) )
            MessageBeep( MB_ICONWARNING );
        break;

    case IDD_ARROWS_ON:
        m_arrowAnimation.SetTarget( Pipe::STD_ARROW_SIZE );
        break;

    case IDD_ARROWS_OFF:
        m_arrowAnimation.SetTarget( 0.0_MicroMeter );
        break;

    case IDD_SOUND_ON:
        m_pSound->On();
        break;

    case IDD_SOUND_OFF:
        m_pSound->Off();
        break;

    case IDD_AUTO_OPEN_ON:
        AutoOpen::On();
        break;

    case IDD_AUTO_OPEN_OFF:
        AutoOpen::Off();
        break;

    case IDD_STOP_ON_TRIGGER:                 // effects model, but seems to be secure  
        m_pSound->Play( TEXT("SNAP_IN_SOUND") ); 
        m_pModelCommands->ToggleStopOnTrigger( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDM_REFRESH:
        m_pMainWindow->Notify( lParam != 0 );
        break;

    default:
        return false; // command has not been processed
    }

    return true;  // command has been processed
}

void NNetController::pulseRateDlg( ShapeId const id )
{
    fHertz  const fOldValue { m_pModelReaderInterface->GetPulseFrequency( id ) };
    if ( fOldValue.IsNull() )
        return;
    HWND    const hwndParent { m_pMainWindow->GetWindowHandle() };
    wstring const header     { GetParameterName ( tParameter::pulseRate ) }; 
    wstring const unit       { GetParameterUnit ( tParameter::pulseRate ) };
    fHertz  const fNewValue  { StdDialogBox::Show( hwndParent, fOldValue.GetValue(), header, unit ) };
    if ( fNewValue != fOldValue )
        m_pModelCommands->SetPulseRate( id, fNewValue );
}

void NNetController::triggerSoundDlg( ShapeId const id )
{
    ShapeType const type { m_pModelReaderInterface->GetShapeType(id) };
    if ( ! type.IsAnyNeuronType() )
        return;

    TriggerSoundDialog dialog( m_pSound, m_pModelReaderInterface->GetTriggerSound( id ) );
    dialog.Show( m_pMainWindow->GetWindowHandle() );
    m_pModelCommands->SetTriggerSound( id, dialog.GetSound() );
}

bool NNetController::processModelCommand( int const wmId, LPARAM const lParam, MicroMeterPoint const umPoint )
{
    switch ( wmId )
    {
    case IDM_ABOUT:
        ShowAboutBox( m_pMainWindow->GetWindowHandle() );
        break;

    case IDM_UNDO:
        m_pModelCommands->UndoCommand();
        break;

    case IDM_REDO:
        m_pModelCommands->RedoCommand();
        break;

    case IDM_SAVE_MODEL:
        if ( m_pStorage->SaveModel( ) )
            m_pPreferences->WritePreferences( m_pStorage->GetModelPath() );
        break;

    case IDM_SAVE_MODEL_AS:
        m_pComputeThread->StopComputation( );
        if ( m_pStorage->SaveModelAs( ) )
            m_pPreferences->WritePreferences( m_pStorage->GetModelPath() );
        break;

    case IDM_COPY_SELECTION:
        m_pModelCommands->CopySelection( );
        break;

    case IDM_DELETE_SELECTION:
        m_pModelCommands->DeleteSelection( );
        break;

    case IDM_CLEAR_BEEPERS:
        m_pModelCommands->ClearBeepers( );
        break;

    case IDM_SELECT_ALL_BEEPERS:
        m_pModelCommands->SelectAllBeepers( );
        break;

    case IDD_PULSE_RATE:
        pulseRateDlg( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDD_TRIGGER_SOUND_DLG:
        triggerSoundDlg( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDM_NNET_REFRESH_RATE:
        m_pMainWindow->PostMessage( WM_COMMAND, IDD_REFRESH_RATE_DIALOG, 0 );
        break;

    case IDD_CONNECT:
        m_pSound->Play( TEXT("SNAP_IN_SOUND") ); 
        m_pModelCommands->Connect( m_pMainWindow->GetHighlightedShapeId(), m_pMainWindow->GetSuperHighlightedShapeId() );
        break;

    case IDM_DELETE:   // keyboard delete key
        if ( m_pMainWindow->GetHighlightedShapeId() == NO_SHAPE )
            break;
        [[fallthrough]];

    case IDD_DELETE_SHAPE:
        m_pSound->Play( TEXT("DISAPPEAR_SOUND") ); 
        m_pModelCommands->DeleteShape( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDD_DISCONNECT:
        m_pSound->Play( TEXT("UNLOCK_SOUND") ); 
        m_pModelCommands->Disconnect( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDD_INSERT_NEURON:
        m_pModelCommands->InsertNeuron( m_pMainWindow->GetHighlightedShapeId(), umPoint );
        break;

    case IDD_NEW_NEURON:
        m_pModelCommands->NewNeuron( umPoint );
        break;

    case IDD_NEW_INPUT_NEURON:
        m_pModelCommands->NewInputNeuron( umPoint );
        break;

    case IDD_APPEND_INPUT_NEURON:
        m_pModelCommands->AppendInputNeuron( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDD_APPEND_NEURON:
        m_pModelCommands->AppendNeuron( m_pMainWindow->GetHighlightedShapeId() );
        break;

    case IDD_ADD_OUTGOING2KNOT:
        m_pModelCommands->AddOutgoing2Knot( m_pMainWindow->GetHighlightedShapeId(), umPoint );
        break;

    case IDD_ADD_INCOMING2KNOT:
        m_pModelCommands->AddIncoming2Knot( m_pMainWindow->GetHighlightedShapeId(), umPoint );
        break;

    case IDD_ADD_OUTGOING2PIPE:
        m_pModelCommands->AddOutgoing2Pipe( m_pMainWindow->GetHighlightedShapeId(), umPoint );
        break;

    case IDD_ADD_INCOMING2PIPE:
        m_pModelCommands->AddIncoming2Pipe( m_pMainWindow->GetHighlightedShapeId(), umPoint );
        break;

    case IDD_ATTACH2MONITOR:
        m_pMonitorWindow->AddSignal( m_pMainWindow->GetHighlightedShapeId() );
        ::SendMessage( m_pWinManager->GetHWND( IDM_MONITOR_WINDOW ), WM_COMMAND, IDM_WINDOW_ON, 0 );
        break;

    case IDD_NEW_SENSOR:
        m_pModelCommands->NewSensor( umPoint );
        ::SendMessage( m_pWinManager->GetHWND( IDM_MONITOR_WINDOW ), WM_COMMAND, IDM_WINDOW_ON, 0 );
        break;

    case IDM_ANALYZE_LOOPS:
    {
        m_pModelCommands->AnalyzeLoops( );
        MicroMeterRect rect { ModelAnalyzer::GetEnclosingRect() };
        if ( rect.IsNotEmpty() )
            m_pMainWindow->CenterAndZoomRect( rect, 2.0f );
    }
    break;

    case IDM_ANALYZE_ANOMALIES:
        {
            m_pModelCommands->AnalyzeAnomalies( );
            MicroMeterRect rect { ModelAnalyzer::GetEnclosingRect() };
            if ( rect.IsNotEmpty() )
                m_pMainWindow->CenterAndZoomRect( rect, 2.0f );
        }
        break;

    case IDM_DESELECT_ALL:
    case IDM_ESCAPE:
        m_pModelCommands->SelectAll( tBoolOp::opFalse );
        Script::StopProcessing();
        break;

    case IDM_SELECT_SHAPE:
        m_pModelCommands->SelectShape( m_pMainWindow->GetHighlightedShapeId(), tBoolOp::opTrue );
        break;

    case IDM_DESELECT_SHAPE:
        m_pModelCommands->SelectShape( m_pMainWindow->GetHighlightedShapeId(), tBoolOp::opFalse );
        break;

    case IDM_SELECT_ALL:
        m_pModelCommands->SelectAll( tBoolOp::opTrue );
        break;

    case IDM_SELECT_SUBTREE:
        m_pModelCommands->SelectSubtree( m_pMainWindow->GetHighlightedShapeId(), tBoolOp::opTrue );
        break;

    default:
        return false;
    }

    return true;
}
