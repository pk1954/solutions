// win32_appWindow.cpp
//
// Evolutopn

#include "stdafx.h"

#include <chrono>
#include "WinUser.h"

using namespace std::literals::chrono_literals;

// EvolutionCore interfaces

#include "config.h"
#include "gridRect.h"
#include "GridDimensions.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCore.h"

// interfaces of various windows

#include "win32_appWindow.h"
#include "win32_statistics.h"
//#include "win32_historyInfo.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_displayOptions.h"
#include "win32_status.h"
#include "GenerationDisplay.h"
#include "win32_histWindow.h"
#include "win32_gridWindow.h"
#include "win32_editor.h"
#include "win32_speedControl.h"
#include "win32_zoomControl.h"

// infrastructure

#include "util.h"
#include "LogarithmicTrackbar.h"
#include "pixelTypes.h"
#include "ObserverInterface.h"
#include "win32_focusPoint.h"

// scripting and tracing

#include "dump.h"
#include "trace.h"
#include "errhndl.h"
#include "script.h"
#include "UtilityWrappers.h"
#include "win32_stopwatch.h"
#include "win32_wrappers.h"
#include "win32_editorWrappers.h"
#include "win32_histWrappers.h"

// system and resources

#include "Resource.h"
#include "d3d_system.h"

// application

#include "win32_appWindow.h"

AppWindow::AppWindow( ) :
    BaseAppWindow( & m_EvoWorkThreadInterface ),
	m_pGraphics( nullptr ),
	m_pModelDataWork( nullptr ),
	m_pEvoCore4Display( nullptr ),
    m_pMainGridWindow( nullptr ),
    m_pMiniGridWindow( nullptr ),
    m_pPerfWindow( nullptr ),
    m_pCrsrWindow( nullptr ),
    m_pStatistics( nullptr ),
	m_pDspOptWindow( nullptr ),
	m_pGenerationDisplay( nullptr ),
    m_traceStream( ),
	m_bStarted( FALSE )
{
	Stopwatch stopwatch;

//	_CrtSetAllocHook( MyAllocHook );

	DefineUtilityWrapperFunctions( );

	m_hwndApp = StartBaseWindow
	( 
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	m_traceStream = OpenTraceFile( L"main_trace.out" );

	DUMP::SetDumpStream( & std::wcout );
	Config::SetDefaultConfiguration( );
    Config::DefineConfigWrapperFunctions( );
	Script::ProcessScript( L"std_configuration.in" );

    // create window objects

	stopwatch.Start();
	m_ColorManager.Initialize( );
	m_EvoWorkThreadInterface.Initialize( & m_traceStream ); 

	stopwatch.Stop( L"create window objects" );

	m_pDspOptWindow   = new DspOptWindow( );
	m_pFocusPoint     = new FocusPoint( );
	m_pPerfWindow     = new PerformanceWindow( );
	m_pStatistics     = new StatisticsWindow( );
	m_pCrsrWindow     = new CrsrWindow( );
	m_pEditorWindow   = new EditorWindow( );

	GridWindow::InitClass
	( 
		& m_EvoReadBuffer, 
		& m_EvoWorkThreadInterface, 
		  m_pFocusPoint, 
		  m_pDspOptWindow, 
		& m_atDisplay, 
		& m_ColorManager 
	);

	m_pMainGridWindow = new GridWindow( );
	m_pMiniGridWindow = new GridWindow( );

	m_EvoController.Initialize
	( 
		this,
		& m_EvoWorkThreadInterface,
		& m_WinManager,
		& m_EvoHistGlue,
		& m_Delay, 
		& m_ColorManager,
		& m_AppMenu,
		  m_pStatusBar, 
		  m_pMainGridWindow, 
		  m_pEditorWindow
	);

	m_pMiniGridWindow->Observe( m_pMainGridWindow );  // mini window observes main grid window

	DefineWin32HistWrapperFunctions( & m_EvoWorkThreadInterface );
	DefineWin32WrapperFunctions    ( & m_EvoWorkThreadInterface );
	DefineWin32EditorWrapperFunctions( m_pEditorWindow );

    m_pCrsrWindow    ->SetRefreshRate( 100ms );
    m_pStatistics    ->SetRefreshRate( 100ms );
    m_pPerfWindow    ->SetRefreshRate( 100ms );
	m_pMiniGridWindow->SetRefreshRate( 300ms );
    m_pMainGridWindow->SetRefreshRate( 100ms );
	m_pEditorWindow  ->SetRefreshRate( 300ms );
	m_pDspOptWindow  ->SetRefreshRate( 300ms );

	GridDimensions::DefineGridSize
	( 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridWidth ) }, 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridHeight ) }, 
		Config::GetConfigValue( Config::tId::nrOfNeighbors ) 
	);
};

