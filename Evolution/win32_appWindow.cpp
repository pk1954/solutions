// win32_appWindow.cpp
//

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
#include "win32_aboutBox.h"
#include "win32_statistics.h"
#include "win32_historyInfo.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_displayOptions.h"
#include "win32_status.h"
#include "win32_evoHistWindow.h"
#include "win32_gridWindow.h"
#include "win32_editor.h"

// infrastructure

#include "util.h"
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

#include "win32_resetDlg.h"
#include "win32_appWindow.h"

AppWindow::AppWindow( ) :
    BaseWindow( ),
	m_pGraphics( nullptr ),
	m_pHistorySystem( nullptr ),
	m_pModelDataWork( nullptr ),
	m_hwndConsole( nullptr ),
	m_pEvoCore4Display( nullptr ),
    m_pMainGridWindow( nullptr ),
    m_pMiniGridWindow( nullptr ),
    m_pStatusBar( nullptr ),
    m_pPerfWindow( nullptr ),
    m_pCrsrWindow( nullptr ),
    m_pHistInfoWindow( nullptr ),
    m_pStatistics( nullptr ),
	m_pEvoHistWindow( nullptr ),
	m_pDspOptWindow( nullptr ),
    m_traceStream( ),
	m_bStopped( TRUE )
{
	Stopwatch stopwatch;

	m_hCrsrWait = LoadCursor( NULL, IDC_WAIT );

	m_hwndConsole = GetConsoleWindow( );
	SetWindowPos( m_hwndConsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

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
	m_WorkThreadInterface.Initialize( & m_traceStream ); 

	stopwatch.Stop( L"create window objects" );

	m_pMainGridWindow = new GridWindow( );
	m_pMiniGridWindow = new GridWindow( );
	m_pPerfWindow     = new PerformanceWindow( );
	m_pStatistics     = new StatisticsWindow( );
	m_pHistInfoWindow = new HistInfoWindow( );
	m_pCrsrWindow     = new CrsrWindow( );
	m_pStatusBar      = new StatusBar( );
	m_pFocusPoint     = new FocusPoint( );
	m_pEditorWindow   = new EditorWindow( );
	m_pEvoHistWindow  = new EvoHistWindow( );
	m_pDspOptWindow   = new DspOptWindow( );

	m_pMiniGridWindow->Observe( m_pMainGridWindow );  // mini window observes main grid window

	DefineWin32HistWrapperFunctions( & m_WorkThreadInterface );
	DefineWin32WrapperFunctions    ( & m_WorkThreadInterface );
	DefineWin32EditorWrapperFunctions( m_pEditorWindow );

    m_pStatusBar     ->SetRefreshRate( 300ms );
    m_pCrsrWindow    ->SetRefreshRate( 100ms );
    m_pStatistics    ->SetRefreshRate( 100ms );
    m_pPerfWindow    ->SetRefreshRate( 100ms );
	m_pHistInfoWindow->SetRefreshRate( 300ms );
	m_pEvoHistWindow ->SetRefreshRate( 200ms ); 
	m_pMiniGridWindow->SetRefreshRate( 300ms );
    m_pMainGridWindow->SetRefreshRate( 100ms );
	
	GridWindow::InitClass
	( 
		& m_ReadBuffer, 
		& m_WorkThreadInterface, 
		  m_pFocusPoint, 
		  m_pDspOptWindow, 
		& m_atDisplay, 
		& m_ColorManager 
	);

	m_pCrsrWindow    ->Start( m_hwndApp, & m_ReadBuffer, m_pFocusPoint );
	m_pStatusBar     ->Start( m_hwndApp, & m_ReadBuffer, &m_EvoHistGlue, & m_WorkThreadInterface, & m_Delay, m_pEditorWindow );
	m_pStatistics    ->Start( m_hwndApp, & m_ReadBuffer );
	m_pHistInfoWindow->Start( m_hwndApp, nullptr );
	m_pPerfWindow    ->Start( m_hwndApp, m_Delay, m_atComputation, m_atDisplay, [&](){ return m_WorkThreadInterface.IsRunning(); } );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW", IDM_CONS_WINDOW,   m_hwndConsole,                   TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW,   m_hwndApp,                       TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_PERF_WINDOW", IDM_PERF_WINDOW, * m_pPerfWindow,                   TRUE,  FALSE );
	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW", IDM_CRSR_WINDOW, * m_pCrsrWindow,                   TRUE,  FALSE );
	m_WinManager.AddWindow( L"IDM_STAT_WINDOW", IDM_STAT_WINDOW, * m_pStatistics,                   TRUE,  FALSE );
	m_WinManager.AddWindow( L"IDM_HIST_INFO",   IDM_HIST_INFO,   * m_pHistInfoWindow,               TRUE,  FALSE );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,    m_pStatusBar->GetWindowHandle(), FALSE, FALSE );

	m_EvoController.Start
	( 
		& m_traceStream, 
		& m_WorkThreadInterface,
		& m_WinManager,
		& m_EvoHistGlue,
		& m_Delay, 
		& m_ColorManager,
		& m_AppMenu,
		  m_pStatusBar, 
		  m_pMainGridWindow, 
		  m_pEditorWindow
	);

	m_ScriptHook.Initialize( m_pStatusBar );
	Script::ScrSetWrapHook( & m_ScriptHook );

	GridDimensions::DefineGridSize
	( 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridWidth ) }, 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridHeight ) }, 
		Config::GetConfigValue( Config::tId::nrOfNeighbors ) 
	);
};

