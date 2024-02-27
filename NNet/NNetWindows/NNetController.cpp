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
import Scanner;
import SaveCast;
import Observable;
import StdDialogBox;
import WinManager;
import FatalError;
import ScriptFile;
import Win32_Util;
import NNetCommands;
import UndoCommand;
import RedoCommand;
import CommandStack;
import NNetModel;
import Preferences;
import NNetPreferences;
import :Compute;
import :MainWindow;

using std::to_wstring;
using std::wcout;
using std::endl;

void NNetController::Initialize
(
    Compute         * const pCompute,
    SlowMotionRatio * const pSlowMotionRatio
) 
{
    m_pSlowMotionRatio = pSlowMotionRatio;
    m_pCompute         = pCompute;
    m_hCrsrWait        = LoadCursor(NULL, IDC_WAIT);
}

NNetController::~NNetController()
{
    m_pNMRI            = nullptr;
    m_pSlowMotionRatio = nullptr;
    m_pCompute   = nullptr;
    m_hCrsrWait        = nullptr;
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

    if (processUIcommand(wmId, lParam)) // handle all commands that affect the UI
        return true;                    // but do not concern the model  

    if (m_pNMRI->ModelLocked())
        return true;

    bool const bRunning = m_pCompute->IsRunning();
    if (bRunning)
    	m_pCompute->StopComputation();
    try
    {
        bRes = processModelCommand(wmId, lParam, umPoint);
    }
    catch (NobException const & e)
    {
        wcout << COMMENT_START << L"command failed, id =  " << wmId << L", lparam =  "<< lParam << endl;
        m_pNMRI->DumpModel(__FILE__, __LINE__);
        FatalError::Happened(9, L"Invalid NobId: " + to_wstring(e.m_id.GetValue()));
    }
    if (bRunning)
    	m_pCompute->StartComputation();

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
        WinManager::SendCommand(RootWinId(wmId), IDM_WINDOW_ON);
        break;

    case IDM_SLOWER:
        if (! m_pSlowMotionRatio->IncRatio())
            MessageBeep(MB_ICONWARNING);
        break;

    case IDM_FASTER:
        if (! m_pSlowMotionRatio->DecRatio())
            MessageBeep(MB_ICONWARNING);
        break;

    case IDM_MAX_SPEED:
        m_pSlowMotionRatio->MaxSpeed(tBoolOp::opToggle);
        break;

    case IDD_ARROWS:  // comes from menu
        NNetPreferences::m_bArrows.Toggle(); // with animation
        break;

    case IDD_SENSOR_PNTS:
        NNetPreferences::m_bSensorPoints.Toggle();
        break;

    case IDD_PERF_MON_MODE:
        BaseWindow::m_bPerfMonMode.Toggle();
        WinManager::SetCaptions();
        break;

    case IDM_INPUT_CABLES_ALL:
        NNetPreferences::SetInputCablesVisibility(NNetPreferences::tInputCablesVisibility::all);
        break;

    case IDM_INPUT_CABLES_NONSTD:
        NNetPreferences::SetInputCablesVisibility(NNetPreferences::tInputCablesVisibility::nonStd);
        break;

    case IDM_INPUT_CABLES_ACTIVE:
        NNetPreferences::SetInputCablesVisibility(NNetPreferences::tInputCablesVisibility::active);
        break;

    case IDM_INPUT_CABLES_NONE:
        NNetPreferences::SetInputCablesVisibility(NNetPreferences::tInputCablesVisibility::none);
        break;

    case IDD_SCAN_AREA:
        NNetPreferences::m_bScanArea.Toggle();
        break;

    case IDD_MODEL_FRONT:
        NNetPreferences::m_bModelFront.Toggle();
        break;

    case IDD_FILTER:
        NNetPreferences::m_bFilter.Toggle();
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
        WinManager::SendCommand(RootWinId(IDM_SIG_DESIGNER),IDM_WINDOW_ON);
        break;

    case IDD_SELECT_SIGNAL_GENERATOR:
        SetActiveSigGenCmd::Push(SigGenId(Cast2Int(lParam)));
        WinManager::SendCommand(RootWinId(IDM_SIG_DESIGNER), IDM_WINDOW_ON);
        break;

    case IDD_ADD_EEG_SENSOR:
        AddSensorCmd::Push(MicroMeterCircle(umPoint, NEURON_RADIUS * 5), TrackNr(0));
        WinManager::SendCommand(RootWinId(IDM_MONITOR_WINDOW), IDM_WINDOW_ON);
        break;

    case IDM_COPY_SELECTION:
    case IDM_DELETE:   // keyboard delete key
    case IDM_ESCAPE:
        WinManager::SendCommand(RootWinId(IDM_MAIN_WINDOW), wmId);
        Script::StopProcessing();
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

    case IDD_DELETE_SIGNAL:
        deleteSignal();
        break;

    case IDD_ADD_TRACK:
        InsertTrackCommand::Push(TrackNr(Cast2Int(lParam)));
        break;

    case IDD_DELETE_TRACK:
        DeleteTrackCommand::Push(TrackNr(Cast2Int(lParam)));
        break;

    default:
        return false;
    }

    return true;
}
void NNetController::deleteSignal()
{
    Signal           const* pSignal     { m_pNMRI->GetMonitorDataC().GetHighlightedSignal() };
    NNetSignal       const* pNNetSignal { static_cast<NNetSignal const*>(pSignal) };
    NNetSignalSource const* pSigSrc     { pNNetSignal->GetSignalSource() };
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
