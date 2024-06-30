// NNetController.cpp
//
// NNetWindows

module;

#include <filesystem>
#include <string>
#include <iostream>
#include <exception>
#include "Windows.h"
#include "Resource.h"

module NNetWin32:NNetController;

import Win32_Util;
import Win32_Util_Resource;
import SlowMotionRatio;
import Scanner;
import ScriptFile;
import SaveCast;
import Observable;
import StdDialogBox;
import WinManager;
import FatalError;
import Commands;
import IoUtil;
import NNetCommands;
import NNetModel;
import NNetModelIO;
import NNetPreferences;
import :AppTitle;
import :Compute;
import :MainWindow;
import :NNetUtilities;
import :NNetInputOutputUI;

using std::filesystem::path;
using std::to_wstring;
using std::wcout;
using std::endl;

void NNetController::Initialize
(
    Compute         * const pCompute,
    SlowMotionRatio * const pSlowMotionRatio,
    AppTitle        * const pAppTitle
) 
{
    m_pSlowMotionRatio = pSlowMotionRatio;
    m_pCompute         = pCompute;
    m_pAppTitle        = pAppTitle;
    m_hCrsrWait        = LoadCursor(NULL,    IDC_WAIT);
	m_hCrsrArrow       = LoadCursor(nullptr, IDC_ARROW);
}

NNetController::~NNetController()
{
    m_pNMRI            = nullptr;
    m_pSlowMotionRatio = nullptr;
    m_pCompute         = nullptr;
    m_hCrsrWait        = nullptr;
}

void NNetController::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
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

    if (processUIcommand(wmId, lParam, umPoint)) // handle all commands that affect the UI
        return true;                             // but do not concern the model  

    if (m_pNMRI->ModelLocked())
    {
   		MessageBox(nullptr, L"Unlock model to perform this command", L"Model is locked", MB_OK);
        return true;
    }

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

bool NNetController::processUIcommand(int const wmId, LPARAM const lParam, MicroMeterPnt const umPoint)
{
    switch (wmId)
    {
    case IDM_SAVE_SCAN:
		m_pCompute->StopComputation();
		SaveScanAs();
		break;

	case IDM_SAVE_MODEL_AS:
		m_pCompute->StopComputation();
		if (SaveModelAs())
			Preferences::WritePreferences();
        break;

	case IDM_SAVE_MODEL:
		m_pCompute->StopComputation();
		if (SaveModel())
			Preferences::WritePreferences();
        break;

    case IDM_VIEWER_WINDOW:
    case IDM_LUT_DESIGNER:
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

	case IDD_SOUND:
		Preferences::m_bSound.Toggle();
		break;

    case IDD_SENSOR_PNTS:
        NNetPreferences::m_bSensorPoints.Toggle();
        break;

	case IDD_AUTO_OPEN:
		Preferences::m_bAutoOpen.Toggle();
		break;

    case IDD_PERF_MON_MODE:
        BaseWindow::m_bPerfMonMode.Toggle();
        WinManager::SetCaptions();
        break;

    case IDM_INPUT_CABLES_ALL:
        NNetPreferences::m_bInputCables.SetVisibility(ShowInputCables::tVisibility::all);
        break;

    case IDM_INPUT_CABLES_NONSTD:
        NNetPreferences::m_bInputCables.SetVisibility(ShowInputCables::tVisibility::nonStd);
        break;

    case IDM_INPUT_CABLES_ACTIVE:
        NNetPreferences::m_bInputCables.SetVisibility(ShowInputCables::tVisibility::active);
        break;

    case IDM_INPUT_CABLES_NONE:
        NNetPreferences::m_bInputCables.SetVisibility(ShowInputCables::tVisibility::none);
        break;

    case IDD_COLOR_MENU:
        Preferences::m_bColorMenu.Toggle();
        break;

    case IDD_SCAN_AREA:
        NNetPreferences::m_bScanArea.Toggle();
        break;

    case IDD_MODEL_FRONT:
        NNetPreferences::m_bModelFront.Toggle();
        break;

    case IDD_ASK_NOT_UNDOABLE:
        NNetPreferences::m_bAskNotUndoable.Toggle();
        break;

    case IDD_FILTER:
        NNetPreferences::m_bFilter.Toggle();
        break;

    case IDD_SELECT_SIGNAL_GENERATOR:
        SetActiveSigGenCmd::Push(SigGenId(Cast2Int(lParam)));
        WinManager::SendCommand(RootWinId(IDM_SIG_DESIGNER), IDM_WINDOW_ON);
        break;

    case IDD_ADD_EEG_SENSOR:
        AddSensorCmd::Push(MicroMeterCircle(umPoint, NEURON_RADIUS * 5), TrackNr(0));
        WinManager::SendCommand(RootWinId(IDM_MONITOR_WINDOW), IDM_WINDOW_ON);
        break;

    case IDD_ADD_TRACK:
        InsertTrackCommand::Push(TrackNr(Cast2Int(lParam)));
        break;

    case IDD_DELETE_TRACK:
        DeleteTrackCommand::Push(TrackNr(Cast2Int(lParam)));
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

    case IDM_COPY_SELECTION:
    case IDM_DELETE:   // keyboard delete key
    case IDM_ESCAPE:
        WinManager::SendCommand2MainWin(wmId);
        Script::StopProcessing();
        break;

   	case IDM_ADD_MODULE:
		NNetModelIO::Import
		(
			AskModelFileName(tFileMode::read), 
			NNetInputOutputUI::CreateNew
            (
                IDM_ADD_IMPORTED_MODEL, 
                WinManager::GetHWND(RootWinId(IDM_APPL_WINDOW))
            )
		);
        break;

   case IDM_ADD_IMPORTED_MODEL:
        AddModuleCommand::Push();
        break;

    default:
        return false;
    }

    return true;
}

