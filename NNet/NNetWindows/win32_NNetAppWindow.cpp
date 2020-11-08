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
#include "win32_messagePump.h"
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
#include "UndoRedoMenu.h"
#include "win32_NNetAppWindow.h"

using namespace std::literals::chrono_literals;

using std::wostringstream;
using std::wstring;
using std::wcout;

class NNetReadModelResult : public ReadModelResult
{
public:
	NNetReadModelResult( HWND hwndApp )
	  : m_hwndApp( hwndApp )
	{ }

	virtual void Reaction( tResult const res, wstring const name )
	{
		switch ( res )
		{
		case ReadModelResult::tResult::ok:
			SendMessage( m_hwndApp, WM_COMMAND, IDM_READ_MODEL_FINISHED, static_cast<LPARAM>( true ) );
			break;

		case ReadModelResult::tResult::fileNotFound:
			MessageBox( nullptr, name.c_str(), L"Could not find model file", MB_OK );
			PostMessage( m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0 );
			break;

		case ReadModelResult::tResult::errorInFile:
			SendMessage( m_hwndApp, WM_COMMAND, IDM_READ_MODEL_FINISHED, static_cast<LPARAM>( false ) );
			break;

		default:
			assert( false );
		}
	};

private:
	HWND m_hwndApp;
};

NNetAppWindow::NNetAppWindow( )
{
	Stopwatch stopwatch;

	m_hwndConsole = GetConsoleWindow( );
//	BringWindowToTop( m_hwndConsole );
	SwitchWcoutTo( L"main_trace.out" );

	wcout << L"*** Application start at " << Util::GetCurrentDateAndTime() << endl;
	wcout << L"*** Computer name: "       << Util::GetComputerName()       << endl;
	wcout << L"*** User name:     "       << Util::GetUserName()           << endl;
	wcout << endl;

	m_preferences.Initialize( & m_sound );
	Neuron::SetSound( & m_sound );
	NNetWindow::InitClass( & m_atDisplay );

	DefineUtilityWrapperFunctions( );
	DefineNNetWrappers( & m_modelCommands );
	DefineNNetWinWrappers( & m_mainNNetWindow );
};

