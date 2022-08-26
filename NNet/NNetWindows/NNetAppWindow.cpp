// NNetAppWindow.cpp
//
// NNetWindows

module;

#include <bit>
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <chrono>
#include <filesystem>
#include <source_location>
#include "NNetCommand.h"
#include "NNetModelReaderInterface.h"
#include "NobException.h"
#include "win32_util_resource.h"
#include "Resource.h"
#include "SignalFactory.h"
#include "Neuron.h"
#include "InputConnector.h"
#include "SigGenId.h"

module NNetAppWindow;

import MoreTypes;
import Util;
import Trace;
import Direct2D;
import StatusBar;
import ObserverInterface;
import PerformanceWindow;
import Win32_AboutBox;
import UndoRedoMenu;
import Preferences;
import UtilityWrappers;
import NNetInputOutputUI;
import NNetWrappers;
import FatalError;
import TimeGraph;
import MessagePump;
import ScriptFile;
import BaseWindow;
import Win32_Util;
import Win32_PIXEL;
import NNetWindow;
import ScriptStack;
import AutoOpen;
import Analyzer;
import SimulationTime;
import Script;

using std::wcout;
using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::wostringstream;
using std::filesystem::path;
using std::source_location;

using namespace std::chrono;
using std::to_wstring;
using std::bit_cast;

static HCURSOR m_hCrsrWait  { nullptr };
static HCURSOR m_hCrsrArrow { nullptr };

NNetAppWindow::NNetAppWindow()
{
	m_hCrsrWait  = LoadCursor(nullptr, IDC_WAIT);
	m_hCrsrArrow = LoadCursor(nullptr, IDC_ARROW);
	DefineUtilityWrapperFunctions();
	SignalFactory  ::Initialize(m_dynamicModelObservable);
	Command        ::Initialize(&m_mainNNetWindow);
	m_modelIO       .Initialize();
	m_sound         .Initialize(&m_soundOnObservable);
	m_cmdStack      .Initialize(&m_staticModelObservable);
	m_modelCommands .Initialize(&m_modelIO, &m_dynamicModelObservable, &m_cmdStack);
	m_NNetController.Initialize
	(
		& m_WinManager,
		& m_modelCommands,
		& m_computeThread,
		& m_SlowMotionRatio,
		& m_sound,
		& m_preferences,
		& m_cmdStack,
		& m_monitorWindow
	);
	m_computeThread.Initialize
	(
		& m_SlowMotionRatio, 
		& m_runObservable, 
		& m_performanceObservable, 
		& m_dynamicModelObservable
	);
	InitializeNNetWrappers(&m_modelCommands, &m_modelIO);
};

NNetAppWindow::~NNetAppWindow() = default;

