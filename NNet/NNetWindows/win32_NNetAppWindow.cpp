// win32_NNetAppWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windows.h"
#include <chrono>
#include <filesystem>

// Model interfaces

#include "MoreTypes.h"
#include "NNetModelReaderInterface.h"

// interfaces of various windows

#include "win32_NNetWindow.h"
#include "win32_status.h"
#include "win32_zoomControl.h"

// infrastructure

#include "util.h"
#include "ObserverInterface.h"
#include "ConnAnimationCommand.h"
#include "win32_messagePump.h"
#include "win32_importTermination.h"
#include "NNetError.h"

// scripting and tracing

#include "trace.h"
#include "script.h"
#include "Preferences.h"
#include "NNetWrappers.h"
#include "NNetWinWrappers.h"
#include "UtilityWrappers.h"
#include "win32_script.h"
#include "win32_stopwatch.h"
#include "win32_fatalError.h"

// system and resources

#include "Resource.h"

// application

#include "win32_baseWindow.h"
#include "win32_aboutBox.h"
#include "Analyzer.h"
#include "AutoOpen.h"
#include "Neuron.h"
#include "UndoRedoMenu.h"
#include "win32_NNetAppWindow.h"

using namespace std::literals::chrono_literals;

using std::unique_ptr;
using std::make_unique;
using std::wostringstream;
using std::wstring;
using std::wcout;
using std::filesystem::path;

NNetAppWindow::NNetAppWindow()
{
	Stopwatch stopwatch;

	SwitchWcoutTo( L"main_trace.out" );

	wcout << L"*** Application start at " << Util::GetCurrentDateAndTime() << endl;
	wcout << L"*** Computer name: "       << Util::GetComputerName()       << endl;
	wcout << L"*** User name:     "       << Util::GetUserName()           << endl;
	wcout << endl;

	Neuron::SetSound( & m_sound );
	NNetWindow::InitClass( & m_atDisplay );

	DefineUtilityWrapperFunctions();
	DefineNNetWrappers( & m_modelCommands );
	DefineNNetWinWrappers( & m_mainNNetWindow, & m_winCommands );
};

NNetAppWindow::~NNetAppWindow() { }