NNetAppWindow::~NNetAppWindow( )
{
//	m_traceStream.close();
}

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

	m_pReadModelResult = new NNetReadModelResult( m_hwndApp );
	m_signalFactory  .Initialize( m_modelReaderInterface, m_parameters, m_dynamicModelObservable );
	m_monitorData    .Initialize( &m_staticModelObservable, &m_signalFactory );
	m_model          .Initialize( &m_monitorData, &m_parameters, &m_staticModelObservable, &m_dynamicModelObservable, &m_modelTimeObservable );
	m_modelStorage   .Initialize( &m_modelReaderInterface, &m_modelWriterInterface, &m_parameters, &m_unsavedChangesObservable, &m_script, m_pReadModelResult, &m_descWindow );
	m_modelCommands  .Initialize( &m_modelWriterInterface, &m_parameters, &m_cmdStack, &m_modelStorage, &m_dynamicModelObservable );
	m_cmdStack       .Initialize( &m_modelWriterInterface, &m_commandStackObservable );
	m_NNetColors     .Initialize( &m_blinkObservable );
	m_sound          .Initialize( &m_soundOnObservable );
	m_beaconAnimation.Initialize( &m_beaconObservable );
	m_NNetController .Initialize
	( 
		& m_modelStorage,
		& m_mainNNetWindow,
		& m_WinManager,
		& m_modelReaderInterface,
		& m_modelCommands,
		& m_computeThread,
		& m_SlowMotionRatio,
		& m_statusBarDispFunctor,
		& m_sound,
		& m_preferences,
		& m_cmdStack,
		& m_monitorWindow,
		& m_parameters,
		& m_dynamicModelObservable
	);

	m_mainNNetWindow   .SetRefreshRate(   0ms );   // immediate refresh
	m_miniNNetWindow   .SetRefreshRate( 200ms );
	m_monitorWindow    .SetRefreshRate(   0ms );
	m_crsrWindow       .SetRefreshRate( 100ms );
	m_performanceWindow.SetRefreshRate( 500ms );
	m_StatusBar        .SetRefreshRate( 300ms );

	m_computeThread         .Start( & m_model, & m_parameters, & m_SlowMotionRatio, & m_runObservable, & m_performanceObservable );
	m_appMenu               .Start( m_hwndApp, & m_computeThread, & m_WinManager, & m_cmdStack, & m_sound );
	m_StatusBar             .Start( m_hwndApp );
	m_descWindow            .Start( m_hwndApp );
	m_undoRedoMenu          .Start( & m_appMenu );
	m_unsavedChangesObserver.Start( m_hwndApp, & m_modelStorage );
	pump.RegisterWindow( m_descWindow.GetWindowHandle(), true );

	m_mainNNetWindow.Start
	( 
		m_hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		false,
		& m_NNetController,
		& m_modelReaderInterface,
		& m_modelCommands,
		& m_cursorPosObservable,
		& m_coordObservable,
		& m_beaconAnimation
	);

	m_miniNNetWindow.Start
	( 
		m_hwndApp, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		true,
		& m_NNetController,
		& m_modelReaderInterface,
		& m_beaconAnimation
	);

	m_miniNNetWindow.ObservedNNetWindow( & m_mainNNetWindow );  // mini window observes main grid window

	SetWindowText( m_miniNNetWindow.GetWindowHandle(), L"Mini window" );

	m_modelReaderInterface.Start( & m_model );
	m_modelWriterInterface.Start( & m_model );
	m_crsrWindow          .Start( m_hwndApp, & m_mainNNetWindow, & m_modelReaderInterface );
	m_parameterDlg        .Start( m_hwndApp, & m_modelCommands, & m_parameters );
	m_performanceWindow   .Start( m_hwndApp, & m_modelReaderInterface, & m_computeThread, & m_SlowMotionRatio, & m_atDisplay );
	m_monitorWindow       .Start( m_hwndApp, & m_NNetController, m_modelReaderInterface, m_parameters, m_beaconAnimation, m_monitorData );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW",    IDM_CONS_WINDOW,    m_hwndConsole,                  true,  true  );
	m_WinManager.AddWindow( L"IDM_APPL_WINDOW",    IDM_APPL_WINDOW,    m_hwndApp,                      true,  true  );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",     IDM_STATUS_BAR,     m_StatusBar.GetWindowHandle(),  false, false );
	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW",    IDM_CRSR_WINDOW,    m_crsrWindow,                   true,  false );
	m_WinManager.AddWindow( L"IDM_DESC_WINDOW",    IDM_DESC_WINDOW,    m_descWindow.GetWindowHandle(), true,  true  );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW",    IDM_MAIN_WINDOW,    m_mainNNetWindow,               true,  false );
	m_WinManager.AddWindow( L"IDM_MINI_WINDOW",    IDM_MINI_WINDOW,    m_miniNNetWindow,               true,  true  );
	m_WinManager.AddWindow( L"IDM_MONITOR_WINDOW", IDM_MONITOR_WINDOW, m_monitorWindow,                true,  true  );
	m_WinManager.AddWindow( L"IDM_PARAM_WINDOW",   IDM_PARAM_WINDOW,   m_parameterDlg,                 true,  false );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW",    IDM_PERF_WINDOW,    m_performanceWindow,            true,  false );

	m_staticModelObservable    .RegisterObserver( & m_modelStorage );
	m_blinkObservable          .RegisterObserver( & m_mainNNetWindow );
	m_beaconObservable         .RegisterObserver( & m_mainNNetWindow );
	m_dynamicModelObservable   .RegisterObserver( & m_mainNNetWindow );
	m_dynamicModelObservable   .RegisterObserver( & m_miniNNetWindow );
	m_dynamicModelObservable   .RegisterObserver( & m_monitorWindow );
	m_staticModelObservable    .RegisterObserver( & m_mainNNetWindow );
	m_staticModelObservable    .RegisterObserver( & m_miniNNetWindow );
	m_staticModelObservable    .RegisterObserver( & m_monitorWindow );
	m_staticModelObservable    .RegisterObserver( & m_performanceWindow );
	m_cursorPosObservable      .RegisterObserver( & m_crsrWindow );
	m_performanceObservable    .RegisterObserver( & m_performanceWindow );
	m_modelTimeObservable      .RegisterObserver( & m_timeDisplay );
	m_runObservable            .RegisterObserver( & m_simulationControl );
	m_SlowMotionRatio          .RegisterObserver( & m_computeThread );
	m_SlowMotionRatio          .RegisterObserver( & m_slowMotionDisplay );
	m_parameters               .RegisterObserver( & m_parameterDlg );
	m_parameters               .RegisterObserver( & m_computeThread );
	m_unsavedChangesObservable .RegisterObserver( & m_unsavedChangesObserver );
	m_soundOnObservable        .RegisterObserver( & m_appMenu );
	m_commandStackObservable   .RegisterObserver( & m_undoRedoMenu );
	m_coordObservable          .RegisterObserver( & m_miniNNetWindow );

	configureStatusBar( );
	adjustChildWindows( );

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

	if ( ! m_WinManager.GetWindowConfiguration( ) )
		Util::Show( m_hwndApp, true );

	m_appMenu.Notify( true );