AppWindow::~AppWindow( )
{
	delete m_pMainGridWindow; 
	delete m_pMiniGridWindow; 
	delete m_pPerfWindow;     
	delete m_pStatistics;     
	delete m_pCrsrWindow;
	delete m_pFocusPoint;     
	delete m_pEditorWindow;
	delete m_pDspOptWindow;
	delete m_pGenerationDisplay;
}

void AppWindow::Start( )
{
	EvolutionCore * pCoreWork;
	BOOL            bHexMode = (GridDimensions::GetNrOfNeigbors() == 6);

	m_AppMenu.Start( bHexMode );

	EvolutionCore::InitClass
	( 
		GridDimensions::GetNrOfNeigbors(), 
		& m_EvoReadBuffer, 
		& m_event
	);

	m_D3d_driver.Initialize
	( 
		m_hwndApp, 
		GridDimensions::GridWidthVal(), 
		GridDimensions::GridHeightVal(), 
		bHexMode 
	);

	m_pGraphics = & m_D3d_driver;

	BaseAppWindow::Start( m_pMainGridWindow, m_hwndApp );
	m_AppMenu.Initialize( m_hwndApp, & m_EvoWorkThreadInterface, & m_WinManager );

	m_pModelDataWork = m_EvoHistGlue.Start( m_pHistorySystem, TRUE ); 
	m_protocolServer.Start( m_pHistorySystem );

	pCoreWork          = m_pModelDataWork->GetEvolutionCore();
	m_pEvoCore4Display = EvolutionCore::CreateCore( );

	DefineCoreWrapperFunctions( pCoreWork );  // Core wrappers run in work thread
	m_EvoReadBuffer.Initialize( pCoreWork, m_pEvoCore4Display );

	m_EvoWorkThreadInterface.Start
	( 
		  m_hwndApp, 
		& m_ColorManager, 
		& m_atComputation,
		& m_event, 
		& m_Delay, 
		& m_EvoReadBuffer, 
		& m_EvoHistGlue
	);
	
	m_pMainGridWindow->Start
	( 
		m_hwndApp, 
		m_pGraphics, 
		WS_CHILD | WS_CLIPSIBLINGS, 
		16_PIXEL, 
		nullptr
	);

    m_pMiniGridWindow->Start
	( 
		m_hwndApp, 
		m_pGraphics, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION, 
		2_PIXEL, 
		[&]() { return ! m_pMainGridWindow->IsFullGridVisible( ); }
	);

	m_pDspOptWindow  ->Start( m_hwndApp );
	m_pEditorWindow  ->Start( m_hwndApp, & m_EvoWorkThreadInterface, & m_EvoReadBuffer, m_pDspOptWindow );
	m_pFocusPoint    ->Start( & m_EvoHistGlue );
	m_pCrsrWindow    ->Start( m_hwndApp, & m_EvoReadBuffer, m_pFocusPoint );
	m_pStatistics    ->Start( m_hwndApp, & m_EvoReadBuffer );
	m_pPerfWindow    ->Start( m_hwndApp, m_Delay, m_atComputation, m_atDisplay, [&](){ return m_EvoWorkThreadInterface.IsRunning(); } );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW", IDM_CONS_WINDOW,   m_hwndConsole,     TRUE, TRUE  );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW", IDM_PERF_WINDOW, * m_pPerfWindow,     TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW", IDM_CRSR_WINDOW, * m_pCrsrWindow,     TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_STAT_WINDOW", IDM_STAT_WINDOW, * m_pStatistics,     TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_DISP_WINDOW", IDM_DISP_WINDOW, * m_pDspOptWindow,   TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, * m_pEditorWindow,   TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_MINI_WINDOW", IDM_MINI_WINDOW, * m_pMiniGridWindow, TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainGridWindow, TRUE, FALSE );

	configureStatusBar( );

	m_pMiniGridWindow->Size( );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_pEditorWindow->Show( TRUE );

	(void)m_pMainGridWindow->SendMessage( WM_COMMAND, IDM_FIT_ZOOM, 0 );