void NNetAppWindow::setModelInterface()
{
	NNetCommand       ::SetModelInterface(&m_nmwi);
	m_parameterDlg     .SetModelInterface(&m_nmwi);
	m_modelCommands    .SetModelInterface(&m_nmwi);
	m_computeThread    .SetModelInterface(&m_nmwi);
	m_monitorWindow    .SetModelInterface(&m_nmwi);
	m_signalDesigner   .SetModelInterface(&m_nmwi);
	m_appMenu          .SetModelInterface(m_pNMRI);
	m_appTitle         .SetModelInterface(m_pNMRI);
	m_cmdStack         .SetModelInterface(m_pNMRI);
	m_NNetController   .SetModelInterface(m_pNMRI);
	m_mainNNetWindow   .SetModelInterface(m_pNMRI);
	m_miniNNetWindow   .SetModelInterface(m_pNMRI);
	m_crsrWindow       .SetModelInterface(m_pNMRI);
	m_performanceWindow.SetModelInterface(m_pNMRI);
	m_preferences      .SetModelInterface(m_pNMRI);
	NNetWrappersSetModelInterface        (m_pNMRI);
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

	NNetInputOutputUI::Initialize(m_hwndApp);
	m_appTitle      .Initialize(m_hwndApp);
	m_preferences   .Initialize(m_descWindow, m_sound, m_modelIO, m_hwndApp);
	m_signalDesigner.Initialize
	(
		m_hwndApp, 
		m_computeThread, 
		m_runObservable, 
		m_dynamicModelObservable, 
		&m_modelCommands
	);

	m_upModel = make_unique<NNetModel>();
	m_nmwi.SetModel(m_upModel.get());
	m_pNMRI = static_cast<NNetModelReaderInterface *>(&m_nmwi);
	m_nmwi.SetDescriptionUI(m_descWindow);

//	m_nmwi.SetHighSigObservable(&m_highlightSigObservable);
	m_mainNNetWindow   .SetRefreshRate(  0ms);   // immediate refresh
	m_miniNNetWindow   .SetRefreshRate(200ms);
	m_monitorWindow    .SetRefreshRate(  0ms);
	m_crsrWindow       .SetRefreshRate(100ms);
	m_performanceWindow.SetRefreshRate(500ms);
	m_statusBar        .SetRefreshRate(300ms);

	m_appMenu          .Start(m_hwndApp, m_computeThread, m_WinManager, m_cmdStack, m_sound, m_preferences);
	m_statusBar        .Start(m_hwndApp);
	m_descWindow       .Start(m_hwndApp);
	m_crsrWindow       .Start(m_hwndApp, & m_mainNNetWindow);
	m_parameterDlg     .Start(m_hwndApp, & m_modelCommands);
	m_performanceWindow.Start(m_hwndApp, & m_computeThread, & m_SlowMotionRatio, & m_atDisplay);
	m_monitorWindow    .Start(m_hwndApp, m_sound, m_modelCommands);
	m_undoRedoMenu     .Start(& m_appMenu);

	setModelInterface();

	m_signalDesigner.RegisterAtSigGen(m_nmwi.GetSigGenIdSelected());
	pump.RegisterWindow(m_descWindow.GetWindowHandle(), true);

	m_mainNNetWindow.Start
	(
		m_hwndApp, 
		false,
		30._fPixel,
		m_preferences,
		m_NNetController,
		m_modelCommands,
		m_cursorPosObservable,
		m_coordObservable,
		& m_atDisplay
	);

	m_miniNNetWindow.Start(m_hwndApp, true,	5._fPixel, m_NNetController);

	m_miniNNetWindow.ObservedNNetWindow(& m_mainNNetWindow);  // mini window observes main grid window

	m_WinManager.AddWindow(L"IDM_APPL_WINDOW",    IDM_APPL_WINDOW,    m_hwndApp,                      true,  true );
	m_WinManager.AddWindow(L"IDM_STATUS_BAR",     IDM_STATUS_BAR,     m_statusBar.GetWindowHandle(),  false, false);
	m_WinManager.AddWindow(L"IDM_CRSR_WINDOW",    IDM_CRSR_WINDOW,    m_crsrWindow,                   true,  false);
	m_WinManager.AddWindow(L"IDM_DESC_WINDOW",    IDM_DESC_WINDOW,    m_descWindow.GetWindowHandle(), true,  true );
	m_WinManager.AddWindow(L"IDM_MAIN_WINDOW",    IDM_MAIN_WINDOW,    m_mainNNetWindow,               true,  false);
	m_WinManager.AddWindow(L"IDM_MINI_WINDOW",    IDM_MINI_WINDOW,    m_miniNNetWindow,               true,  true );
	m_WinManager.AddWindow(L"IDM_MONITOR_WINDOW", IDM_MONITOR_WINDOW, m_monitorWindow,                true,  true );
	m_WinManager.AddWindow(L"IDM_PARAM_WINDOW",   IDM_PARAM_WINDOW,   m_parameterDlg,                 true,  false);
	m_WinManager.AddWindow(L"IDM_PERF_WINDOW",    IDM_PERF_WINDOW,    m_performanceWindow,            true,  false);
	m_WinManager.AddWindow(L"IDM_SIG_DESIGNER",   IDM_SIG_DESIGNER,   m_signalDesigner,               true,  true );

	m_dynamicModelObservable.RegisterObserver(m_mainNNetWindow);
	m_dynamicModelObservable.RegisterObserver(m_miniNNetWindow);
	m_dynamicModelObservable.RegisterObserver(m_monitorWindow);
	m_dynamicModelObservable.RegisterObserver(m_timeDisplay);
	m_dynamicModelObservable.RegisterObserver(m_crsrWindow);
	m_staticModelObservable .RegisterObserver(m_mainNNetWindow);
	m_staticModelObservable .RegisterObserver(m_miniNNetWindow);
	m_staticModelObservable .RegisterObserver(m_monitorWindow);
	m_staticModelObservable .RegisterObserver(m_performanceWindow);
	m_staticModelObservable .RegisterObserver(m_appTitle);
	m_staticModelObservable .RegisterObserver(m_undoRedoMenu);
	m_staticModelObservable .RegisterObserver(m_appMenu);
	m_highlightSigObservable.RegisterObserver(m_mainNNetWindow);
	m_highlightSigObservable.RegisterObserver(m_monitorWindow);
	m_cursorPosObservable   .RegisterObserver(m_crsrWindow);
	m_performanceObservable .RegisterObserver(m_performanceWindow);
	m_runObservable         .RegisterObserver(m_simulationControl);
	m_SlowMotionRatio       .RegisterObserver(m_computeThread);
	m_SlowMotionRatio       .RegisterObserver(m_slowMotionDisplay);
	m_nmwi.GetParams()      .RegisterObserver(m_parameterDlg);
	m_nmwi.GetParams()      .RegisterObserver(m_computeThread);
	m_soundOnObservable     .RegisterObserver(m_appMenu);
	m_coordObservable       .RegisterObserver(m_miniNNetWindow);
	m_coordObservable       .RegisterObserver(m_mainNNetWindow);

	configureStatusBar();
	adjustChildWindows();

	m_monitorWindow .Move(PixelRect{ 200_PIXEL,   0_PIXEL, 300_PIXEL, 200_PIXEL }, true);
	m_miniNNetWindow.Move(PixelRect{   0_PIXEL,   0_PIXEL, 300_PIXEL, 300_PIXEL }, true);
	m_descWindow    .Move(PixelRect{   0_PIXEL,   0_PIXEL, 300_PIXEL, 300_PIXEL }, true);
	m_signalDesigner.Move(PixelRect{ 100_PIXEL, 100_PIXEL, 600_PIXEL, 500_PIXEL }, true);

	m_monitorWindow    .Show(false);
	m_miniNNetWindow   .Show(true);
	m_statusBar        .Show(true);
	m_mainNNetWindow   .Show(true);
	m_crsrWindow       .Show(true);
	m_parameterDlg     .Show(true);
	m_performanceWindow.Show(true);
	m_descWindow       .Show(true);
	m_signalDesigner   .Show(true);

	if (! m_WinManager.GetWindowConfiguration())
		Util::Show(m_hwndApp, true);

	m_appMenu.Notify(true);
	m_undoRedoMenu.Notify(true);

	Show(true);

//	::CreateWindowToolTip(m_statusBar.GetWindowHandle(), L"blah blah");

	if (! AutoOpen::IsOn() || ! m_preferences.ReadPreferences())
	{
		m_modelCommands.ResetModel();
		m_modelCommands.CreateInitialNobs();
	}

	m_bStarted = true;
}

