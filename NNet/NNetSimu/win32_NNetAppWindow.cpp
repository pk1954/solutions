// win32_NNetAppWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Windows.h"
#include <chrono>

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

#include "win32_baseWindow.h"
#include "win32_aboutBox.h"
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

	m_hwndConsole = GetConsoleWindow( );
	BringWindowToTop( m_hwndConsole );

	m_traceStream = OpenTraceFile( L"main_trace.out" );

	Preferences::Initialize( );
	NNetWindow ::InitClass( & m_atDisplay );

	DefineUtilityWrapperFunctions( );
	DefineNNetWrappers( & m_modelWriterInterface, & m_mainNNetWindow );
};

NNetAppWindow::~NNetAppWindow( )
{
	m_traceStream.close();
}

void NNetAppWindow::Start( )
{
	m_hwndApp = StartBaseWindow
	( 
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	m_model               .Initialize( & m_parameters, & m_staticModelObservable, & m_dynamicModelObservable, & m_modelTimeObservable );
	m_modelStorage        .Initialize( m_hwndApp, & m_model, & m_parameters, & m_script );
	m_modelWriterInterface.Initialize( & m_traceStream );
	m_drawModel           .Initialize( & m_model );
	m_NNetColors          .Initialize( & m_blinkObservable );
	m_NNetController      .Initialize
	( 
		& m_modelStorage,
		& m_mainNNetWindow,
		& m_WinManager,
		& m_modelReaderInterface,
		& m_modelWriterInterface,
		& m_computeThread,
		& m_SlowMotionRatio,
		& m_statusBarDispFunctor
	);

	m_mainNNetWindow   .SetRefreshRate(   0ms );   // immediate refresh
	m_miniNNetWindow   .SetRefreshRate( 200ms );
	m_crsrWindow       .SetRefreshRate( 100ms );
	m_performanceWindow.SetRefreshRate( 500ms );
	m_StatusBar        .SetRefreshRate( 300ms );

	m_computeThread.Start( & m_model, & m_parameters, & m_SlowMotionRatio, & m_runObservable, & m_performanceObservable	);
	m_appMenu      .Start( m_hwndApp, & m_computeThread, & m_WinManager );
	m_StatusBar    .Start( m_hwndApp );

	m_mainNNetWindow.Start
	( 
		m_hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		false,
		& m_computeThread,
		& m_NNetController,
		& m_modelReaderInterface,
		& m_modelWriterInterface,
		& m_drawModel,
		& m_cursorPosObservable
	);

	m_miniNNetWindow.Start
	( 
		m_hwndApp, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		true,
		& m_computeThread,
		& m_NNetController,
		& m_modelReaderInterface,
		& m_modelWriterInterface,
		& m_drawModel,
		& m_cursorPosObservable
	);

	m_mainNNetWindow.AddObserver( & m_miniNNetWindow );
	m_miniNNetWindow.ObservedNNetWindow( & m_mainNNetWindow );  // mini window observes main grid window

	SetWindowText( m_miniNNetWindow.GetWindowHandle(), L"Mini window" );

	m_modelReaderInterface.Start( & m_model );
	m_modelWriterInterface.Start( & m_model );
	m_crsrWindow          .Start( m_hwndApp, & m_mainNNetWindow, & m_modelReaderInterface );
	m_parameterDlg        .Start( m_hwndApp, & m_modelWriterInterface, & m_parameters );
	m_performanceWindow   .Start( m_hwndApp, & m_modelReaderInterface, & m_computeThread, & m_SlowMotionRatio, & m_atDisplay );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW",  IDM_CONS_WINDOW,  m_hwndConsole,                 true,  true  );
	m_WinManager.AddWindow( L"IDM_APPL_WINDOW",  IDM_APPL_WINDOW,  m_hwndApp,                     true,  true  );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",   IDM_STATUS_BAR,   m_StatusBar.GetWindowHandle(), false, false );
	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW",  IDM_CRSR_WINDOW,  m_crsrWindow,                  true,  false );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW",  IDM_MAIN_WINDOW,  m_mainNNetWindow,              true,  false );
	m_WinManager.AddWindow( L"IDM_MINI_WINDOW",  IDM_MINI_WINDOW,  m_miniNNetWindow,              true,  true  );
	m_WinManager.AddWindow( L"IDM_PARAM_WINDOW", IDM_PARAM_WINDOW, m_parameterDlg,                true,  false );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW",  IDM_PERF_WINDOW,  m_performanceWindow,           true,  false );

	m_staticModelObservable .RegisterObserver( & m_modelStorage );
	m_blinkObservable       .RegisterObserver( & m_mainNNetWindow );
	m_dynamicModelObservable.RegisterObserver( & m_mainNNetWindow );
	m_staticModelObservable .RegisterObserver( & m_mainNNetWindow );
	m_staticModelObservable .RegisterObserver( & m_miniNNetWindow );
	m_cursorPosObservable   .RegisterObserver( & m_crsrWindow );
	m_performanceObservable .RegisterObserver( & m_performanceWindow );
	m_modelTimeObservable   .RegisterObserver( & m_timeDisplay );
	m_runObservable         .RegisterObserver( & m_simulationControl );
	m_SlowMotionRatio       .RegisterObserver( & m_slowMotionDisplay );
	m_SlowMotionRatio       .RegisterObserver( & m_computeThread );
	m_parameters            .RegisterObserver( & m_computeThread );
	m_parameters            .RegisterObserver( & m_parameterDlg );

	configureStatusBar( );
	adjustChildWindows( );
	m_miniNNetWindow.Move( PixelRect{ 0_PIXEL, 0_PIXEL, 300_PIXEL, 300_PIXEL }, true );

	m_miniNNetWindow   .Show( true );
	m_StatusBar        .Show( true );
	m_mainNNetWindow   .Show( true );
	m_crsrWindow       .Show( true );
	m_parameterDlg     .Show( true );
	m_performanceWindow.Show( true );

	m_WinManager.GetWindowConfiguration( );

	Show( true );

	if ( ! AutoOpen::IsOn( ) || ! Preferences::ReadPreferences( & m_modelStorage ) )
		m_modelWriterInterface.ResetModel( );

	m_computeThread.RunComputation();

	m_bStarted = true;
}

