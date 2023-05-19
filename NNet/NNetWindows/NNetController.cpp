// NNetController.cpp
//
// NNetWindows

module;

#include <string>
#include <iostream>
#include <exception>
#include "Windows.h"
#include "Resource.h"

module NNetWin32:NNetController;

import Win32_Util_Resource;
import SlowMotionRatio;
import SaveCast;
import Observable;
import Win32_Sound;
import StdDialogBox;
import WinManager;
import FatalError;
import ScriptFile;
import Win32_Util;
import NNetCommands;
import Commands;
import AutoOpen;
import NNetModel;
import :ComputeThread;
import :MainWindow;
import :Preferences;
import :MonitorWindow;

using std::to_wstring;
using std::wcout;
using std::endl;

void NNetController::Initialize
(
    WinManager      * const pWinManager,
    ComputeThread   * const pComputeThread,
    SlowMotionRatio * const pSlowMotionRatio,
    Sound           * const pSound,
    Preferences     * const pPreferences,
    CommandStack    * const pCommandStack,
    MonitorWindow   * const pMonitorWindow
) 
{
    m_pWinManager      = pWinManager;
    m_pSlowMotionRatio = pSlowMotionRatio;
    m_pComputeThread   = pComputeThread;
    m_pSound           = pSound;
    m_pPreferences     = pPreferences;
    m_pCommandStack    = pCommandStack;
    m_pMonitorWindow   = pMonitorWindow;
    m_hCrsrWait        = LoadCursor(NULL, IDC_WAIT);
}

NNetController::~NNetController()
{
    m_pWinManager      = nullptr;
    m_pNMRI            = nullptr;
    m_pSlowMotionRatio = nullptr;
    m_pComputeThread   = nullptr;
    m_hCrsrWait        = nullptr;
    m_pSound           = nullptr;
    m_pPreferences     = nullptr;
    m_pCommandStack    = nullptr;
    m_pMonitorWindow   = nullptr;
}

void NNetController::SetModelInterface(NNetModelReaderInterface * const pNMRI)
{
    m_pNMRI = pNMRI;
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
        catch (NobException const & e)
        {
            wcout << Scanner::COMMENT_START << L"command failed, id =  " << wmId << L", lparam =  "<< lParam << endl;
            m_pNMRI->DumpModel(__FILE__, __LINE__);
            FatalError::Happened(9, L"Invalid NobId: " + to_wstring(e.m_id.GetValue()));
        }
        m_pComputeThread->UnlockComputation();
    }

    return bRes;
}