void AppWindow::Start( )
{
	EvolutionCore * pCoreWork;
	BOOL            bHexMode = (GridDimensions::GetNrOfNeigbors() == 6);

	m_AppMenu.Start( bHexMode );

	EvolutionCore::InitClass
	( 
		GridDimensions::GetNrOfNeigbors(), 
		& m_ReadBuffer, 
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

    m_pHistorySystem = HistorySystem::CreateHistorySystem( );  // deleted in Stop function

	m_pHistInfoWindow->SetHistorySystem( m_pHistorySystem );

	m_pModelDataWork   = m_EvoHistGlue.Start( m_pHistorySystem, TRUE ); 
	pCoreWork          = m_pModelDataWork->GetEvolutionCore();
	m_pEvoCore4Display = EvolutionCore::CreateCore( );

	m_protocolServer.Start( m_pHistorySystem );
	DefineCoreWrapperFunctions( pCoreWork );  // Core wrappers run in work thread
	m_ReadBuffer.Initialize( pCoreWork, m_pEvoCore4Display );

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

	m_WorkThreadInterface.Start
	( 
		  m_hwndApp, 
		& m_ColorManager, 
		& m_atComputation,
		& m_event, 
		& m_Delay, 
		& m_ReadBuffer, 
		& m_EvoHistGlue
	);
	
	m_pEvoHistWindow->Start( m_hwndApp, m_pFocusPoint, m_pHistorySystem, & m_WorkThreadInterface );
	m_pDspOptWindow ->Start( m_hwndApp );
	m_pEditorWindow ->Start( m_hwndApp, & m_WorkThreadInterface, & m_ReadBuffer, m_pDspOptWindow );
	m_pFocusPoint   ->Start( & m_EvoHistGlue );

	m_WinManager.AddWindow( L"IDM_HIST_WINDOW", IDM_HIST_WINDOW, * m_pEvoHistWindow,  FALSE, FALSE ); 
    m_WinManager.AddWindow( L"IDM_DISP_WINDOW", IDM_DISP_WINDOW, * m_pDspOptWindow,   TRUE,  FALSE );
    m_WinManager.AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, * m_pEditorWindow,   TRUE,  FALSE );
    m_WinManager.AddWindow( L"IDM_MINI_WINDOW", IDM_MINI_WINDOW, * m_pMiniGridWindow, TRUE,  FALSE );
    m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainGridWindow, TRUE,  FALSE );

	m_AppMenu.Initialize( m_hwndApp, & m_WorkThreadInterface, & m_WinManager );

	m_pMiniGridWindow->Size( );
	adjustChildWindows( ); 

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_pStatusBar->ClearStatusLine( );

	m_pStatusBar     ->Show( TRUE );
	m_pEditorWindow  ->Show( TRUE );
	m_pMainGridWindow->Show( TRUE );

	(void)m_pMainGridWindow->SendMessage( WM_COMMAND, IDM_FIT_ZOOM, 0 );
//	Script::ProcessScript( L"std_script.in" );

	m_bStopped = FALSE;
}

