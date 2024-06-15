// NNetAppWindow.cpp
//
// NNetWindows

module;

#include <bit>
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <source_location>
#include "Resource.h"

module NNetAppWindow;

import Win32_Util_Resource;
import Types;
import Util;
import Trace;
import Direct2D;
import Vector2D;
import ObserverInterface;
import WinCommand;
import AboutBox;
import FatalError;
import MessagePump;
import ScriptFile;
import BaseWindow;
import Win32_Util;
import Win32_PIXEL;
import ScriptStack;
import Signals;
import Script;
import Symtab;
import NNetModel;
import NNetCommands;
import NNetSignals;
import NNetPreferences;
import StatusBar;
import NNetWin32;

using std::endl;
using std::wcout;
using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::wostringstream;
using std::filesystem::path;
using std::source_location;
using std::wostream;

using namespace std::chrono;
using std::to_wstring;
using std::bit_cast;
using std::wcout;

NNetAppWindow::NNetAppWindow(wstring const &wstrProductName, MessagePump & pump)
{
	m_pwstrProductName = &wstrProductName;
	m_aboutBox.SetProductName(wstrProductName);

	WinManager       ::Initialize();
	Model            ::Initialize(&m_lockModelObservable, &m_staticModelObservable);
	Signal           ::Initialize(&m_dynamicModelObservable);
	WinCommand       ::Initialize(&m_cmdStack);
	NNetCommand      ::Initialize(&m_sound);
	CoordAnimationCmd::Initialize(&m_coordObservable);
	NNetPreferences  ::Initialize();

	m_simuRunning   .Initialize(&m_compute);
	m_cmdStack      .Initialize(&m_staticModelObservable);
	m_NNetController.Initialize(&m_compute, &m_slowMotionRatio, &m_appTitle);

	NNetModelIO::AddModelWrapper<MonitorScrollState>(L"MonitorScrollState");
	Nob::SetColorLut(NNetPreferences::m_colorLutVoltage);

	m_hwndApp = StartBaseWindow
	(
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	m_appTitle.Initialize(m_hwndApp, *m_pwstrProductName);

	m_signalDesigner.Initialize
	(
		m_hwndApp, 
		m_simuRunning, 
		m_runObservable, 
		m_dynamicModelObservable
	);

	m_compute.Initialize
	(
		&m_slowMotionRatio,
		&m_runObservable,
		&m_performanceObservable,
		&m_dynamicModelObservable
	);

	m_upModel = m_nmwi.CreateNewModel();
	m_pNMRI   = static_cast<NNetModelReaderInterface const *>(&m_nmwi);
	m_nmwi.SetDescriptionUI(m_descWindow);
	m_upModel->m_upSigGenList->SetActiveSigGenObservable(m_activeSigGenObservable);

	m_mainNNetWindow   .SetRefreshRate(300ms);
	m_miniNNetWindow   .SetRefreshRate(500ms);
	m_monitorWindow    .SetRefreshRate( 20ms);
	m_crsrWindow       .SetRefreshRate(300ms);
	m_performanceWindow.SetRefreshRate(500ms);
	m_statusBar        .SetRefreshRate(300ms);

	m_appMenu          .Start(m_hwndApp, m_compute, m_cmdStack);
	m_statusBar        .Start(m_hwndApp);
	m_descWindow       .Start(m_hwndApp);
	m_crsrWindow       .Start(m_hwndApp, &m_mainNNetWindow, &m_monitorWindow);
	m_parameterDlg     .Start(m_hwndApp);
	m_performanceWindow.Start(m_hwndApp, &m_compute, &m_slowMotionRatio, &m_atDisplay);
	m_monitorWindow    .Start(m_hwndApp, m_simuRunning, m_sound, m_staticModelObservable);
	m_colLutWindow     .Start(m_hwndApp, &NNetPreferences::m_colorLutScan);
	m_undoRedoMenu     .Start(& m_appMenu);

	m_monitorWindow    .SetHighSigObservable(m_highlightSigObservable);
	setModelInterface();

	m_signalDesigner.RegisterAtSigGen(m_nmwi.GetSigGenIdSelected());
	pump.RegisterWindow(m_descWindow.GetWindowHandle(), true);

	m_mainNNetWindow.Start
	(
		m_hwndApp, 
		false,
		m_NNetController,
		m_cursorPosObservable,
		m_coordObservable,
		m_staticModelObservable,
		m_compute,
		& m_atDisplay,
		& m_monitorWindow
	);

	m_miniNNetWindow.Start(m_hwndApp, true,	m_NNetController);

	m_miniNNetWindow.ObservedNNetWindow(& m_mainNNetWindow);  // mini window observes main window

	WinManager::AddWindow(L"IDM_APPL_WINDOW",    RootWinId(IDM_APPL_WINDOW   ), m_hwndApp,                      true,  true );
	WinManager::AddWindow(L"IDM_STATUS_BAR",     RootWinId(IDM_STATUS_BAR    ), m_statusBar.GetWindowHandle(),  false, false);
	WinManager::AddWindow(L"IDM_CRSR_WINDOW",    RootWinId(IDM_CRSR_WINDOW   ), m_crsrWindow,                   true,  false);
	WinManager::AddWindow(L"IDM_DESC_WINDOW",    RootWinId(IDM_DESC_WINDOW   ), m_descWindow.GetWindowHandle(), true,  true );
	WinManager::AddWindow(L"IDM_MAIN_WINDOW",    RootWinId(IDM_MAIN_WINDOW   ), m_mainNNetWindow,               true,  false);
	WinManager::AddWindow(L"IDM_MINI_WINDOW",    RootWinId(IDM_MINI_WINDOW   ), m_miniNNetWindow,               true,  true );
	WinManager::AddWindow(L"IDM_MONITOR_WINDOW", RootWinId(IDM_MONITOR_WINDOW), m_monitorWindow,                true,  true );
	WinManager::AddWindow(L"IDM_PARAM_WINDOW",   RootWinId(IDM_PARAM_WINDOW  ), m_parameterDlg,                 true,  false);
	WinManager::AddWindow(L"IDM_PERF_WINDOW",    RootWinId(IDM_PERF_WINDOW   ), m_performanceWindow,            true,  false);
	WinManager::AddWindow(L"IDM_SIG_DESIGNER",   RootWinId(IDM_SIG_DESIGNER  ), m_signalDesigner,               true,  true );
	WinManager::AddWindow(L"IDM_LUT_DESIGNER",   RootWinId(IDM_LUT_DESIGNER  ), m_colLutWindow,                 true,  true );
	WinManager::AddWindow(L"IDM_VIEWER_WINDOW",  RootWinId(IDM_VIEWER_WINDOW ), m_viewerWindow,                 true,  true );

	configureStatusBar();
	m_statusBar.Arrange(*this, m_mainNNetWindow);

	m_monitorWindow .Move(PixelRect{ 200_PIXEL,   0_PIXEL, 300_PIXEL, 200_PIXEL }, true);
	m_miniNNetWindow.Move(PixelRect{   0_PIXEL,   0_PIXEL, 300_PIXEL, 300_PIXEL }, true);
	m_descWindow    .Move(PixelRect{   0_PIXEL,   0_PIXEL, 300_PIXEL, 300_PIXEL }, true);
	m_signalDesigner.Move(PixelRect{ 100_PIXEL, 100_PIXEL, 600_PIXEL, 500_PIXEL }, true);
	m_colLutWindow  .Move(PixelRect{ 200_PIXEL, 200_PIXEL, 500_PIXEL, 400_PIXEL }, true);

	m_monitorWindow    .Show(false);
	m_miniNNetWindow   .Show(true);
	m_statusBar        .Show(true);
	m_mainNNetWindow   .Show(true);
	m_crsrWindow       .Show(true);
	m_parameterDlg     .Show(true);
	m_performanceWindow.Show(true);
	m_descWindow       .Show(true);
	m_signalDesigner   .Show(true);
	m_colLutWindow     .Show(true);

	if (! WinManager::GetWindowConfiguration())
		::Show(m_hwndApp, true);

	m_dynamicModelObservable       .RegisterObserver(m_mainNNetWindow);
	m_dynamicModelObservable       .RegisterObserver(m_miniNNetWindow);
	m_dynamicModelObservable       .RegisterObserver(m_monitorWindow);
	m_dynamicModelObservable       .RegisterObserver(m_timeDisplay);
	m_dynamicModelObservable       .RegisterObserver(m_crsrWindow);
	m_staticModelObservable        .RegisterObserver(m_mainNNetWindow);
	m_staticModelObservable        .RegisterObserver(m_miniNNetWindow);
	m_staticModelObservable        .RegisterObserver(m_monitorWindow);
	m_staticModelObservable        .RegisterObserver(m_performanceWindow);
	m_staticModelObservable        .RegisterObserver(m_appTitle);
	m_staticModelObservable        .RegisterObserver(m_undoRedoMenu);
	m_staticModelObservable        .RegisterObserver(m_appMenu);
	m_staticModelObservable        .RegisterObserver(m_parameterDlg);
	m_lockModelObservable          .RegisterObserver(m_appMenu);
	m_lockModelObservable          .RegisterObserver(m_parameterDlg);
	m_lockModelObservable          .RegisterObserver(m_mainNNetWindow);
	m_lockModelObservable          .RegisterObserver(m_appTitle);
	m_lockModelObservable          .RegisterObserver(m_simulationControl);
	m_lockModelObservable          .RegisterObserver(m_signalDesigner);
	m_highlightSigObservable       .RegisterObserver(m_mainNNetWindow);
	m_highlightSigObservable       .RegisterObserver(m_monitorWindow);
	m_highlightSigObservable       .RegisterObserver(m_mainNNetWindow);
	m_cursorPosObservable          .RegisterObserver(m_crsrWindow);
	m_performanceObservable        .RegisterObserver(m_performanceWindow);
	m_performanceObservable        .RegisterObserver(m_slowMotionDisplay);
	m_runObservable                .RegisterObserver(m_simulationControl);
	NNetPreferences::m_bScanArea   .RegisterObserver(m_simulationControl);
	NNetPreferences::m_bScanArea   .RegisterObserver(m_miniNNetWindow);
	NNetPreferences::m_bScanArea   .RegisterObserver(m_mainNNetWindow);
	NNetPreferences::m_bModelFront .RegisterObserver(m_mainNNetWindow);
	NNetPreferences::m_bFilter     .RegisterObserver(m_mainNNetWindow);
	NNetPreferences::m_bInputCables.RegisterObserver(m_mainNNetWindow);
	NNetPreferences::m_bArrows     .RegisterObserver(m_mainNNetWindow);
	NNetPreferences::m_colorLutScan.RegisterObserver(m_mainNNetWindow);
	m_slowMotionRatio              .RegisterObserver(m_compute);
	m_slowMotionRatio              .RegisterObserver(m_slowMotionDisplay);
	m_nmwi.GetParams()             .RegisterObserver(m_parameterDlg);
	m_nmwi.GetParams()             .RegisterObserver(m_compute);
	Preferences::m_bSound          .RegisterObserver(m_appMenu);
	m_coordObservable              .RegisterObserver(m_mainNNetWindow);
	m_coordObservable              .RegisterObserver(m_miniNNetWindow);
	m_activeSigGenObservable       .RegisterObserver(m_mainNNetWindow);
	m_activeSigGenObservable       .RegisterObserver(m_signalDesigner);

	m_appMenu.Notify(true);
	m_undoRedoMenu.Notify(true);

	Show(true);

	if (!Preferences::ReadPreferences() || !Preferences::m_bAutoOpen.Get())
	{
		ResetModelCmd::Push();
		CreateInitialNobsCmd::Push();
		m_dynamicModelObservable.NotifyAll();
		m_staticModelObservable.NotifyAll();
	}
}

void NNetAppWindow::setModelInterface()
{
	NNetCommand       ::SetModelInterface(&m_nmwi);
	m_parameterDlg     .SetModelInterface(&m_nmwi);
	m_compute          .SetModelInterface(&m_nmwi);
	m_monitorWindow    .SetModelInterface(&m_nmwi);
	m_signalDesigner   .SetModelInterface(&m_nmwi);
	m_appTitle         .SetModelInterface(&m_nmwi);
	m_appMenu          .SetModelInterface(m_pNMRI);
	m_NNetController   .SetModelInterface(m_pNMRI);
	m_mainNNetWindow   .SetModelInterface(m_pNMRI);
	m_miniNNetWindow   .SetModelInterface(m_pNMRI);
	m_crsrWindow       .SetModelInterface(m_pNMRI);
	m_performanceWindow.SetModelInterface(m_pNMRI);
	NNetPreferences   ::SetModelInterface(m_pNMRI);
	Nob::SetParams(&m_pNMRI->GetParamsC());
}

bool NNetAppWindow::OnSize(PIXEL const width, PIXEL const height)
{
	m_statusBar.Arrange(*this, m_mainNNetWindow);
	return true;
}

bool NNetAppWindow::UserProc
(
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	switch (message)
	{

	case WM_ENTERMENULOOP:
		if (wParam == false)
			m_appMenu.Notify(true);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return BaseWindow::UserProc(message, wParam, lParam);
}

void NNetAppWindow::configureStatusBar()
{
	int iPart = 0;
	m_timeDisplay.Initialize(& m_statusBar, iPart);

	iPart = m_statusBar.NewPart();
	m_simulationControl.Initialize(&m_statusBar, &m_compute);

	iPart = m_statusBar.NewPart();
	m_slowMotionDisplay.Initialize(&m_statusBar, iPart, m_slowMotionRatio);

	SlowMotionControl::Add(m_statusBar);

	iPart = m_statusBar.NewPart();
	m_ScriptHook.Initialize(& m_statusBar, iPart);
	m_statusBar.ClearPart(iPart);
	m_statusBarDispFunctor.Initialize(& m_statusBar, iPart);
	m_statusMessagePart = iPart;

	m_statusBar.LastPart();
	m_timeDisplay.Notify(true);
	m_slowMotionDisplay.Notify(true);
}

void NNetAppWindow::OnClose()
{
	m_compute.StopComputation();
	if (m_NNetController.AskAndSave())
		WinManager::StoreWindowConfiguration();
	BaseWindow::OnClose();
}

void NNetAppWindow::OnNotify(WPARAM const wParam, LPARAM const lParam)
{
	NMHDR const * nmhdr { bit_cast<NMHDR const *>(lParam) };
	if (nmhdr->code == NM_DBLCLK)
		SendCommand(IDM_RESET_DYNAMIC_DATA, 0);
}

void NNetAppWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
	m_mainNNetWindow.OnChar(wParam, lParam);
}

bool NNetAppWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	try
	{
		switch (wmId)
		{
		case IDM_ABOUT:
			m_aboutBox.Show(m_mainNNetWindow.GetWindowHandle());
			return true;

		case IDM_DOCU:
			ShellExecute(0, 0, L"https://nnetsimu.miraheze.org/wiki/Main_Page ", 0, 0, SW_SHOW);
			return true;

		case IDM_EXIT:
			PostMessage(WM_CLOSE, 0, 0);
			return true;

		case IDM_DUMP:
			m_nmwi.DumpModel(__FILE__, __LINE__);
			SymbolTable::Dump(wcout);
			return true;

		case IDM_UNDO:
			if (!m_cmdStack.UndoStackCommand())
				m_sound.Warning();
			return true;

		case IDM_REDO:
			if (!m_cmdStack.RedoStackCommand())
				m_sound.Warning();
			return true;

		case IDM_RESET_DYNAMIC_DATA:
			m_nmwi.ClearDynamicData();
			m_compute.Reset();
			m_dynamicModelObservable.NotifyAll(true);
			return true;

		case IDM_SCAN:
			m_nmwi.SetTimestamp(SCANTIME);
			m_compute.StartScan();
			return true;

		case IDM_STARTING_SCAN:
			m_statusBar.DisplayInPart(m_statusMessagePart, L"Scanning " + to_wstring(lParam) + L" of " + to_wstring(m_nmwi.GetNrOfScans()));
			return true;

		case IDM_FINISHING_SCAN:
			m_statusBar.ClearPart(m_statusMessagePart);
			return true;

		case IDM_UNLOCK:
			m_compute.StopScan();
			m_nmwi.RejectImage();
			m_nmwi.ClearEventList();
			return true;

		case IDM_FORWARD:
			m_compute.SingleStep();
			return true;

		case IDM_RUN:
			m_compute.StartComputation();
			return true;

		case IDM_STOP:
			m_compute.StopComputation();
			return true;

		case IDM_SCRIPT_DIALOG:
			m_compute.StopComputation();
			processScript();
			return true;

		case IDM_TRIGGER_STIMULUS:
			m_compute.StartStimulus();
			return true;

		case IDM_NEXT_SCRIPT_CMD:
			if (ScriptStack::IsScriptActive())
			{
				Script * const pScript { ScriptStack::GetScript() };
				if (pScript && (!pScript->ProcessToken() || !pScript->ReadNextToken()))
					ScriptStack::CloseScript();
			}
			return true;

		case IDM_NEW_MODEL:
			if (AskNotUndoable() && m_NNetController.AskAndSave())
				newModel();
			return true;

		case IDM_IMPORT_MODEL:
			m_cmdStack.Clear();
			importModel();
			return true;

		case IDM_APP_DATA_CHANGED:
			m_appTitle.SetUnsavedChanges(true);
			return true;

		case IDD_TRIGGER_SIGNAL_DESIGNER:
			m_signalDesigner.Trigger(false);
			return true;

		case IDM_OPEN_MODEL:
			m_cmdStack.Clear();
			openModel();
			return true;

		case IDM_RELOAD_MODEL:
			if (AskNotUndoable())
			{
				m_cmdStack.Clear();
				NNetModelIO::Import(L"", NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL, GetWindowHandle()));
			}
			return true;

		case IDM_CENTER_MODEL:
			m_mainNNetWindow.CenterModel();
			return true;

		case IDX_EXPORT_FINISHED:
			m_statusBar.ClearPart(m_statusMessagePart);
			return true;

		case IDX_READ_PROGRESS_REPORT:  //no user command, only internal usage
			m_statusBar.ReadProgressReport(m_statusMessagePart, bit_cast<Script *>(lParam));
			return true;

		case IDX_WRITE_PROGRESS_REPORT:  //no user command, only internal usage
			m_statusBar.WriteProgressReport(m_statusMessagePart, *bit_cast<wstring const *>(lParam));
			return true;

		case IDX_ASK_REPLACE_MODEL:  //no user command, only internal usage
			if (m_NNetController.AskAndSave())
				replaceModel();
			return true;

		case IDX_REPLACE_MODEL:  //no user command, only internal usage
			replaceModel();
			return true;

		case IDX_FILE_NOT_FOUND:  //no user command, only internal usage
			MessageBox(nullptr, L"Could not find model file", L"Error", MB_OK);
			return true;

		case IDX_ERROR_IN_FILE:  //no user command, only internal usage
			MessageBox
			(
				nullptr, 
				L"Error reading model file\r\nSee main_trace.out for details", 
				L"Error", 
				MB_OK 
			);
			return true;

		default:
			if (m_NNetController.HandleCommand(wmId, lParam, NP_NULL))
				return true;
		}
	}
	catch (NobTypeException const & e)
	{
		wcout << COMMENT_START << L"Command failed: " << endl;
		wcout << COMMENT_START << L"File    : " << e.m_szFile  << endl;
		wcout << COMMENT_START << L"Line    : " << e.m_iLineNr << endl;
		wcout << COMMENT_START << L"Function: " << source_location::current().function_name()    << endl;
		wcout << COMMENT_START << L"NobType : " << to_wstring(static_cast<int>(e.m_type.GetValue())) << endl;
		FatalError::Happened(10, L"Invalid NobType");
	}

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