void NNetAppWindow::Stop()
{
	m_bStarted = false;

	m_computeThread.LockComputation();

	m_timeDisplay         .Stop( );
	m_mainNNetWindow      .Stop( );
	m_miniNNetWindow      .Stop( );
	m_crsrWindow          .Stop( );
	m_performanceWindow   .Stop( );
	m_parameterDlg        .Stop( );
	m_StatusBar           .Stop( );
	m_appMenu             .Stop( );
	m_modelReaderInterface.Stop( );
	m_modelWriterInterface.Stop( );

	m_staticModelObservable .UnregisterAllObservers( );
	m_blinkObservable       .UnregisterAllObservers( );
	m_dynamicModelObservable.UnregisterAllObservers( );
	m_cursorPosObservable   .UnregisterAllObservers( );
	m_performanceObservable .UnregisterAllObservers( );
	m_modelTimeObservable   .UnregisterAllObservers( );
	m_runObservable         .UnregisterAllObservers( );
	m_SlowMotionRatio       .UnregisterAllObservers( );
	m_parameters            .UnregisterAllObservers( );

	m_WinManager.RemoveAll( );
}

LRESULT NNetAppWindow::UserProc
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
			m_appMenu.AdjustVisibility( );
		break;

	case WM_PAINT:
	{
		static COLORREF const CLR_GREY = RGB( 128, 128, 128 );
		PAINTSTRUCT   ps;
		HDC           hDC = BeginPaint( &ps );
		FillBackground( hDC, CLR_GREY );
		(void)EndPaint( &ps );
		return false;
	}

	case WM_SIZE:
	case WM_MOVE:
		adjustChildWindows( );
		break;

	case WM_CLOSE:
		OnClose( );
		return true;  

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		break;
	}

	return DefWindowProc( message, wParam, lParam );
}

