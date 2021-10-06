// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include <exception>
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "Analyzer.h"
#include "Observable.h"
#include "Preferences.h"
#include "SlowMotionRatio.h"
#include "NobException.h"
#include "NNetModelExporter.h"
#include "ComputeThread.h"
#include "CommandStack.h"
#include "Signal.h"
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

using std::to_wstring;

void NNetController::Initialize
(
    NNetModelExporter        * const pModelExporter,
    MainWindow               * const pMainWindow,
    WinManager               * const pWinManager,
    NNetModelReaderInterface * const pMRI,
    NNetModelCommands        * const pModelCommands,
    ComputeThread            * const pComputeThread,
    SlowMotionRatio          * const pSlowMotionRatio,
    DisplayFunctor           * const func,
    Sound                    * const pSound,
    Preferences              * const pPreferences,
    CommandStack             * const pCommandStack,
    MonitorWindow            * const pMonitorWindow
) 
{
    m_pModelExporter    = pModelExporter;
    m_pMainWindow       = pMainWindow;
    m_pWinManager       = pWinManager;
    m_pNMRI             = pMRI;
    m_pModelCommands    = pModelCommands;
    m_pSlowMotionRatio  = pSlowMotionRatio;
    m_pComputeThread    = pComputeThread;
    m_pStatusBarDisplay = func;
    m_pSound            = pSound;
    m_pPreferences      = pPreferences;
    m_pCommandStack     = pCommandStack;
    m_pMonitorWindow    = pMonitorWindow;
    m_hCrsrWait         = LoadCursor(NULL, IDC_WAIT);
}

NNetController::~NNetController()
{
    m_pModelExporter    = nullptr;
    m_pMainWindow       = nullptr;
    m_pWinManager       = nullptr;
    m_pNMRI             = nullptr;
    m_pModelCommands    = nullptr;
    m_pSlowMotionRatio  = nullptr;
    m_pComputeThread    = nullptr;
    m_pStatusBarDisplay = nullptr;
    m_hCrsrWait         = nullptr;
    m_pSound            = nullptr;
    m_pPreferences      = nullptr;
    m_pCommandStack     = nullptr;
    m_pMonitorWindow    = nullptr;
}

bool NNetController::HandleCommand(int const wmId, LPARAM const lParam, MicroMeterPnt const umPoint)
{
    bool bRes { false };

    if (wmId == IDM_FATAL_ERROR)
    {
        FatalError::Happened(static_cast<long>(lParam), L"unknown");
    }
    else if (wmId == IDM_BLOCK_UI)
    {
        m_bBlockedUI = (lParam != 0);
    }

    if (processUIcommand(wmId, lParam)) // handle all commands that affect the UI
        return true;                    // but do not concern the model  

    if (! m_bBlockedUI)
    {
        m_pComputeThread->LockComputation();
        try
        {
            bRes = processModelCommand(wmId, lParam, umPoint);
        }
        catch (NobException e)
        {
            wcout << Scanner::COMMENT_START << L"command failed, id =  " << wmId << L", lparam =  "<< lParam << endl;
            m_pNMRI->DUMP();
            FatalError::Happened(9, L"Invalid NobId: " + to_wstring(e.m_id.GetValue()));
        }
        m_pComputeThread->ReleaseComputationLock();
    }

    return bRes;
}

