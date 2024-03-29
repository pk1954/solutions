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

module NNetWin32:NNetAppWindow;

import Win32_Util_Resource;
import Types;
import Util;
import Trace;
import Direct2D;
import Vector2D;
import ObserverInterface;
import Command;
import AboutBox;
import IoUtil;
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
import :PerformanceWindow;
import :UndoRedoMenu;
import :NNetInputOutputUI;
import :NNetWindow;
import :NNetSimuRunning;

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

static HCURSOR m_hCrsrWait  { nullptr };
static HCURSOR m_hCrsrArrow { nullptr };

NNetAppWindow::NNetAppWindow(wstring const & wstrProductName)
{
	m_aboutBox.SetProductName(wstrProductName);
	m_hCrsrWait  = LoadCursor(nullptr, IDC_WAIT);
	m_hCrsrArrow = LoadCursor(nullptr, IDC_ARROW);
	DefineUtilityWrapperFunctions();

	WinManager       ::Initialize();
	Model            ::Initialize(&m_lockModelObservable);
	Signal           ::Initialize(&m_dynamicModelObservable);
	Command          ::Initialize(&m_cmdStack, &m_sound);
	CoordAnimationCmd::Initialize(&m_coordObservable);
	BaseCommand      ::Initialize(&m_sound);
	NNetModelIO      ::Initialize(&m_lockModelObservable);

	m_scanMatrix    .Initialize();
	m_simuRunning   .Initialize(&m_compute);
	m_cmdStack      .Initialize(&m_staticModelObservable);
	m_NNetController.Initialize(&m_compute, &m_slowMotionRatio);

	MonitorScrollState* pMonitorScrollState { NNetModelIO::AddModelWrapper<MonitorScrollState>(L"MonitorScrollState") };
	pMonitorScrollState->SetMonitorWindow(&m_monitorWindow);
	Nob::SetColorLut(NNetPreferences::m_colorLutVoltage);
};

NNetAppWindow::~NNetAppWindow() = default;

void NNetAppWindow::setModelInterface()
{
	NNetCommand       ::SetModelInterface(&m_nmwi);
	m_parameterDlg     .SetModelInterface(&m_nmwi);
	m_cmdStack         .SetModelInterface(&m_nmwi);
	m_compute          .SetModelInterface(&m_nmwi);
	m_monitorWindow    .SetModelInterface(&m_nmwi);
	m_signalDesigner   .SetModelInterface(&m_nmwi);
	m_appTitle         .SetModelInterface(m_pNMRI);
	m_NNetController   .SetModelInterface(m_pNMRI);
	m_scanMatrix       .SetModelInterface(m_pNMRI);
	m_mainNNetWindow   .SetModelInterface(m_pNMRI);
	m_miniNNetWindow   .SetModelInterface(m_pNMRI);
	m_crsrWindow       .SetModelInterface(m_pNMRI);
	m_performanceWindow.SetModelInterface(m_pNMRI);
	NNetPreferences   ::SetModelInterface(m_pNMRI);
	Nob::SetParams(&m_pNMRI->GetParamsC());
}

