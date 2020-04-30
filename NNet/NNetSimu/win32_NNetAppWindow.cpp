// win32_NNetAppWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windows.h"
#include <chrono>

// Model interfaces

#include "MoreTypes.h"
#include "NNetModel.h"
#include "NNetModelStorage.h"

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

	Preferences::Initialize();
	BaseAppWindow::Initialize( & m_NNetWorkThreadInterface );

	NNetWorkThread::InitClass( (tAppCallBack)( [&]( int const id ) { PostMessage( WM_COMMAND, id, 0 ); } ) );

	m_pModelRedrawProxy  = new ObserverProxy( );
	m_pModelChangedProxy = new ObserverProxy( );
	m_pPerformanceWindow = new PerformanceWindow( );

	NNetWindow::InitClass( & m_NNetWorkThreadInterface, & m_atDisplay );

	m_pNNetColors     = new NNetColors( m_pModelRedrawProxy );
	m_pCursorPos      = new Observable( );
	m_pAppMenu        = new NNetAppMenu( );
	m_pMainNNetWindow = new NNetWindow( );
	m_pMainNNetWindow2 = new NNetWindow( );
	m_pCrsrWindow     = new CrsrWindow( );
	m_pParameterDlg   = new ParameterDialog( & m_NNetWorkThreadInterface );

	DefineNNetWrappers( & m_NNetWorkThreadInterface, m_pMainNNetWindow );

	m_pMainNNetWindow   ->SetRefreshRate( 100ms );
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
	delete m_pModelRedrawProxy;
	delete m_pModelChangedProxy;
}

void NNetAppWindow::Start( )
{
	m_pParameters       = new Param( );
	m_pModel            = new NNetModel( m_pParameters, m_pModelChangedProxy );
	m_pNNetModelStorage = new NNetModelStorage( m_pModel, m_pParameters );
	m_pModelChangedProxy->RegisterObserver( m_pNNetModelStorage );

	BaseAppWindow::Start( m_pMainNNetWindow );
	m_pAppMenu->Initialize
	( 
		m_hwndApp, 
		& m_NNetWorkThreadInterface,
		& m_WinManager 
	);

	m_pNNetController = new NNetController
	( 
		m_pNNetModelStorage,
		m_pMainNNetWindow,
		& m_WinManager,
		& m_NNetWorkThreadInterface,
		& m_SlowMotionRatio
	);

	m_pMainNNetWindow->Start
	( 
		m_hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		nullptr,  // no visibility criterion. Allways visible,
		m_pModel,
		m_pCursorPos
	);

	m_pMainNNetWindow2->Start
	( 
		m_hwndApp, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_VISIBLE, 
		nullptr,  // no visibility criterion. Allways visible,
		m_pModel,
		m_pCursorPos
	);

	m_pModelRedrawProxy->RegisterObserver( m_pMainNNetWindow );
	//m_pModelRedrawProxy->RegisterObserver( m_pMainNNetWindow2 );

	m_NNetWorkThreadInterface.Start
	( 
		m_hwndApp, 
		& m_atComputation,
		& m_eventPOI, 
		  m_pModelRedrawProxy,
		  m_pNNetModelStorage,
		& m_SlowMotionRatio,
		m_pModel,
		m_pParameters,
		m_pNNetModelStorage,
		TRUE    // async thread?
	);

	m_pCrsrWindow->Start( m_hwndApp, m_pMainNNetWindow, m_pModel );
	m_pCursorPos       ->RegisterObserver( m_pCrsrWindow );
	m_pModelRedrawProxy->RegisterObserver( m_pCrsrWindow );
	m_pParameterDlg->Start( m_hwndApp, m_pParameters );
	m_pPerformanceWindow->Start( m_hwndApp, m_pModel, & m_NNetWorkThreadInterface, & m_atDisplay );

	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW",  IDM_CRSR_WINDOW,  * m_pCrsrWindow,        TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW",  IDM_MAIN_WINDOW,  * m_pMainNNetWindow,    TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW_2",  IDM_MAIN_WINDOW_2,  * m_pMainNNetWindow2,    TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_PARAM_WINDOW", IDM_PARAM_WINDOW, * m_pParameterDlg,      TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW",  IDM_PERF_WINDOW,  * m_pPerformanceWindow, TRUE, FALSE );

	configureStatusBar( );

	m_pMainNNetWindow2->Move( PixelRect{ 0_PIXEL, 0_PIXEL, 200_PIXEL, 200_PIXEL }, true );

	m_pNNetController->SetDisplayFunctor( m_pStatusBarDisplayFunctor );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
		m_pMainNNetWindow->Show( TRUE );
		m_pMainNNetWindow2->Show( TRUE );
	}

	m_pCrsrWindow       ->Show( TRUE );
	m_pParameterDlg     ->Show( TRUE );
	m_pPerformanceWindow->Show( TRUE );

	PostCommand2Application( IDM_RUN, true );

	if ( ! AutoOpen::IsOn( ) || ! Preferences::ReadPreferences( m_pNNetModelStorage ) )
		m_pModel->CreateInitialShapes();