void AppWindow::Stop()
{
	m_bStopped = TRUE;

	m_pMiniGridWindow->Stop( );
	m_pMainGridWindow->Stop( );
	m_pEvoHistWindow ->Stop( );
	m_pEditorWindow  ->Stop( );
	m_pDspOptWindow  ->Stop( );
	m_pHistInfoWindow->Stop( );
	m_pPerfWindow    ->Stop( );
	m_pStatistics    ->Stop( );
	m_pCrsrWindow    ->Stop( );

	m_AppMenu            .Stop( );
	m_Delay              .Stop( );
	m_ReadBuffer         .Stop( );
	m_WorkThreadInterface.Stop( );
	m_EvoHistGlue        .Stop( );  // deletes m_pModelDataWork

	m_WinManager.RemoveWindow( IDM_HIST_WINDOW ); 
	m_WinManager.RemoveWindow( IDM_DISP_WINDOW );
	m_WinManager.RemoveWindow( IDM_EDIT_WINDOW );
	m_WinManager.RemoveWindow( IDM_MINI_WINDOW );
	m_WinManager.RemoveWindow( IDM_MAIN_WINDOW );

	m_pStatusBar->Show( FALSE );

	delete m_pHistorySystem;   
	delete m_pEvoCore4Display; 

	m_pModelDataWork   = nullptr;
	m_pGraphics		   = nullptr;
	m_pHistorySystem   = nullptr;
	m_pEvoCore4Display = nullptr;
}

AppWindow::~AppWindow( )
{
	m_WorkThreadInterface.TerminateThread( );

	delete m_pMainGridWindow; 
	delete m_pMiniGridWindow; 
	delete m_pPerfWindow;     
	delete m_pStatistics;     
	delete m_pHistInfoWindow;
	delete m_pCrsrWindow;
	delete m_pStatusBar;     
	delete m_pFocusPoint;     
	delete m_pEditorWindow;
	delete m_pEvoHistWindow;
	delete m_pDspOptWindow;
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
    {
        int const wmId = LOWORD(wParam);
        switch (wmId)
        {
		case IDM_ABOUT:
            ShowAboutBox( GetWindowHandle( ) );
			break;

		case IDM_RESET:
		{
			int     iRes    = ResetDialog::Show( m_hwndApp );
			HCURSOR crsrOld = SetCursor( m_hCrsrWait );
			switch ( iRes )
			{
			case IDM_SOFT_RESET:
			case IDM_HISTORY_RESET:
				m_EvoController.ProcessCommand( iRes );
				break;
			case IDM_HARD_RESET:
				Stop();
				GridDimensions::DefineGridSize
				( 
					GRID_COORD( ResetDialog::GetNewWidth() ), 
					GRID_COORD( ResetDialog::GetNewHeight() ), 
					ResetDialog::GetNewNrOfNeighbors()
				);
				Start();
			}
			SetCursor( crsrOld );
		}
			break;

		case IDM_EXIT:
			PostMessage( WM_CLOSE, 0, 0 );
			break;

		default:
			m_EvoController.ProcessCommand( wParam, lParam );
            break;
        }
    }
    return FALSE;

    case WM_SIZE:
	case WM_MOVE:
		adjustChildWindows( );
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
		if ( ! m_bStopped )
			m_WinManager.StoreWindowConfiguration( );
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

void AppWindow::adjustChildWindows( )
{
    static PIXEL const HIST_WINDOW_HEIGHT = 30_PIXEL;

    PixelRectSize pntAppClientSize( GetClRectSize( ) );
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

    if ( pntAppClientSize.IsNotZero( ) )
    {
        m_pStatusBar->Resize( );
        pixAppClientWinHeight -= m_pStatusBar->GetHeight( );
		pixAppClientWinHeight -= HIST_WINDOW_HEIGHT, 
        m_pEvoHistWindow->Move   // adapt history window to new size
		( 
			0_PIXEL, 
			pixAppClientWinHeight, 
			pixAppClientWinWidth, 
			HIST_WINDOW_HEIGHT, 
			TRUE 
		); 
        m_pMainGridWindow->Move
		( 
			0_PIXEL, 
			0_PIXEL, 
			pixAppClientWinWidth, 
			pixAppClientWinHeight, 
			TRUE 
		);
    }
}

//int MyAllocHook
//(
//	int allocType, 
//	void * userData, 
//	size_t size,
//	int blockType, 
//	long requestNumber,
//	const unsigned char* filename, 
//	int lineNumber
//)
//{
//	int x;
//
//	switch (allocType)
//	{
//	case _HOOK_ALLOC:
//		x = 1;
//		break;
//
//	case _HOOK_REALLOC:
//		x = 2;
//		break;
//
//	case _HOOK_FREE:
//		x = 3;
//		break;
//
//	default:
//		break;
//	}
//
//	return TRUE;
//}
//
