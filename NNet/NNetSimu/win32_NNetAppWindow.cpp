// win32_NNetAppWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windows.h"
#include <chrono>

// Model interfaces

#include "MoreTypes.h"
#include "NNetModelInterface.h"
#include "NNetModelStorage.h"
#include "DrawModel.h"

// interfaces of various windows

#include "win32_NNetWindow.h"
#include "win32_status.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_parameterDlg.h"
#include "win32_simulationControl.h"
#include "win32_slowMotionControl.h"
#include "win32_zoomControl.h"
#include "SlowMotionDisplay.h"
#include "TimeDisplay.h"

// infrastructure

#include "util.h"
#include "DisplayFunctor.h"
#include "ObserverInterface.h"

// scripting and tracing

#include "trace.h"
#include "script.h"
#include "Preferences.h"
#include "NNetWrappers.h"
#include "ComputeThread.h"
#include "UtilityWrappers.h"
#include "win32_stopwatch.h"
#include "win32_fatalError.h"

// system and resources

#include "Resource.h"

// application

#include "Analyzer.h"
#include "AutoOpen.h"
#include "win32_NNetAppWindow.h"

using namespace std::literals::chrono_literals;
using std::wostringstream;
using std::wstring;
using std::wcout;

NNetAppWindow::NNetAppWindow( )
{
	Stopwatch stopwatch;

	Preferences   ::Initialize( );
	BaseAppWindow ::Initialize( );
	NNetWorkThread::InitClass( (tAppCallBack)( [&]( int const id ) { PostMessage( WM_COMMAND, id, 0 ); } ) );
	NNetWindow    ::InitClass( & m_NNetWorkThreadInterface, & m_atDisplay );

	m_traceStream = OpenTraceFile( L"main_trace.out" );

	m_NNetWorkThreadInterface.Initialize( & m_traceStream );

	m_pPerformanceWindow = new PerformanceWindow( );
	m_pNNetColors        = new NNetColors( & m_blinkObservable );
	m_pAppMenu           = new NNetAppMenu( );
	m_pMainNNetWindow    = new MainWindow( );
	m_pMiniNNetWindow    = new MiniWindow( );
	m_pCrsrWindow        = new CrsrWindow( );
	m_pParameterDlg      = new ParameterDialog( & m_NNetWorkThreadInterface );

	DefineNNetWrappers( & m_NNetWorkThreadInterface, m_pMainNNetWindow );

	m_pMainNNetWindow   ->SetRefreshRate( 100ms );
	m_pMiniNNetWindow   ->SetRefreshRate( 200ms );
	m_pCrsrWindow       ->SetRefreshRate( 100ms );
	m_pPerformanceWindow->SetRefreshRate( 500ms );
};

NNetAppWindow::~NNetAppWindow( )
{
	delete m_pNNetColors;
	delete m_pMainNNetWindow;
	delete m_pAppMenu;
	delete m_pCrsrWindow;
	delete m_pParameterDlg;
	delete m_pPerformanceWindow;
	delete m_pStatusBarDisplayFunctor;
}