//	m_pNNetModelStorage->Write( wcout );

	m_bStarted = TRUE;
}

void NNetAppWindow::Stop()
{
	m_bStarted = FALSE;

	m_pMainNNetWindow->Stop( );
	m_pMainNNetWindow2->Stop( );
	m_pCrsrWindow    ->Stop( );
	m_pParameterDlg  ->Stop( );
	m_pPerformanceWindow->Stop( );

	m_pModelRedrawProxy->UnregisterAllObservers( );
	m_NNetWorkThreadInterface.Stop( );

	BaseAppWindow::Stop();

	delete m_pModel;
	delete m_pTimeDisplay;
	delete m_pSlowMotionDisplay;
	delete m_pNNetController;

	m_WinManager.RemoveAll( );
}

void NNetAppWindow::configureStatusBar( )
{
	int iPartScriptLine = 0;

	m_pTimeDisplay = new TimeDisplay( & m_StatusBar, m_pModel, iPartScriptLine );
	m_pModelRedrawProxy->RegisterObserver( m_pTimeDisplay );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_pSimulationControl = new SimulationControl( & m_StatusBar, & m_NNetWorkThreadInterface );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_pSlowMotionDisplay = new SlowMotionDisplay( & m_StatusBar, & m_SlowMotionRatio, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart( );
	SlowMotionControl::Add( & m_StatusBar );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_ScriptHook.Initialize( & m_StatusBar, iPartScriptLine );
	m_StatusBar.DisplayInPart( iPartScriptLine, L"" );
	Script::ScrSetWrapHook( & m_ScriptHook );
	m_pStatusBarDisplayFunctor = new StatusBarDisplayFunctor( & m_StatusBar, iPartScriptLine );
	ModelAnalyzer::SetDisplayFunctor( m_pStatusBarDisplayFunctor );

	m_StatusBar.LastPart( );
	m_pTimeDisplay->Notify( true );
	m_pSlowMotionDisplay->Notify( true );
}

void NNetAppWindow::ProcessCloseMessage( )
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

bool NNetAppWindow::ProcessAppCommand( WPARAM const wParam, LPARAM const lParam )
{
	int const wmId = LOWORD( wParam );

	if ( wmId == IDM_FATAL_ERROR )
	{
		Stop();
		FatalError::Happened( static_cast<long>(lParam), "unknown" );
	}

	try
	{
		bool bRes = m_pNNetController->ProcessUIcommand( wmId, lParam ); // handle all commands that affect the UI
		if ( bRes )                                                      // but do not concern the model  
			return true;                                                    
	}
	catch ( ... )
	{
		Stop();
		FatalError::Happened( 2, "ProcessUIcommand" );
	}

	try
	{
		if ( m_pNNetController->ProcessModelCommand( wmId, lParam ) )
			return ProcessFrameworkCommand( wmId, lParam );             // Some commands are handled by the framework controller
	}
	catch ( ... )
	{
		Stop();
		FatalError::Happened( 3, "ProcessModelCommand" );
	}

	return false;
}