//	Show( true );

	if ( ! AutoOpen::IsOn( ) || ! m_preferences.ReadPreferences( & m_modelStorage ) )
		m_modelCommands.ResetModel( );

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
	m_monitorWindow       .Stop( );
	m_crsrWindow          .Stop( );
	m_performanceWindow   .Stop( );
	m_parameterDlg        .Stop( );
	m_StatusBar           .Stop( );
	m_modelReaderInterface.Stop( );
	m_modelWriterInterface.Stop( );

	m_staticModelObservable   .UnregisterAllObservers( );
	m_blinkObservable         .UnregisterAllObservers( );
	m_dynamicModelObservable  .UnregisterAllObservers( );
	m_cursorPosObservable     .UnregisterAllObservers( );
	m_performanceObservable   .UnregisterAllObservers( );
	m_modelTimeObservable     .UnregisterAllObservers( );
	m_runObservable           .UnregisterAllObservers( );
	m_SlowMotionRatio         .UnregisterAllObservers( );
	m_parameters              .UnregisterAllObservers( );
	m_unsavedChangesObservable.UnregisterAllObservers( );
	m_soundOnObservable       .UnregisterAllObservers( );
	m_commandStackObservable  .UnregisterAllObservers( );

	m_WinManager.RemoveAll( );

	delete m_pReadModelResult;
	m_pReadModelResult = nullptr;
}

bool NNetAppWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	adjustChildWindows( );
	return true;
}

void NNetAppWindow::OnPaint( )
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
		adjustChildWindows( );
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
	ModelAnalyzer::SetEscFunc         ( & Util::EscapeKeyPressed );
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
		m_computeThread.StopComputation( );
//		m_computeThread.LockComputation( );
		bool bRes { m_modelStorage.AskAndSave( ) };
//		m_computeThread.ReleaseComputationLock( );
		if ( bRes == false )
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

	case IDM_SCRIPT_DIALOG:
//		m_computeThread.LockComputation( );
		m_computeThread.StopComputation( );
		ProcessNNetScript
		( 
			& m_script, 
			& m_modelWriterInterface, 
			ScriptFile::AskForFileName( L"in", L"Script files", tFileMode::read )
		);
//		m_computeThread.ReleaseComputationLock( );
		break;

	case IDM_SCRIPT_PROGRESS:
		m_ScriptHook.DisplayScriptProgress( m_script );
		break;

	case IDM_NEW_MODEL:
		m_computeThread.StopComputation( );
//		m_computeThread.LockComputation( );  // will be restarted when centering complete
		if ( m_modelStorage.AskAndSave( ) )
		{
			m_modelCommands.ResetModel( );
			m_descWindow.ClearDescription( );
			m_mainNNetWindow.Reset();
			m_mainNNetWindow.CenterModel( );
		}
//		else
//			m_computeThread.ReleaseComputationLock( );
		break;

	case IDM_OPEN_MODEL:
		{
			m_computeThread.StopComputation( );
//			m_computeThread.LockComputation( );
			bool bRes { m_modelStorage.AskAndSave( ) };
//			m_computeThread.ReleaseComputationLock( );
			if ( bRes && m_modelStorage.AskModelFile() )
			{
//				m_computeThread.LockComputation( );  // will be restarted later
				m_modelStorage.ReadAsync( );         // will trigger IDM_READ_MODEL_FINISHED when done
			}
		}
		break;

	case IDM_READ_MODEL_FINISHED:
		{
			bool bSuccess { static_cast<bool>(lParam) };
			if ( bSuccess )
			{
				m_StatusBar.DisplayInPart( m_statusMessagePart, L"" );
//				m_preferences.WritePreferences( m_modelStorage.GetModelPath() );
				m_mainNNetWindow.CenterModel( );  // computation will be started when done
			}
			else
			{
				MessageBox( nullptr, m_modelStorage.GetModelPath().c_str(), L"Error in model file. Using default model.", MB_OK );
				OnCommand( IDM_NEW_MODEL, 0, PP_NULL );
			}
		}
		break;

	case IDM_CENTERING_FINISHED:
		m_computeThread.ReleaseComputationLock( );
		break;

	default:
		if ( m_NNetController.HandleCommand( wmId, lParam, NP_NULL ) )
			return true;
	}

	return BaseWindow::OnCommand( wParam, lParam, pixPoint );
}