void NNetAppWindow::Start( MessagePump & pump )
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

	SignalFactory::Initialize( m_nmri, m_dynamicModelObservable );
	Shape::Initialize( m_model.GetParams() );
	m_model.SetDescriptionUI( m_descWindow );

	m_modelImporter .Initialize( &m_script );
	m_modelExporter .Initialize( &m_nmri );
	m_modelCommands .Initialize( &m_nmri, &m_nmwi, &m_modelImporter, &m_dynamicModelObservable, &m_cmdStack );
	m_winCommands   .Initialize( &m_cmdStack, &m_modelCommands, &m_nmwi );
	m_cmdStack      .Initialize( &m_nmwi, & m_staticModelObservable );
	m_sound         .Initialize( &m_soundOnObservable );
	m_appTitle      .Initialize( m_hwndApp, &m_nmri );
	m_preferences   .Initialize( m_sound, m_modelImporter, m_hwndApp );
	m_NNetController.Initialize
	( 
		& m_modelExporter,
		& m_mainNNetWindow,
		& m_WinManager,
		& m_nmri,
		& m_modelCommands,
		& m_winCommands,
		& m_computeThread,
		& m_SlowMotionRatio,
		& m_statusBarDispFunctor,
		& m_sound,
		& m_preferences,
		& m_cmdStack,
		& m_monitorWindow
	);

	m_mainNNetWindow   .SetRefreshRate(   0ms );   // immediate refresh
	m_miniNNetWindow   .SetRefreshRate( 200ms );
	m_monitorWindow    .SetRefreshRate(   0ms );
	m_crsrWindow       .SetRefreshRate( 100ms );
	m_performanceWindow.SetRefreshRate( 500ms );
	m_StatusBar        .SetRefreshRate( 300ms );

	m_nmri        .Start( & m_model );
	m_nmwi        .Start( & m_model );
	m_appMenu     .Start( m_hwndApp, m_computeThread, m_WinManager, m_cmdStack, m_sound, m_mainNNetWindow );
	m_StatusBar   .Start( m_hwndApp );
	m_descWindow  .Start( m_hwndApp );
	m_undoRedoMenu.Start( & m_appMenu );
	pump.RegisterWindow( m_descWindow.GetWindowHandle(), true );

	m_computeThread.Start
	( 
		& m_model, 
		& m_SlowMotionRatio, 
		& m_runObservable, 
		& m_performanceObservable, 
		& m_dynamicModelObservable 
	);

	m_mainNNetWindow.Start
	( 
		m_hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		false,
		30._fPixel,
		m_nmri,
		m_monitorWindow,
		m_NNetController,
		m_modelCommands,
		m_winCommands,
		m_cursorPosObservable,
		m_coordObservable
	);

	m_miniNNetWindow.Start
	( 
		m_hwndApp, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		true,
		5._fPixel,
		m_nmri,
		m_monitorWindow,
		m_NNetController
	);

	m_miniNNetWindow.ObservedNNetWindow( & m_mainNNetWindow );  // mini window observes main grid window

	SetWindowText( m_miniNNetWindow.GetWindowHandle(), L"Mini window" );

	m_crsrWindow       .Start( m_hwndApp, & m_mainNNetWindow, & m_nmri );
	m_parameterDlg     .Start( m_hwndApp, & m_modelCommands, & m_model.GetParams() );
	m_performanceWindow.Start( m_hwndApp, & m_nmri, & m_computeThread, & m_SlowMotionRatio, & m_atDisplay );
	m_monitorWindow    .Start( m_hwndApp, & m_sound, & m_NNetController, & m_modelCommands, m_nmri, m_model.GetMonitorData() );

	m_WinManager.AddWindow( L"IDM_APPL_WINDOW",    IDM_APPL_WINDOW,    m_hwndApp,                      true,  true  );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",     IDM_STATUS_BAR,     m_StatusBar.GetWindowHandle(),  false, false );
	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW",    IDM_CRSR_WINDOW,    m_crsrWindow,                   true,  false );
	m_WinManager.AddWindow( L"IDM_DESC_WINDOW",    IDM_DESC_WINDOW,    m_descWindow.GetWindowHandle(), true,  true  );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW",    IDM_MAIN_WINDOW,    m_mainNNetWindow,               true,  false );
	m_WinManager.AddWindow( L"IDM_MINI_WINDOW",    IDM_MINI_WINDOW,    m_miniNNetWindow,               true,  true  );
	m_WinManager.AddWindow( L"IDM_MONITOR_WINDOW", IDM_MONITOR_WINDOW, m_monitorWindow,                true,  true  );
	m_WinManager.AddWindow( L"IDM_PARAM_WINDOW",   IDM_PARAM_WINDOW,   m_parameterDlg,                 true,  false );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW",    IDM_PERF_WINDOW,    m_performanceWindow,            true,  false );

	m_dynamicModelObservable.RegisterObserver( & m_mainNNetWindow );
	m_dynamicModelObservable.RegisterObserver( & m_miniNNetWindow );
	m_dynamicModelObservable.RegisterObserver( & m_monitorWindow );
	m_dynamicModelObservable.RegisterObserver( & m_timeDisplay );
	m_staticModelObservable .RegisterObserver( & m_mainNNetWindow );
	m_staticModelObservable .RegisterObserver( & m_miniNNetWindow );
	m_staticModelObservable .RegisterObserver( & m_monitorWindow );
	m_staticModelObservable .RegisterObserver( & m_performanceWindow );
	m_staticModelObservable .RegisterObserver( & m_appTitle );
	m_staticModelObservable .RegisterObserver( & m_undoRedoMenu );
	m_cursorPosObservable   .RegisterObserver( & m_crsrWindow );
	m_performanceObservable .RegisterObserver( & m_performanceWindow );
	m_runObservable         .RegisterObserver( & m_simulationControl );
	m_SlowMotionRatio       .RegisterObserver( & m_computeThread );
	m_SlowMotionRatio       .RegisterObserver( & m_slowMotionDisplay );
	m_model.GetParams()     .RegisterObserver( & m_parameterDlg );
	m_model.GetParams()     .RegisterObserver( & m_computeThread );
	m_soundOnObservable     .RegisterObserver( & m_appMenu );
	m_coordObservable       .RegisterObserver( & m_miniNNetWindow );
	m_coordObservable       .RegisterObserver( & m_mainNNetWindow );

	configureStatusBar();
	adjustChildWindows();

	m_monitorWindow .Move( PixelRect{ 200_PIXEL, 0_PIXEL, 300_PIXEL, 200_PIXEL }, true );
	m_miniNNetWindow.Move( PixelRect{   0_PIXEL, 0_PIXEL, 300_PIXEL, 300_PIXEL }, true );
	m_descWindow    .Move( PixelRect{   0_PIXEL, 0_PIXEL, 300_PIXEL, 300_PIXEL }, true );

	m_monitorWindow    .Show( false );
	m_miniNNetWindow   .Show( true );
	m_StatusBar        .Show( true );
	m_mainNNetWindow   .Show( true );
	m_crsrWindow       .Show( true );
	m_parameterDlg     .Show( true );
	m_performanceWindow.Show( true );
	m_descWindow       .Show( true );

	if ( ! m_WinManager.GetWindowConfiguration() )
		Util::Show( m_hwndApp, true );

	m_appMenu.Notify( true );
	m_undoRedoMenu.Notify( true );

	Show( true );

	if ( ! AutoOpen::IsOn() || ! m_preferences.ReadPreferences() )
	{
		m_modelCommands.ResetModel();
		m_modelCommands.CreateInitialShapes();
	}

	m_bStarted = true;
}