void NNetAppWindow::Stop()
{
	m_bStarted = false;

	m_computeThread.LockComputation();

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
	m_SlowMotionRatio       .UnregisterAllObservers();
	m_nmwi.GetParams()      .UnregisterAllObservers();
	m_soundOnObservable     .UnregisterAllObservers();

	m_WinManager.RemoveAll();
	m_nmwi.SetModel(nullptr);
}

bool NNetAppWindow::OnSize(PIXEL const width, PIXEL const height)
{
	adjustChildWindows();
	return true;
}

void NNetAppWindow::OnPaint()
{
	static auto const CLR_GREY { RGB(128, 128, 128) };
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, CLR_GREY);
	(void)EndPaint(&ps);
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
	m_simulationControl.Initialize(& m_statusBar, & m_computeThread);

	iPart = m_statusBar.NewPart();
	m_slowMotionDisplay.Initialize(& m_statusBar, & m_SlowMotionRatio, iPart);

	SlowMotionControl::Add(m_statusBar);

	iPart = m_statusBar.NewPart();
	m_ScriptHook.Initialize(& m_statusBar, iPart);
	m_statusBar.ClearPart(iPart);

	m_statusBarDispFunctor.Initialize(& m_statusBar, iPart);
	ModelAnalyzer::SetStatusBarDisplay(& m_statusBarDispFunctor);
	ModelAnalyzer::SetEscFunc         (& Util::EscapeKeyPressed);
	m_statusMessagePart = iPart;

	m_statusBar.LastPart();
	m_timeDisplay.Notify(true);
	m_slowMotionDisplay.Notify(true);
}