bool NNetController::SaveModelAs()
{
	wstring wstrModelPath = AskModelFileName(tFileMode::write);
	if (wstrModelPath == L"")
		return false;
	m_pNMRI->SetModelFilePath(wstrModelPath);
	WriteModel(wstrModelPath);
	return true;
}

bool NNetController::SaveScanAs()
{
    path fullPath { m_pNMRI->GetModelFilePath() };
    path fileName { fullPath.stem() };
	wstring const wstrModelPath = AskScanFileName(fileName, tFileMode::write);
	if (wstrModelPath == L"")
		return false;
	WriteModel(wstrModelPath);
	return true;
}

bool NNetController::SaveModel()
{
	wstring wstrModelPath { m_pNMRI->GetModelFilePath() };
	if (wstrModelPath == L"")
	{
		return SaveModelAs();
	}
	else
	{
		WriteModel(wstrModelPath);
		return true;
	}
}

void NNetController::WriteModel(wstring const& wstrPath)
{
	SetCursor(m_hCrsrWait);
	HWND const hwndApp { WinManager::GetHWND(RootWinId(IDM_APPL_WINDOW)) };
	NNetModelIO::Export
	(
		wstrPath,
		*m_pNMRI, 
		NNetInputOutputUI::CreateNew(IDX_EXPORT_FINISHED, hwndApp)
	);
	Preferences::WritePreferences();
	m_pAppTitle->SetUnsavedChanges(false);
	SetCursor(m_hCrsrArrow);
}

bool NNetController::AskAndSave()
{
	if (m_pAppTitle->AnyUnsavedChanges())
	{
		int iRes = MessageBox(nullptr, L"Save now?", L"Unsaved changes", MB_YESNOCANCEL);
		if (iRes == IDYES)
			SaveModel();
		else if (iRes == IDNO)
			return true;
		else if (iRes == IDCANCEL)
			return false;
	}
	return true;
}