void NNetAppWindow::Stop()
{
	m_bStarted = false;

	m_computeThread.LockComputation();

	m_timeDisplay      .Stop();
	m_mainNNetWindow   .Stop();
	m_miniNNetWindow   .Stop();
	m_monitorWindow    .Stop();
	m_crsrWindow       .Stop();
	m_performanceWindow.Stop();
	m_parameterDlg     .Stop();
	m_StatusBar        .Stop();
	m_nmri             .Stop();
	m_nmwi             .Stop();

	m_staticModelObservable .UnregisterAllObservers();
	m_dynamicModelObservable.UnregisterAllObservers();
	m_cursorPosObservable   .UnregisterAllObservers();
	m_performanceObservable .UnregisterAllObservers();
	m_runObservable         .UnregisterAllObservers();
	m_SlowMotionRatio       .UnregisterAllObservers();
	m_model.GetParams()     .UnregisterAllObservers();
	m_soundOnObservable     .UnregisterAllObservers();

	m_WinManager.RemoveAll();
}

bool NNetAppWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	adjustChildWindows();
	return true;
}

void NNetAppWindow::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC = BeginPaint( &ps );
	static COLORREF const CLR_GREY = RGB( 128, 128, 128 );
	FillBackground( hDC, CLR_GREY );
	(void)EndPaint( &ps );
}

bool NNetAppWindow::UserProc
( 
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	switch ( message )
	{

	case WM_ENTERMENULOOP:
		if ( wParam == false )
			m_appMenu.Notify( true );
		break;

	case WM_MOVE:
		adjustChildWindows();
		break;

	case WM_CHAR:
		m_mainNNetWindow.OnChar( wParam, lParam );
		return true;  

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		break;
	}

	return BaseWindow::UserProc( message, wParam, lParam );
}