void NNetAppWindow::adjustChildWindows()
{
	PixelRectSize pntAppClientSize(Util::GetClRectSize(m_hwndApp));
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
		m_computeThread.StopComputation();
		if (! AskAndSave())
			return;
		m_WinManager.StoreWindowConfiguration();
		Stop();
	}
	DestroyWindow();
}

void NNetAppWindow::OnNotify(WPARAM const wParam, LPARAM const lParam)
{
	NMHDR const * nmhdr { bit_cast<NMHDR const *>(lParam) };
	if (nmhdr->code == NM_DBLCLK)
		SendCommand(IDM_RESET_DYNAMIC_DATA, 0);
}

bool NNetAppWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);
	
	try
	{
		switch (wmId)
		{
		case IDM_ABOUT:
			ShowAboutBox(m_mainNNetWindow.GetWindowHandle());
			break;

		case IDM_EXIT:
			PostMessage(WM_CLOSE, 0, 0);
			break;

		case IDM_DUMP:
			m_nmwi.DUMP();
			break;

		case IDM_RESET_DYNAMIC_DATA:
			m_nmwi.ClearDynamicData();
			m_computeThread.Reset();
			m_dynamicModelObservable.NotifyAll(true);
			break;

		case IDM_FORWARD:
			m_computeThread.SingleStep();
			break;

		case IDM_RUN_STOP:
			m_computeThread.RunStopComputation();
			break;

		case IDM_SCRIPT_DIALOG:
			m_computeThread.StopComputation();
			processScript();
			break;

		case IDM_NEXT_SCRIPT_CMD:
			if (ScriptStack::IsScriptActive())
			{
				Script * const pScript { ScriptStack::GetScript() };
				if (pScript && (!pScript->ProcessToken() || !pScript->ReadNextToken()) )
					ScriptStack::CloseScript();
			}
			return false;

		case IDM_NEW_MODEL:
			if (AskAndSave())
				newModel();
			break;

		case IDM_SAVE_MODEL:
			if (SaveModel())
				m_preferences.WritePreferences();
			break;

		case IDD_TRIGGER_SIGNAL_DESIGNER:
			m_signalDesigner.Trigger();
			break;

		case IDD_REGISTER_SIG_GEN:
			m_signalDesigner.RegisterAtSigGen(SigGenId(Cast2Int(lParam)));
			break;

		case IDM_SAVE_MODEL_AS:
			m_computeThread.StopComputation();
			if (SaveModelAs())
				m_preferences.WritePreferences();
			break;

		case IDM_OPEN_MODEL:
			m_cmdStack.Clear();
			m_modelIO.Import
			(
				AskModelFile(tFileMode::read), 
				NNetInputOutputUI::CreateNew(IDX_REPLACE_MODEL)
			);
			break;

		case IDM_ADD_MODEL:
			m_modelIO.Import
			(
				AskModelFile(tFileMode::read), 
				NNetInputOutputUI::CreateNew(IDM_ADD_IMPORTED_MODEL)
			);
			break;

		case IDM_CENTER_MODEL:
			m_mainNNetWindow.CenterModel();
			break;

		case IDD_SENSOR_PNTS:
			m_mainNNetWindow.SetSensorPoints();
			break;

		case IDM_ANALYZE_LOOPS:
			m_modelCommands.AnalyzeLoops();
			m_mainNNetWindow.CenterSelection();
			break;

		case IDM_ANALYZE_ANOMALIES:
			m_modelCommands.AnalyzeAnomalies();
			m_mainNNetWindow.CenterSelection();
			break;

		case IDD_ARROWS:
			m_mainNNetWindow.AnimateArrows();
			break;

		case IDX_READ_PROGRESS_REPORT:  //no user command, only internal usage
			m_statusBar.ReadProgressReport(m_statusMessagePart, bit_cast<Script *>(lParam));
			break;

		case IDX_WRITE_PROGRESS_REPORT:  //no user command, only internal usage
			m_statusBar.WriteProgressReport(m_statusMessagePart, *bit_cast<wstring const *>(lParam));
			break;

		case IDX_REPLACE_MODEL:  //no user command, only internal usage
			if (AskAndSave())
				replaceModel();
			m_statusBar.ClearPart(m_statusMessagePart);
			break;

		case IDX_FILE_NOT_FOUND:  //no user command, only internal usage
			MessageBox(nullptr, L"Could not find model file", L"Error", MB_OK);
			break;

		case IDX_ERROR_IN_FILE:  //no user command, only internal usage
			MessageBox
			(
				nullptr, 
				L"Error reading model file\r\nSee main_trace.out for details", 
				L"Error", 
				MB_OK 
			);
			break;

		default:
			if (m_NNetController.HandleCommand(wmId, lParam, NP_NULL))
				return true;
		}
	}
	catch (NobTypeException const & e)
	{
		wcout << Scanner::COMMENT_START << L"Command failed: " << endl;
		wcout << Scanner::COMMENT_START << L"File    : " << e.m_szFile  << endl;
		wcout << Scanner::COMMENT_START << L"Line    : " << e.m_iLineNr << endl;
		wcout << Scanner::COMMENT_START << L"Function: " << source_location::current().function_name()    << endl;
		wcout << Scanner::COMMENT_START << L"NobType : " << to_wstring(static_cast<int>(e.m_type.GetValue())) << endl;
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
		wstrModelPath = AskModelFile(tFileMode::write);
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

void NNetAppWindow::newModel()
{
	m_computeThread.StopComputation();
	m_mainNNetWindow.Reset();
	m_modelCommands.ResetModel();
	m_modelCommands.CreateInitialNobs();
	m_staticModelObservable.NotifyAll(false);
	m_appTitle.SetUnsavedChanges(true);
	m_mainNNetWindow.CenterModel();
}

void NNetAppWindow::replaceModel()
{
	m_computeThread.StopComputation();
	m_mainNNetWindow.Reset();
	m_upModel = m_modelIO.GetImportedModel();
	m_nmwi.SetModel(m_upModel.get());

	setModelInterface();

	m_signalDesigner.RegisterAtSigGen(m_nmwi.GetSigGenIdSelected());
	m_dynamicModelObservable.NotifyAll(false);
	m_staticModelObservable.NotifyAll(false);
	m_nmwi.SetDescriptionUI(m_descWindow);
	m_appTitle.SetUnsavedChanges(false);
	m_mainNNetWindow.CenterModel();
	m_nmwi.GetParams().RegisterObserver(m_parameterDlg);
	m_nmwi.GetParams().RegisterObserver(m_computeThread);
	m_nmwi.GetParams().NotifyAll(false);
}

void NNetAppWindow::StartScript(wstring const & wstrFile) const
{
	wcout << Scanner::COMMENT_START + L"Processing script file " << wstrFile << endl;
	Script * pScript { ScriptStack::OpenScript() };
	if (pScript && pScript->ScrOpen(wstrFile))
	{
		pScript->ScrSetNewLineHook(& m_ScriptHook);
		Command::NextScriptCommand();  // start reading script file
	}
}

void NNetAppWindow::processScript() const
{
	wstring wstrFile { ScriptFile::AskForFileName(L"in", L"Script files", tFileMode::read) };
	if (!wstrFile.empty())
		StartScript(wstrFile);
	if (!ScriptStack::GetScript()->ReadNextToken())
		ScriptStack::CloseScript();
}

void NNetAppWindow::WriteModel()
{
	SetCursor(m_hCrsrWait);
	m_modelIO.Export(*m_pNMRI, NNetInputOutputUI::CreateNew(0));
	m_appTitle.SetUnsavedChanges(false);
	m_statusBar.ClearPart(m_statusMessagePart);
	SetCursor(m_hCrsrArrow);
}