bool NNetAppWindow::AskNotUndoable()
{
	if (m_appTitle.AnyUnsavedChanges() && NNetPreferences::m_bAskNotUndoable.Get())
	{
		int iRes = MessageBox(nullptr, L"This command will not be undoable.\nCommand history will be lost.\n\nContinue?", L"Warning", MB_YESNO);
		return iRes == IDYES;
	}
	return true;
}

void NNetAppWindow::importModel()
{
	wstring const wstrFileName { NNetModelIO::GetModelFileName() };
	bool    const bRes         
	{ 
		NNetModelIO::Import
		(
			wstrFileName, 
			NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL, GetWindowHandle())
		) 
	};
	if (!bRes)
		SendCommand(IDM_NEW_MODEL, 0);
}

void NNetAppWindow::openModel()
{
	NNetModelIO::Import
	(
		AskModelFileName(tFileMode::read),
		NNetInputOutputUI::CreateNew(IDX_ASK_REPLACE_MODEL, GetWindowHandle())
	);
}

void NNetAppWindow::newModel()
{
	m_compute.StopComputation();
	m_mainNNetWindow.Reset();
	ResetModelCmd::Push();
	CreateInitialNobsCmd::Push();
	m_upModel->ClearDynamicData();
	m_staticModelObservable.NotifyAll();
	m_dynamicModelObservable.NotifyAll();
	m_mainNNetWindow.CenterModel();
}