void NNetAppWindow::configureStatusBar()
{
	int iPartScriptLine = 0;
	m_timeDisplay.Start( & m_StatusBar, & m_nmri, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart();
	m_simulationControl.Initialize( & m_StatusBar, & m_computeThread );

	iPartScriptLine = m_StatusBar.NewPart();
	m_slowMotionDisplay.Initialize( & m_StatusBar, & m_SlowMotionRatio, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart();
	SlowMotionControl::Add( & m_StatusBar );

	iPartScriptLine = m_StatusBar.NewPart();
	m_ScriptHook.Initialize( & m_StatusBar, iPartScriptLine, & m_script );
	m_StatusBar.ClearPart( iPartScriptLine );
	Script::ScrSetWrapHook( & m_ScriptHook );
	m_statusBarDispFunctor.Initialize( & m_StatusBar, iPartScriptLine );
	ModelAnalyzer::SetStatusBarDisplay( & m_statusBarDispFunctor );
	ModelAnalyzer::SetEscFunc         ( & Util::EscapeKeyPressed );
	m_statusMessagePart = iPartScriptLine;

	m_StatusBar.LastPart();
	m_timeDisplay.Notify( true );
	m_slowMotionDisplay.Notify( true );
}

void NNetAppWindow::adjustChildWindows()
{
	PixelRectSize pntAppClientSize( Util::GetClRectSize( m_hwndApp ) );
	if ( pntAppClientSize.IsNotZero() )
	{
		PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
		PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();
		m_StatusBar.Resize();
		pixAppClientWinHeight -= m_StatusBar.GetHeight();

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
	if ( m_bStarted )
	{
		m_computeThread.StopComputation();
		if ( ! AskAndSave() )
			return;
		m_WinManager.StoreWindowConfiguration();
		Stop();
	}
	DestroyWindow();
}

bool NNetAppWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	int const wmId = LOWORD( wParam );
	
	switch (wmId)
	{
	case IDM_EXIT:
		PostMessage( WM_CLOSE, 0, 0 );
		break;

	case IDM_FORWARD:
		m_computeThread.SingleStep();
		break;

	case IDM_RUN:
		m_computeThread.RunComputation();
		break;

	case IDM_STOP:
		m_computeThread.StopComputation();
		m_nmwi.ClearModel();
		break;

	case IDM_SCRIPT_DIALOG:
		m_computeThread.StopComputation();
		ProcessNNetScript
		( 
		    m_script, 
			m_nmwi.GetUPShapes(), 
			ScriptFile::AskForFileName( L"in", L"Script files", tFileMode::read )
		);
		break;

	case IDM_NEW_MODEL:
		if ( AskAndSave() )
		{
			m_computeThread.StopComputation();
			m_mainNNetWindow.Reset();
			m_modelCommands.ResetModel();
			m_modelCommands.CreateInitialShapes();
			m_staticModelObservable.NotifyAll( false );
			m_appTitle.SetUnsavedChanges( true );
			m_mainNNetWindow.CenterModel();
		}
		break;

	case IDM_SAVE_MODEL:
		if ( SaveModel() )
			m_preferences.WritePreferences( m_nmri.GetModelFilePath() );
		break;

	case IDM_SAVE_MODEL_AS:
		m_computeThread.StopComputation();
		if ( SaveModelAs() )
			m_preferences.WritePreferences( m_nmri.GetModelFilePath() );
		break;

	case IDM_OPEN_MODEL:
		m_cmdStack.Clear();
		m_modelImporter.Import
		( 
			AskModelFile(), 
			make_unique<NNetImportTermination>( m_hwndApp, IDX_REPLACE_MODEL )
		);
		break;

	case IDM_ADD_MODEL:
		m_modelImporter.Import
		( 
			AskModelFile(), 
			make_unique<NNetImportTermination>( m_hwndApp, IDM_ADD_IMPORTED_MODEL )
		); 
		break;

	case IDX_REPLACE_MODEL:  //no user command, only internal usage
		m_StatusBar.ClearPart( m_statusMessagePart );
		if ( AskAndSave() )
		{
			m_computeThread.StopComputation();
			m_mainNNetWindow.Reset();
			m_model = move(* m_modelImporter.GetImportedModel());
			m_staticModelObservable.NotifyAll( false );
			m_model.SetDescriptionUI( m_descWindow );
			m_appTitle.SetUnsavedChanges( false );
			m_mainNNetWindow.CenterModel();
		}
		break;

	default:
		if ( m_NNetController.HandleCommand( wmId, lParam, NP_NULL ) )
			return true;
	}

	return BaseWindow::OnCommand( wParam, lParam, pixPoint );
}

bool NNetAppWindow::SaveModelAs()
{
	wstring wstrModelPath { m_nmri.GetModelFilePath() };
	if ( wstrModelPath == L"" )
	{
		wstrModelPath = path( ScriptFile::GetPathOfExecutable() ).parent_path();
		wstrModelPath += L"\\std.mod";
		m_nmwi.SetModelFilePath( wstrModelPath );
		writeModel();
		return true;
	}
	else
	{
		wstrModelPath = ScriptFile::AskForFileName( L"mod", L"Model files", tFileMode::write );
		bool const bRes = wstrModelPath != L"";
		if ( bRes )
		{
			m_nmwi.SetModelFilePath( wstrModelPath );
			writeModel();
		}
		return bRes;
	}
}

bool NNetAppWindow::SaveModel()
{
	wstring wstrModelPath { m_nmri.GetModelFilePath() };
	if ( wstrModelPath == L"" )
	{
		return SaveModelAs();
	}
	else
	{
		writeModel();
		return true;
	}
}

bool NNetAppWindow::AskAndSave()
{
	if ( m_appTitle.AnyUnsavedChanges() )
	{
		int iRes = MessageBox( nullptr, L"Save now?", L"Unsaved changes", MB_YESNOCANCEL );
		if ( iRes == IDYES )
			SaveModel();
		else if ( iRes == IDNO )
			return true;
		else if ( iRes == IDCANCEL )
			return false;
	}
	return true;
}