//	Script::ProcessScript( L"std_script.in" );

	m_bStarted = TRUE;
}

void AppWindow::Stop()
{
	m_bStarted = FALSE;

	m_pMiniGridWindow->Stop( );
	m_pMainGridWindow->Stop( );
	m_pEditorWindow  ->Stop( );
	m_pDspOptWindow  ->Stop( );
	m_pPerfWindow    ->Stop( );
	m_pStatistics    ->Stop( );
	m_pCrsrWindow    ->Stop( );
	m_pStatusBar     ->Stop( );

	m_AppMenu               .Stop( );
	m_Delay                 .Stop( );
	m_EvoReadBuffer         .Stop( );
	m_EvoWorkThreadInterface.Stop( );
	m_EvoHistGlue           .Stop( );

	m_WinManager.RemoveAll( );

	BaseAppWindow::Stop();

	delete m_pEvoCore4Display; 

	m_pModelDataWork   = nullptr;
	m_pGraphics		   = nullptr;  // Set in Start
	m_pEvoCore4Display = nullptr;
}

LRESULT AppWindow::UserProc
( 
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
    switch ( message )
    {

	case WM_ENTERMENULOOP:
		if ( wParam == FALSE )
			m_AppMenu.AdjustVisibility( );
		break;

	case WM_COMMAND:
		m_EvoController.ProcessCommand( wParam, lParam );
	    return FALSE;

    case WM_SIZE:
	case WM_MOVE:
		AdjustChildWindows( );
		break;

	case WM_PAINT:
	{
		static COLORREF const CLR_GREY = RGB( 128, 128, 128 );
		PAINTSTRUCT   ps;
		HDC           hDC = BeginPaint( &ps );
		FillBackground( hDC, CLR_GREY );
		(void)EndPaint( &ps );
		return FALSE;
	}

	case WM_CLOSE:
		if ( m_bStarted )
		{
			m_WinManager.StoreWindowConfiguration( );
		    Stop( );
		}
		DestroyWindow( );        
		return TRUE;  

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

    default:
        break;
    }
    
    return DefWindowProc( message, wParam, lParam );
}

void AppWindow::configureStatusBar( )
{
	m_pGenerationDisplay = new GenerationDisplay( m_pStatusBar, & m_EvoReadBuffer, 0 );

	m_pStatusBar->NewPart( );
	m_pStatusBar->AddButton( L"Show editor", (HMENU)IDM_EDIT_WINDOW, BS_PUSHBUTTON );

	m_pStatusBar->NewPart( );
	ZoomControl::AddSizeControl( m_pStatusBar, MINIMUM_FIELD_SIZE.GetValue(), MAXIMUM_FIELD_SIZE.GetValue() );
	ZoomControl::SetSizeTrackBar( m_pStatusBar, DEFAULT_FIELD_SIZE );

	m_pStatusBar->NewPart( );
	SpeedControl::AddSimulationControl( m_pStatusBar, & m_EvoWorkThreadInterface, m_pHistorySystem, Config::UseHistorySystem( ) );
	SpeedControl::SetSpeedTrackBar( DEFAULT_DELAY );

	int iPartScriptLine = m_pStatusBar->NewPart( );
	m_ScriptHook.Initialize( m_pStatusBar, iPartScriptLine );
	m_pStatusBar->DisplayInPart( iPartScriptLine, L"" );
	Script::ScrSetWrapHook( & m_ScriptHook );

	m_pStatusBar->LastPart( );
}