void NNetAppWindow::Start( )
{
	m_pParameters = new Param( );
	m_pModel      = new NNetModel
	( 
		m_pParameters, 
		& m_staticModelObservable, 
		& m_dynamicModelObservable, 
		& m_modelTimeObservable 
	);
	m_pModelInterface   = new NNetModelInterface( m_pModel );
	m_pDrawModel        = new DrawModel         ( m_pModel );
	m_pNNetModelStorage = new NNetModelStorage  ( m_pModel, m_pParameters );
	m_pComputeThread    = new ComputeThread     ( m_pModel, m_pParameters, & m_SlowMotionRatio );

	m_staticModelObservable.RegisterObserver( m_pNNetModelStorage );

	m_StatusBar.Start( m_hwndApp, & m_NNetWorkThreadInterface );
	BaseAppWindow::Start( m_pMainNNetWindow, m_pComputeThread );
	m_pAppMenu->Initialize
	( 
		m_hwndApp,
		m_pComputeThread,
		& m_NNetWorkThreadInterface,
		& m_WinManager 
	);

	m_pNNetController = new NNetController
	( 
		m_pNNetModelStorage,
		m_pMainNNetWindow,
		& m_WinManager,
		& m_NNetWorkThreadInterface,
		m_pComputeThread,
		& m_SlowMotionRatio
	);

	m_pMainNNetWindow->Start
	( 
		m_hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		m_pComputeThread,
		m_pNNetController,
		m_pModelInterface,
		m_pDrawModel,
		& m_cursorPosObservable
	);

	m_blinkObservable       .RegisterObserver( m_pMainNNetWindow );
	m_staticModelObservable .RegisterObserver( m_pMainNNetWindow );
	m_dynamicModelObservable.RegisterObserver( m_pMainNNetWindow );
	m_pMainNNetWindow->ShowRefreshRateDlg( false );
	m_pMainNNetWindow->AddObserver( m_pMiniNNetWindow );

	m_pMiniNNetWindow->Start
	( 
		m_hwndApp, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		m_pComputeThread,
		m_pNNetController,
		m_pModelInterface,
		m_pDrawModel,
		& m_cursorPosObservable
	);

	SetWindowText( m_pMiniNNetWindow->GetWindowHandle(), L"Mini window" );

	m_pMiniNNetWindow->ObservedNNetWindow( m_pMainNNetWindow );  // mini window observes main grid window

	m_pMiniNNetWindow->Move( PixelRect{ 0_PIXEL, 0_PIXEL, 300_PIXEL, 300_PIXEL }, true );

	m_staticModelObservable.RegisterObserver( m_pMiniNNetWindow );

	m_NNetWorkThreadInterface.Start
	( 
		m_hwndApp, 
		& m_atComputation,
		& m_eventPOI, 
		& m_SlowMotionRatio,
		m_pModel,
		m_pParameters,
		m_pNNetModelStorage,
		false // true    // async thread?
	);

	m_pCrsrWindow->Start( m_hwndApp, m_pMainNNetWindow, m_pModelInterface );
	m_cursorPosObservable   .RegisterObserver( m_pCrsrWindow );
	m_dynamicModelObservable.RegisterObserver( m_pMainNNetWindow );
	m_pParameterDlg->Start( m_hwndApp, m_pParameters );
	m_pPerformanceWindow->Start( m_hwndApp, m_pModelInterface, m_pComputeThread, & m_SlowMotionRatio, & m_atDisplay );

	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW",  IDM_CRSR_WINDOW,  * m_pCrsrWindow,        true, false );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW",  IDM_MAIN_WINDOW,  * m_pMainNNetWindow,    true, false );
	m_WinManager.AddWindow( L"IDM_MINI_WINDOW",  IDM_MINI_WINDOW,  * m_pMiniNNetWindow,    true, false );
	m_WinManager.AddWindow( L"IDM_PARAM_WINDOW", IDM_PARAM_WINDOW, * m_pParameterDlg,      true, false );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW",  IDM_PERF_WINDOW,  * m_pPerformanceWindow, true, false );

	configureStatusBar( );

	m_pNNetController->SetStatusBarDisplay( m_pStatusBarDisplayFunctor );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		wcout << L"Using default window positions" << std::endl;
		Show( true );
		m_pMainNNetWindow->Show( true );
		m_pMiniNNetWindow->Show( true );
	}
	m_pCrsrWindow       ->Show( true );
	m_pParameterDlg     ->Show( true );
	m_pPerformanceWindow->Show( true );

	if ( ! AutoOpen::IsOn( ) || ! Preferences::ReadPreferences( m_pNNetModelStorage ) )
		m_NNetWorkThreadInterface.PostResetModel( );

//	m_pNNetModelStorage->Write( wcout );

	PostCommand2Application( IDM_RUN, true );

	m_bStarted = true;
}

void NNetAppWindow::Stop()
{
	m_bStarted = false;

	m_pMainNNetWindow   ->Stop( );
	m_pMiniNNetWindow   ->Stop( );
	m_pCrsrWindow       ->Stop( );
	m_pParameterDlg     ->Stop( );
	m_pPerformanceWindow->Stop( );

	m_staticModelObservable.UnregisterAllObservers( );
	m_NNetWorkThreadInterface.Stop( );

	BaseAppWindow::Stop();

	delete m_pModelInterface;
	delete m_pDrawModel;
	delete m_pModel;
	delete m_pTimeDisplay;
	delete m_pSlowMotionDisplay;
	delete m_pNNetController;

	m_WinManager.RemoveAll( );
}

void NNetAppWindow::configureStatusBar( )
{
	int iPartScriptLine = 0;

	m_pTimeDisplay = new TimeDisplay( & m_StatusBar, m_pModelInterface, iPartScriptLine );
	m_modelTimeObservable.RegisterObserver( m_pTimeDisplay );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_pSimulationControl = new SimulationControl( & m_StatusBar, m_pComputeThread );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_pSlowMotionDisplay = new SlowMotionDisplay( & m_StatusBar, & m_SlowMotionRatio, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart( );
	SlowMotionControl::Add( & m_StatusBar );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_ScriptHook.Initialize( & m_StatusBar, iPartScriptLine );
	m_StatusBar.DisplayInPart( iPartScriptLine, L"" );
	Script::ScrSetWrapHook( & m_ScriptHook );
	m_pStatusBarDisplayFunctor = new StatusBarDisplayFunctor( & m_StatusBar, iPartScriptLine );
	ModelAnalyzer::SetStatusBarDisplay( m_pStatusBarDisplayFunctor );

	m_StatusBar.LastPart( );
	m_pTimeDisplay->Notify( true );
	m_pSlowMotionDisplay->Notify( true );
}

void NNetAppWindow::OnClose( )
{
	if ( m_bStarted )
	{
		if ( ! m_pNNetModelStorage->AskAndSave( ) )
			return;
		m_WinManager.StoreWindowConfiguration( );
		Stop( );
	}
	DestroyWindow( );        
}

bool NNetAppWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	if ( m_pNNetController->HandleCommand( wParam, lParam, NP_NULL ) )
		return true;
	else 
		return BaseAppWindow::OnCommand( wParam, lParam, pixPoint );
}