void NNetAppWindow::configureStatusBar( )
{
	int iPartScriptLine = 0;
	m_timeDisplay.Start( & m_StatusBar, & m_modelReaderInterface, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_simulationControl.Initialize( & m_StatusBar, & m_computeThread );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_slowMotionDisplay.Initialize( & m_StatusBar, & m_SlowMotionRatio, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart( );
	SlowMotionControl::Add( & m_StatusBar );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_ScriptHook.Initialize( & m_StatusBar, iPartScriptLine, m_hwndApp );
	m_StatusBar.DisplayInPart( iPartScriptLine, L"" );
	Script::ScrSetWrapHook( & m_ScriptHook );
	m_statusBarDispFunctor.Initialize( & m_StatusBar, iPartScriptLine );
	ModelAnalyzer::SetStatusBarDisplay( & m_statusBarDispFunctor );
	m_statusMessagePart = iPartScriptLine;

	m_StatusBar.LastPart( );
	m_timeDisplay.Notify( true );
	m_slowMotionDisplay.Notify( true );
}

void NNetAppWindow::adjustChildWindows( )
{
	PixelRectSize pntAppClientSize( Util::GetClRectSize( m_hwndApp ) );
	if ( pntAppClientSize.IsNotZero( ) )
	{
		PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
		PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();
		m_StatusBar.Resize( );
		pixAppClientWinHeight -= m_StatusBar.GetHeight( );

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

void NNetAppWindow::OnClose( )
{
	if ( m_bStarted )
	{
		if ( ! m_modelStorage.AskAndSave( ) )
			return;
		m_WinManager.StoreWindowConfiguration( );
		Stop( );
	}
	DestroyWindow( );        
}

bool NNetAppWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	int const wmId = LOWORD( wParam );
	
	switch (wmId)
	{
	//case IDM_ABOUT:
	//	ShowAboutBox( GetWindowHandle( ) );
	//	break;

	case IDM_EXIT:
		PostMessage( WM_CLOSE, 0, 0 );
		break;

	case IDM_FORWARD:
		m_computeThread.SingleStep( );
		break;

	case IDM_RUN:
		m_computeThread.RunComputation( );
		break;

	case IDM_STOP:
		m_computeThread.StopComputation( );
		break;

	case IDM_SCRIPT_PROGRESS:
		m_ScriptHook.DisplayScriptProgress( m_script );
		break;

	case IDM_NEW_MODEL:
		m_computeThread.LockComputation( );  // will be restarted when centering complete
		if ( m_modelStorage.AskAndSave( ) )
		{
			m_modelWriterInterface.ResetModel( );
			m_modelWriterInterface.ResetTimer( );
			m_mainNNetWindow.CenterModel( true );
			m_modelStorage.ResetModelPath( );
		}
		else
			m_computeThread.ReleaseComputationLock( );
		break;

	case IDM_OPEN_MODEL:
		m_computeThread.LockComputation( );  // will be restarted later
		if ( m_modelStorage.AskAndSave( ) && m_modelStorage.AskModelFile() )
			m_modelStorage.Read( );         // will post IDM_READ_MODEL_FINISHED when done
		else
			m_computeThread.ReleaseComputationLock( );
		break;

	case IDM_READ_MODEL_FINISHED:
		if ( bool bSuccess { static_cast<bool>(lParam) } )
		{
			m_StatusBar.DisplayInPart( m_statusMessagePart, L"" );
			Preferences::WritePreferences( m_modelStorage.GetModelPath() );
			m_mainNNetWindow.CenterModel( true );  // computation will be started when done
		}
		break;

	default:
		if ( m_NNetController.HandleCommand( wmId, lParam, NP_NULL ) )
			return true;
		else 
			return BaseWindow::OnCommand( wParam, lParam, pixPoint );
	}

	return true;  // command has been processed
}