void NNetAppWindow::replaceModel()
{
	m_compute.StopComputation();
	m_mainNNetWindow.Reset();
	m_upModel = NNetModelIO::GetImportedModel();	
	m_upModel->m_upSigGenList->SetActiveSigGenObservable(m_activeSigGenObservable);
	m_monitorWindow.SetHighSigObservable(m_highlightSigObservable);
	m_nmwi.SetModel(m_upModel.get());

	setModelInterface();

	m_signalDesigner.RegisterAtSigGen(m_nmwi.GetSigGenIdSelected());
	m_dynamicModelObservable.NotifyAll();
	m_staticModelObservable.NotifyAll();
	m_nmwi.SetDescriptionUI(m_descWindow);
	m_appTitle.SetUnsavedChanges(false);
	m_mainNNetWindow.CenterModel();
	m_nmwi.GetParams().RegisterObserver(m_parameterDlg);
	m_nmwi.GetParams().RegisterObserver(m_compute);
	m_nmwi.GetParams().NotifyAll();
	m_statusBar.ClearPart(m_statusMessagePart);
}

void NNetAppWindow::processScript() const
{
	wstring wstrFile { ScriptFile::AskForFileName(L"in", L"", L"Script files", tFileMode::read)};
	if (!wstrFile.empty())
		StartScript(wstrFile, m_ScriptHook);
	if (!ScriptStack::GetScript()->ReadNextToken())
		ScriptStack::CloseScript();
}