bool NNetController::processUIcommand(int const wmId, LPARAM const lParam)
{
    switch (wmId)
    {

    case IDM_SIG_DESIGNER:
    case IDM_PERF_WINDOW:
    case IDM_CRSR_WINDOW:
    case IDM_DESC_WINDOW:
    case IDM_MINI_WINDOW:
    case IDM_MONITOR_WINDOW:
    case IDM_PARAM_WINDOW:
        ::SendMessage(m_pWinManager->GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
        break;

    case IDM_SLOWER:
        if (! m_pSlowMotionRatio->IncRatio())
            MessageBeep(MB_ICONWARNING);
        break;

    case IDM_FASTER:
        if (! m_pSlowMotionRatio->DecRatio())
            MessageBeep(MB_ICONWARNING);
        break;

    case IDD_SCALES_OFF:
        m_pPreferences->SetScales(false);
        break;

    case IDD_SCALES_ON:
        m_pPreferences->SetScales(true);
        break;

    case IDD_SCALES:
        ::SendMessage(m_pWinManager->GetHWND(IDM_MAIN_WINDOW), WM_COMMAND, wmId, 0);
        break;

    case IDD_ARROWS_ON:
        m_pPreferences->SetArrows(true);
        break;

    case IDD_ARROWS_OFF:
        m_pPreferences->SetArrows(false);
        break;

    case IDD_SENSOR_PNTS_ON:
        m_pPreferences->SetSensorPoints(true);
        break;

    case IDD_SENSOR_PNTS_OFF:
        m_pPreferences->SetSensorPoints(false);
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

    case IDD_PERF_MON_MODE_ON:
        BaseWindow::SetPerfMonMode(true);
        m_pWinManager->SetCaptions();
        break;

    case IDD_PERF_MON_MODE_OFF:
        BaseWindow::SetPerfMonMode(false);
        m_pWinManager->SetCaptions();
        break;

    case IDM_INPUT_CABLES_ALL:   
        m_pPreferences->SetInputCablesVisibility(Preferences::tInputCablesVisibility::all);
        break;

    case IDM_INPUT_CABLES_NONSTD:
        m_pPreferences->SetInputCablesVisibility(Preferences::tInputCablesVisibility::nonStd);
        break;

    case IDM_INPUT_CABLES_ACTIVE:
        m_pPreferences->SetInputCablesVisibility(Preferences::tInputCablesVisibility::active);
        break;

    case IDM_INPUT_CABLES_NONE:
        m_pPreferences->SetInputCablesVisibility(Preferences::tInputCablesVisibility::none);
        break;

    default:
        return false; // command has not been processed
    }

    return true;  // command has been processed
}

bool NNetController::processModelCommand(int const wmId, LPARAM const lParam, MicroMeterPnt const umPoint)
{
    switch (wmId)
    {
    case IDD_NEW_SIGNAL_GENERATOR:
        NewSigGenCmd::Push();
        ::SendMessage(m_pWinManager->GetHWND(IDM_SIG_DESIGNER), WM_COMMAND, IDM_WINDOW_ON, 0);
        break;

    case IDD_SELECT_SIGNAL_GENERATOR:
        SetActiveSigGenCmd::Push(SigGenId(Cast2Int(lParam)));
        ::SendMessage(m_pWinManager->GetHWND(IDM_SIG_DESIGNER), WM_COMMAND, IDM_WINDOW_ON, 0);
        break;

    case IDM_COPY_SELECTION:
    case IDM_DELETE:   // keyboard delete key
        ::SendMessage(m_pWinManager->GetHWND(IDM_MAIN_WINDOW), WM_COMMAND, wmId, 0);
        break;

    case IDM_UNDO:
        UndoCommand::Push();
        break;

    case IDM_REDO:
        RedoCommand::Push();
        break;

    case IDM_ADD_IMPORTED_MODEL:
        AddModuleCommand::Push();
        break;

    case IDD_ADD_EEG_SENSOR:
        AddSensorCmd::Push(MicroMeterCircle(umPoint, NEURON_RADIUS * 5), TrackNr(0));
        m_pMonitorWindow->Show(true);
        break;

    case IDD_DELETE_SIGNAL:
        deleteSignal(static_cast<NNetSignal const *>(m_pNMRI->GetMonitorDataC().GetHighlightedSignal()));
        break;

    case IDD_ADD_TRACK:
        InsertTrackCommand::Push(TrackNr(Cast2Int(lParam)));
        break;

    case IDD_DELETE_TRACK:
        DeleteTrackCommand::Push(TrackNr(Cast2Int(lParam)));
        break;

    case IDM_ESCAPE:
        ::SendMessage(m_pWinManager->GetHWND(IDM_MAIN_WINDOW), WM_COMMAND, wmId, 0);
        Script::StopProcessing();
        break;

    case IDM_TRIGGER_STIMULUS:
        m_pMonitorWindow->StimulusTriggered();
        StartStimulusCmd::Push();
        break;

    default:
        return false;
    }

    return true;
}
void NNetController::deleteSignal(NNetSignal const* pSignal)
{
    NNetSignalSource const* pSigSrc { pSignal->GetSignalSource() };
    switch (pSigSrc->SignalSourceType())
    {
        using enum NNetSignalSource::Type;
        case macroSensor:
        {
            Sensor const* pSensor  { static_cast<Sensor const*>(pSigSrc) };
            SensorId      idSensor { m_pNMRI->GetSensorList().GetSensorId(*pSensor) };
            DelSensorCmd::Push(idSensor);
            break;
        }
        case microSensor:
        {
            MicroSensor const * pMicroSensor { static_cast<MicroSensor const*>(pSigSrc) };
            NobId       const   idNob        { pMicroSensor->GetNobId() };
            DelMicroSensorCmd::Push(idNob);
            break;
        }
    }
}