void NNetAppWindow::Start(MessagePump & pump)
{
	m_hwndApp = StartBaseWindow
	(
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	m_appTitle.Initialize(m_hwndApp);

	NNetPreferences::Initialize();

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
		&m_dynamicModelObservable,
		&m_scanMatrix
	);

	m_upModel = m_nmwi.CreateNewModel();
	m_pNMRI   = static_cast<NNetModelReaderInterface *>(&m_nmwi);
	m_nmwi.SetDescriptionUI(m_descWindow);
	m_upModel->SetActiveSigGenObservable(m_activeSigGenObservable);
	m_upModel->SetHighSigObservable     (m_highlightSigObservable);
	m_mainNNetWindow   .SetRefreshRate(300ms);
	m_miniNNetWindow   .SetRefreshRate(500ms);
	m_monitorWindow    .SetRefreshRate( 20ms);
	m_crsrWindow       .SetRefreshRate(300ms);
	m_performanceWindow.SetRefreshRate(500ms);
	m_statusBar        .SetRefreshRate(300ms);

	m_appMenu          .Start(m_hwndApp, m_compute, m_cmdStack, m_sound);
	m_statusBar        .Start(m_hwndApp);
	m_descWindow       .Start(m_hwndApp);
	m_crsrWindow       .Start(m_hwndApp, &m_mainNNetWindow, &m_scanMatrix);
	m_parameterDlg     .Start(m_hwndApp);
	m_performanceWindow.Start(m_hwndApp, &m_compute, &m_slowMotionRatio, &m_atDisplay);
	m_monitorWindow    .Start(m_hwndApp, m_simuRunning, m_sound, m_staticModelObservable);
	m_colLutWindow     .Start(m_hwndApp, &NNetPreferences::m_colorLutScan);
	m_undoRedoMenu     .Start(& m_appMenu);

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
		& m_atDisplay,
		& m_monitorWindow,
		& m_scanMatrix
	);

	m_miniNNetWindow.Start(m_hwndApp, true,	m_NNetController, &m_scanMatrix);

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

	configureStatusBar();
	adjustChildWindows();

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
	m_staticModelObservable        .RegisterObserver(m_scanMatrix);
	m_staticModelObservable        .RegisterObserver(m_mainNNetWindow);
	m_staticModelObservable        .RegisterObserver(m_miniNNetWindow);
	m_staticModelObservable        .RegisterObserver(m_monitorWindow);
	m_staticModelObservable        .RegisterObserver(m_performanceWindow);
	m_staticModelObservable        .RegisterObserver(m_appTitle);
	m_staticModelObservable        .RegisterObserver(m_undoRedoMenu);
	m_staticModelObservable        .RegisterObserver(m_appMenu);
	m_staticModelObservable        .RegisterObserver(m_parameterDlg);
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

	m_bStarted = true;
}

void NNetAppWindow::Stop()
{
	BaseWindow::Stop();
	m_bStarted = false;

	m_compute.StopComputation();

	m_mainNNetWindow   .Stop();
	m_miniNNetWindow   .Stop();
	m_monitorWindow    .Stop();
	m_crsrWindow       .Stop();
	m_performanceWindow.Stop();
	m_parameterDlg     .Stop();
	m_statusBar        .Stop();

	m_staticModelObservable .UnregisterAllObservers();
	m_dynamicModelObservable.UnregisterAllObservers();
	m_cursorPosObservable   .UnregisterAllObservers();
	m_performanceObservable .UnregisterAllObservers();
	m_runObservable         .UnregisterAllObservers();
	m_slowMotionRatio       .UnregisterAllObservers();
	m_nmwi.GetParams()      .UnregisterAllObservers();

	WinManager::RemoveAll();
	m_nmwi.SetModel(nullptr);
}