bool NNetController::processUIcommand(int const wmId, LPARAM const lParam)
{
    switch (wmId)
    {
    case IDM_ABOUT:
        ShowAboutBox(m_pMainWindow->GetWindowHandle());
        break;

    case IDM_PERF_WINDOW:
    case IDM_CRSR_WINDOW:
    case IDM_DESC_WINDOW:
    case IDM_MINI_WINDOW:
    case IDM_MONITOR_WINDOW:
    case IDM_PARAM_WINDOW:
        ::SendMessage(m_pWinManager->GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
        break;

    case IDM_CENTER_MODEL:
        m_pMainWindow->CenterModel();
        break;

    case IDM_SLOWER:
        if (! m_pSlowMotionRatio->IncRatio())
            MessageBeep(MB_ICONWARNING);
        break;

    case IDM_FASTER:
        if (! m_pSlowMotionRatio->DecRatio())
            MessageBeep(MB_ICONWARNING);
        break;

    case IDD_ARROWS_ON:
        m_pMainWindow->ShowArrows(true);
        break;

    case IDD_ARROWS_OFF:
        m_pMainWindow->ShowArrows(false);
        break;

    case IDX_PLAY_SOUND:
        m_pSound->Play(reinterpret_cast<wchar_t *>(lParam)); 
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

    case IDM_REFRESH:
        m_pMainWindow->Notify(lParam != 0);
        break;

    default:
        return false; // command has not been processed
    }

    return true;  // command has been processed
}

void NNetController::pulseRateDlg(NobId const id)
{
    fHertz  const fOldValue { m_pNMRI->GetPulseFrequency(id) };
    if (fOldValue.IsNull())
        return;
    HWND    const hwndParent { m_pMainWindow->GetWindowHandle() };
    wstring const header     { ParamType::GetName(ParamType::Value::pulseRate) }; 
    wstring const unit       { ParamType::GetUnit(ParamType::Value::pulseRate) };
    fHertz  const fNewValue  { StdDialogBox::Show(hwndParent, fOldValue.GetValue(), header, unit) };
    if (fNewValue != fOldValue)
        m_pModelCommands->SetPulseRate(id, fNewValue);
}

void NNetController::triggerSoundDlg(NobId const id)
{
    NobType const type { m_pNMRI->GetNobType(id) };
    if (! type.IsAnyNeuronType())
        return;

    TriggerSoundDialog dialog(m_pSound, m_pNMRI->GetTriggerSound(id));
    dialog.Show(m_pMainWindow->GetWindowHandle());
    m_pModelCommands->SetTriggerSound(id, dialog.GetSound());
}

bool NNetController::processModelCommand(int const wmId, LPARAM const lParam, MicroMeterPnt const umPoint)
{
    switch (wmId)
    {
    case IDM_UNDO:
        m_pModelCommands->UndoCommand();
        break;

    case IDM_REDO:
        m_pModelCommands->RedoCommand();
        break;

    case IDM_ADD_IMPORTED_MODEL:
        m_pModelCommands->AddModel();
        break;

    case IDM_COPY_SELECTION:
        m_pModelCommands->CopySelection();
        break;

    case IDM_CLEAR_BEEPERS:
        m_pModelCommands->ClearBeepers();
        break;

    case IDM_SELECT_ALL_BEEPERS:
        m_pModelCommands->SelectAllBeepers();
        break;

    case IDD_PULSE_RATE:
        pulseRateDlg(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_TRIGGER_SOUND_DLG:
        triggerSoundDlg(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDM_NNET_REFRESH_RATE:
        m_pMainWindow->PostMessage(WM_COMMAND, IDD_REFRESH_RATE_DIALOG, 0);
        break;

    case IDD_CONNECT:
        m_pModelCommands->Connect(m_pMainWindow->GetHighlightedNobId(), m_pMainWindow->GetTargetNobId());
        break;

    case IDM_DELETE:   // keyboard delete key
        if (IsDefined(m_pMainWindow->GetHighlightedNobId()))
            processModelCommand(IDD_DELETE_NOB);
        else if (m_pMainWindow->AnyNobsSelected())
            processModelCommand(IDM_DELETE_SELECTION);
        break;

    case IDD_DELETE_NOB:
        m_pSound->Play(TEXT("DISAPPEAR_SOUND")); 
        m_pModelCommands->DeleteNob(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_DELETE_EEG_SENSOR:
        m_pSound->Play(TEXT("DISAPPEAR_SOUND")); 
        m_pModelCommands->DeleteSignal(m_pNMRI->GetMonitorData().GetHighlightedSignalId());
        break;

    case IDD_ADD_TRACK:
        m_pModelCommands->InsertTrack(TrackNr(Cast2Int(lParam)));
        m_pSound->Play(TEXT("SNAP_IN_SOUND")); 
        break;

    case IDD_DELETE_TRACK:
        m_pModelCommands->DeleteTrack(TrackNr(Cast2Int(lParam)));
        m_pSound->Play(TEXT("DISAPPEAR_SOUND")); 
        break;

    case IDM_DELETE_SELECTION:
        m_pModelCommands->DeleteSelection();
        break;

    case IDD_DISC_BASEKNOT:
        m_pSound->Play(TEXT("UNLOCK_SOUND")); 
        m_pModelCommands->DiscBaseKnot(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_DISC_IOCONNECTOR:
        m_pSound->Play(TEXT("UNLOCK_SOUND")); 
        m_pModelCommands->DiscIoConnector(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_SPLIT_NEURON:
        m_pSound->Play(TEXT("UNLOCK_SOUND")); 
        m_pModelCommands->SplitNeuron(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_INSERT_KNOT:
        m_pModelCommands->InsertKnot(m_pMainWindow->GetHighlightedNobId(), umPoint);
        break;

    case IDD_INSERT_NEURON:
        m_pModelCommands->InsertNeuron(m_pMainWindow->GetHighlightedNobId(), umPoint);
        break;

    case IDD_NEW_INPUT_NEURON:
        m_pModelCommands->NewInputNeuron(umPoint);
        break;

    case IDD_NEW_OUTPUT_NEURON:
        m_pModelCommands->NewOutputNeuron(umPoint);
        break;

    case IDD_APPEND_INPUT_NEURON:
        m_pModelCommands->AppendInputNeuron(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_APPEND_OUTPUT_NEURON:
        m_pModelCommands->AppendOutputNeuron(m_pMainWindow->GetHighlightedNobId());
        break;

    case IDD_ADD_OUTGOING2KNOT:
        m_pModelCommands->AddOutgoing2Knot(m_pMainWindow->GetHighlightedNobId(), umPoint);
        break;

    case IDD_ADD_INCOMING2KNOT:
        m_pModelCommands->AddIncoming2Knot(m_pMainWindow->GetHighlightedNobId(), umPoint);
        break;

    case IDD_ADD_OUTGOING2PIPE:
        m_pModelCommands->AddOutgoing2Pipe(m_pMainWindow->GetHighlightedNobId(), umPoint);
        break;

    case IDD_ADD_INCOMING2PIPE:
        m_pModelCommands->AddIncoming2Pipe(m_pMainWindow->GetHighlightedNobId(), umPoint);
        break;

    case IDD_ADD_EEG_SENSOR:
        m_pModelCommands->AddSignal(MicroMeterCircle(umPoint, NEURON_RADIUS * 5), TrackNr(0));
        m_pMonitorWindow->Show(true);
        m_pSound->Play(TEXT("SNAP_IN_SOUND")); 
        break;

    case IDM_ANALYZE_LOOPS:
        m_pModelCommands->AnalyzeLoops();
        m_pMainWindow->CenterSelection();
        break;

    case IDM_ANALYZE_ANOMALIES:
        m_pModelCommands->AnalyzeAnomalies();
        m_pMainWindow->CenterSelection();
        break;

    case IDM_DESELECT_ALL:
    case IDM_ESCAPE:
        m_pModelCommands->SelectAll(false);
        Script::StopProcessing();
        break;

    case IDM_SELECT_NOB:
        m_pModelCommands->SelectNob(m_pMainWindow->GetHighlightedNobId(), tBoolOp::opTrue);
        break;

    case IDM_DESELECT_NOB:
        m_pModelCommands->SelectNob(m_pMainWindow->GetHighlightedNobId(), tBoolOp::opFalse);
        break;

    case IDM_SELECT_ALL:
        m_pModelCommands->SelectAll(true);
        break;

    case IDM_SELECT_SUBTREE:
        m_pModelCommands->SelectSubtree(m_pMainWindow->GetHighlightedNobId(), true);
        break;

    case IDD_STOP_ON_TRIGGER:
        m_pModelCommands->ToggleStopOnTrigger(m_pMainWindow->GetHighlightedNobId());
        m_pSound->Play(TEXT("SNAP_IN_SOUND")); 
        break;

    default:
        return false;
    }

    return true;
}