bool NNetAppWindow::OnSize(PIXEL const width, PIXEL const height)
{
	adjustChildWindows();
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

void NNetAppWindow::adjustChildWindows()
{
	PixelRectSize pntAppClientSize(::GetClRectSize(m_hwndApp));
	if (pntAppClientSize.IsNotZero())
	{
		PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
		PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();
		m_statusBar.Resize();
		pixAppClientWinHeight -= m_statusBar.GetHeight();

		m_mainNNetWindow.Move  // use all available space for model window
		(
			0_PIXEL, 
			0_PIXEL, 
			pixAppClientWinWidth, 
			pixAppClientWinHeight, 
			true 
		);
	}
}

void NNetAppWindow::OnClose()
{
	if (m_bStarted)
	{
		m_compute.StopComputation();
		if (! AskAndSave())
			return;
		WinManager::StoreWindowConfiguration();
		Stop();
	}
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

wstring NNetAppWindow::askModelFile(enum class tFileMode const mode) const
{
	return ScriptFile::AskForFileName(L"mod", L"Model files", mode);
}

bool NNetAppWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	try
	{
		switch (wmId)
		{
		case 0:             // unclear origin
			return true;

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

		case IDM_RESET_DYNAMIC_DATA:
			m_nmwi.ClearDynamicData();
			m_compute.Reset();
			m_dynamicModelObservable.NotifyAll(true);
			return true;

		case IDM_SCAN:
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
			m_nmwi.RejectScanImage();
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
			m_monitorWindow.StimulusTriggered();
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
			if (AskNotUndoable() && AskAndSave())
				newModel();
			return true;

		case IDM_SAVE_MODEL:
			if (SaveModel())
				Preferences::WritePreferences();
			return true;

		case IDM_IMPORT_MODEL:
			m_cmdStack.Clear();
			if (!NNetModelIO::Import(NNetModelIO::GetModelFileName(), NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL)))
				SendCommand(IDM_NEW_MODEL, 0);
			return true;

		case IDM_APP_DATA_CHANGED:
			m_appTitle.SetUnsavedChanges(true);
			return true;

		case IDD_TRIGGER_SIGNAL_DESIGNER:
			m_signalDesigner.Trigger(false);
			return true;

		case IDM_SAVE_MODEL_AS:
			m_compute.StopComputation();
			if (SaveModelAs())
				Preferences::WritePreferences();
			return true;

		case IDM_OPEN_MODEL:
			m_cmdStack.Clear();
			NNetModelIO::Import
			(
				askModelFile(tFileMode::read),
				NNetInputOutputUI::CreateNew(IDX_ASK_REPLACE_MODEL)
			);
			return true;

		case IDM_RELOAD_MODEL:
			if (AskNotUndoable())
			{
				m_cmdStack.Clear();
				NNetModelIO::Import(L"", NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL));
			}
			return true;

		case IDM_ADD_MODULE:
			NNetModelIO::Import
			(
				askModelFile(tFileMode::read), 
				NNetInputOutputUI::CreateNew(IDM_ADD_IMPORTED_MODEL)
			);
			return true;

		case IDM_CENTER_MODEL:
			m_mainNNetWindow.CenterModel();
			return true;

		case IDX_READ_PROGRESS_REPORT:  //no user command, only internal usage
			m_statusBar.ReadProgressReport(m_statusMessagePart, bit_cast<Script *>(lParam));
			return true;

		case IDX_WRITE_PROGRESS_REPORT:  //no user command, only internal usage
			m_statusBar.WriteProgressReport(m_statusMessagePart, *bit_cast<wstring const *>(lParam));
			return true;

		case IDX_ASK_REPLACE_MODEL:  //no user command, only internal usage
			if (AskAndSave())
				replaceModel();
			m_statusBar.ClearPart(m_statusMessagePart);
			return true;

		case IDX_REPLACE_MODEL:  //no user command, only internal usage
			replaceModel();
			m_statusBar.ClearPart(m_statusMessagePart);
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

bool NNetAppWindow::SaveModelAs()
{
	wstring wstrModelPath { m_pNMRI->GetModelFilePath() };
	if (wstrModelPath == L"")
	{
		wstrModelPath = path(ScriptFile::GetPathOfExecutable()).parent_path();
		wstrModelPath += L"\\std.mod";
		m_nmwi.SetModelFilePath(wstrModelPath);
		WriteModel();
		return true;
	}
	else
	{
		wstrModelPath = askModelFile(tFileMode::write);
		bool const bRes = wstrModelPath != L"";
		if (bRes)
		{
			m_nmwi.SetModelFilePath(wstrModelPath);
			WriteModel();
		}
		return bRes;
	}
}

bool NNetAppWindow::SaveModel()
{
	wstring wstrModelPath { m_pNMRI->GetModelFilePath() };
	if (wstrModelPath == L"")
	{
		return SaveModelAs();
	}
	else
	{
		WriteModel();
		return true;
	}
}

bool NNetAppWindow::AskAndSave()
{
	if (m_appTitle.AnyUnsavedChanges())
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

bool NNetAppWindow::AskNotUndoable()
{
	if (m_appTitle.AnyUnsavedChanges())
	{
		int iRes = MessageBox(nullptr, L"This command will not be undoable.\nCommand history will be lost.\n\nContinue?", L"Warning", MB_YESNO);
		return iRes == IDYES;
	}
	return true;
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
	m_appTitle.SetUnsavedChanges(true);
	m_mainNNetWindow.CenterModel();
}

void NNetAppWindow::replaceModel()
{
	m_compute.StopComputation();
	m_mainNNetWindow.Reset();
	m_upModel = NNetModelIO::GetImportedModel();	
	m_upModel->SetActiveSigGenObservable(m_activeSigGenObservable);
	m_upModel->SetHighSigObservable     (m_highlightSigObservable);
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
}

void NNetAppWindow::processScript() const
{
	wstring wstrFile { ScriptFile::AskForFileName(L"in", L"Script files", tFileMode::read) };
	if (!wstrFile.empty())
		StartScript(wstrFile, m_ScriptHook);
	if (!ScriptStack::GetScript()->ReadNextToken())
		ScriptStack::CloseScript();
}

void NNetAppWindow::WriteModel()
{
	SetCursor(m_hCrsrWait);
	NNetModelIO::Export(*m_pNMRI, NNetInputOutputUI::CreateNew(0));
	Preferences::WritePreferences();
	m_appTitle.SetUnsavedChanges(false);
	m_statusBar.ClearPart(m_statusMessagePart);
	SetCursor(m_hCrsrArrow);
}
