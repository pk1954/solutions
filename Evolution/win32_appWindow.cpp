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

// history system

#include "EvoHistorySysGlue.h"

// interfaces of various windows

#include "win32_gridWindow.h"
#include "win32_evoHistWindow.h"
#include "win32_editor.h"
#include "win32_displayOptions.h"
#include "win32_appWindow.h"
#include "win32_status.h"
#include "win32_statistics.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_historyInfo.h"
#include "win32_aboutBox.h"

// infrastructure

#include "util.h"
#include "pixelTypes.h"
#include "win32_winManager.h"
#include "win32_workThreadInterface.h"
#include "win32_focusPoint.h"
#include "win32_colorManager.h"
#include "win32_readBuffer.h"
#include "ObserverInterface.h"

// scripting and tracing

#include "dump.h"
#include "trace.h"
#include "errhndl.h"
#include "script.h"
#include "UtilityWrappers.h"
#include "win32_stopwatch.h"
#include "win32_scriptHook.h"
#include "win32_wrappers.h"
#include "win32_editorWrappers.h"
#include "win32_histWrappers.h"

// system and resources

#include "Resource.h"

#include "GDI_driver.h"
#include "D3D_driver.h"
#include "d3d_system.h"

// application

#include "win32_appMenu.h"
#include "win32_resetDlg.h"
#include "win32_evoController.h"
#include "win32_appWindow.h"

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

AppWindow::AppWindow( ) :
    BaseWindow( ),
	m_pD3d_driver( nullptr ),
    m_pMainGridWindow( nullptr ),
    m_pMiniGridWindow( nullptr ),
    m_pWorkThreadInterface( nullptr ),
    m_pPerfWindow( nullptr ),
    m_pEditorWindow( nullptr ),
    m_pCrsrWindow( nullptr ),
	m_pHistInfoWindow( nullptr ),
    m_pStatusBar( nullptr ),
    m_pStatistics( nullptr ),
	m_pDspOptWindow( nullptr ),
	m_pFocusPoint( nullptr ),
	m_pWinManager( nullptr ),
	m_pGraphics( nullptr ),
	m_pHistorySystem( nullptr ),
	m_pReadBuffer( nullptr ),
	m_pModelDataWork( nullptr ),
    m_pScriptHook( nullptr ),
    m_pEvoHistWindow( nullptr ),
	m_pEvoHistGlue( nullptr ),
	m_pEvoController( nullptr ),
	m_hwndConsole( nullptr ),
	m_pCoreObservers( nullptr ),
	m_pEvoCore4Display( nullptr ),
	m_pAppMenu( nullptr ),
	m_traceStream( ),
	m_bStopped( TRUE )
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
		nullptr
	);

	m_traceStream = OpenTraceFile( L"main_trace.out" );
	m_hwndConsole = Util::StdOutConsole( );

	DUMP::SetDumpStream( & std::wcout );
	Config::SetDefaultConfiguration( );
    Config::DefineConfigWrapperFunctions( );
	Script::ProcessScript( L"std_configuration.in" );

    // create window objects

	stopwatch.Start();
    m_pFocusPoint          = new FocusPoint( );                          
    m_pWinManager          = new WinManager( );  						 
	m_pColorManager        = new ColorManager( );						 
    m_pStatusBar           = new StatusBar( );       					 
    m_pCrsrWindow          = new CrsrWindow( );   						 
	m_pHistInfoWindow      = new HistInfoWindow( );						 
    m_pDspOptWindow        = new DspOptWindow( );       				 
    m_pEditorWindow        = new EditorWindow( );    					 
    m_pStatistics          = new StatisticsWindow( );   				 
    m_pMainGridWindow      = new GridWindow( );   						 
    m_pMiniGridWindow      = new GridWindow( );   						 
    m_pPerfWindow          = new PerformanceWindow( );  				 
    m_pEvoHistWindow       = new EvoHistWindow( );						 
	m_pEvoController       = new EvoController( );						 
	m_pEvoHistGlue         = new EvoHistorySysGlue( );					 
	m_pWorkThreadInterface = new WorkThreadInterface( & m_traceStream ); 
	m_pD3d_driver          = new D3D_driver( );                  
	m_pCoreObservers       = new ViewCollection();
	m_pReadBuffer          = new ReadBuffer( m_pCoreObservers ); 
	m_pAppMenu             = new AppMenu( m_hwndApp, m_pWinManager, m_pStatusBar );

	stopwatch.Stop( L"create window objects" );

	m_pMiniGridWindow->Observe( m_pMainGridWindow );  // mini window observes main grid window

	DefineWin32HistWrapperFunctions( m_pWorkThreadInterface );
	DefineWin32WrapperFunctions( m_pWorkThreadInterface );
	DefineWin32EditorWrapperFunctions( m_pEditorWindow );

    m_pStatusBar     ->SetRefreshRate( 300ms );
    m_pEvoHistWindow ->SetRefreshRate( 200ms ); 
    m_pCrsrWindow    ->SetRefreshRate( 100ms );
    m_pStatistics    ->SetRefreshRate( 100ms );
    m_pPerfWindow    ->SetRefreshRate( 100ms );
	m_pHistInfoWindow->SetRefreshRate( 300ms );
    m_pMiniGridWindow->SetRefreshRate( 300ms );
    m_pMainGridWindow->SetRefreshRate( 100ms );
	
	m_pCoreObservers->AttachObserver( m_pStatusBar      );
	m_pCoreObservers->AttachObserver( m_pEvoHistWindow  ); 
	m_pCoreObservers->AttachObserver( m_pCrsrWindow     );
	m_pCoreObservers->AttachObserver( m_pStatistics     );
	m_pCoreObservers->AttachObserver( m_pPerfWindow     );
	m_pCoreObservers->AttachObserver( m_pMiniGridWindow );
	m_pCoreObservers->AttachObserver( m_pMainGridWindow );

	GridWindow::InitClass
	( 
		m_pReadBuffer, 
		m_pWorkThreadInterface, 
		m_pFocusPoint, 
		m_pDspOptWindow, 
		m_pPerfWindow, 
		m_pColorManager 
	);

	m_pCrsrWindow    ->Start( m_hwndApp, m_pReadBuffer, m_pFocusPoint );
	m_pStatusBar     ->Start( m_hwndApp, m_pReadBuffer );
	m_pStatistics    ->Start( m_hwndApp, m_pReadBuffer );
	m_pHistInfoWindow->Start( m_hwndApp);
	m_pPerfWindow    ->Start( m_hwndApp );

	m_pWinManager->AddWindow( L"IDM_CONS_WINDOW", IDM_CONS_WINDOW, m_hwndConsole,                        TRUE,  TRUE  );
	m_pWinManager->AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, m_hwndApp,                            TRUE,  TRUE  );
	m_pWinManager->AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_pStatusBar     ->GetWindowHandle(), FALSE, FALSE );
	m_pWinManager->AddWindow( L"IDM_PERF_WINDOW", IDM_PERF_WINDOW, m_pPerfWindow    ->GetWindowHandle(), TRUE,  FALSE );
	m_pWinManager->AddWindow( L"IDM_CRSR_WINDOW", IDM_CRSR_WINDOW, m_pCrsrWindow    ->GetWindowHandle(), TRUE,  FALSE );
	m_pWinManager->AddWindow( L"IDM_STAT_WINDOW", IDM_STAT_WINDOW, m_pStatistics    ->GetWindowHandle(), TRUE,  FALSE );
	m_pWinManager->AddWindow( L"IDM_HIST_INFO",   IDM_HIST_INFO,   m_pHistInfoWindow->GetWindowHandle(), TRUE,  FALSE );

	m_pEvoController->Start
	( 
		& m_traceStream, 
		m_pWorkThreadInterface, 
		m_pWinManager, 
		m_pPerfWindow, 
		m_pStatusBar, 
		m_pMainGridWindow, 
		m_pEditorWindow, 
		m_pColorManager,
		m_pAppMenu
	);

	m_pScriptHook = new ScriptHook( m_pStatusBar );
	Script::ScrSetWrapHook( m_pScriptHook );

	GridDimensions::DefineGridSize
	( 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridWidth ) }, 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridHeight ) }, 
		Config::GetConfigValue( Config::tId::nrOfNeighbors ) 
	);
};

void AppWindow::Start(  )
{
	EvolutionCore * pCoreWork;

	EvolutionCore::InitClass
	( 
		GridDimensions::GetNrOfNeigbors(), 
		m_pCoreObservers, 
		& m_event 
	);

	m_pD3d_driver->Initialize
	( 
		m_hwndApp, 
		GridDimensions::GridWidthVal(), 
		GridDimensions::GridHeightVal(), 
		GridDimensions::GetNrOfNeigbors() == 6 
	);

	m_pGraphics = m_pD3d_driver;

    m_pHistorySystem = HistorySystem::CreateHistorySystem( );  // deleted in Stop function

	m_pHistInfoWindow->SetHistorySystem( m_pHistorySystem );

	m_pModelDataWork   = m_pEvoHistGlue->Start( m_pHistorySystem, true, m_pHistInfoWindow );  // m_pEvoHistGlue->Stop deletes 
	pCoreWork          = m_pModelDataWork->GetEvolutionCore();
	m_pEvoCore4Display = EvolutionCore::CreateCore( );

	DefineCoreWrapperFunctions( pCoreWork );
	m_pReadBuffer->Initialize( pCoreWork, m_pEvoCore4Display );

	m_pMainGridWindow->Start( m_hwndApp, m_pGraphics, WS_CHILD       | WS_CLIPSIBLINGS,             16_PIXEL );
    m_pMiniGridWindow->Start( m_hwndApp, m_pGraphics, WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION, 2_PIXEL );
	m_pEvoHistWindow ->Start( m_hwndApp, m_pFocusPoint, m_pHistorySystem, m_pWorkThreadInterface );
	m_pDspOptWindow  ->Start( m_hwndApp, pCoreWork );
    m_pEditorWindow  ->Start( m_hwndApp, m_pWorkThreadInterface, pCoreWork, m_pDspOptWindow, m_pStatusBar );

	m_pAppMenu            ->Start();
	m_pFocusPoint         ->Start( m_pEvoHistGlue, pCoreWork );
	m_pWorkThreadInterface->Start( m_hwndApp, m_pColorManager, m_pPerfWindow, m_pEditorWindow, & m_event, m_pReadBuffer, pCoreWork, m_pEvoHistGlue );
	
    m_pWinManager->AddWindow( L"IDM_HIST_WINDOW", IDM_HIST_WINDOW, m_pEvoHistWindow ->GetWindowHandle(), FALSE, FALSE ); 
    m_pWinManager->AddWindow( L"IDM_DISP_WINDOW", IDM_DISP_WINDOW, m_pDspOptWindow  ->GetWindowHandle(), TRUE, FALSE );
    m_pWinManager->AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, m_pEditorWindow  ->GetWindowHandle(), TRUE, FALSE );
    m_pWinManager->AddWindow( L"IDM_MINI_WINDOW", IDM_MINI_WINDOW, m_pMiniGridWindow->GetWindowHandle(), TRUE, FALSE );
    m_pWinManager->AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, m_pMainGridWindow->GetWindowHandle(), TRUE, FALSE );

    m_pMiniGridWindow->Size( );
	adjustChildWindows( ); 

	if ( ! m_pWinManager->GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_pStatusBar->ClearStatusLine( );
	m_pStatusBar->Show( TRUE );
	m_pEditorWindow->SetSimulationMode( );

	(void)m_pMainGridWindow->SendMessage( WM_COMMAND, IDM_FIT_ZOOM, 0 );
	m_pEvoController->ProcessCommand( IDM_SET_SIMU_MODE, static_cast<LPARAM>(tBoolOp::opFalse) );
//	Script::ProcessScript( L"std_script.in" );

	m_bStopped = FALSE;
}

void AppWindow::Stop()
{
	m_bStopped = TRUE;

	m_pMiniGridWindow->Stop( );
	m_pMainGridWindow->Stop( );
	m_pEvoHistWindow->Stop( );
	m_pEditorWindow->Stop( );
	m_pDspOptWindow->Stop( );
	m_pAppMenu->Stop();

	m_pHistInfoWindow->SetHistorySystem( nullptr );

	m_pWinManager->RemoveWindow( IDM_HIST_WINDOW ); 
	m_pWinManager->RemoveWindow( IDM_DISP_WINDOW );
	m_pWinManager->RemoveWindow( IDM_EDIT_WINDOW );
	m_pWinManager->RemoveWindow( IDM_MINI_WINDOW );
	m_pWinManager->RemoveWindow( IDM_MAIN_WINDOW );

	m_pEvoHistGlue->Stop( );  // deletes m_pModelDataWork

	m_pStatusBar->Show( FALSE );
	m_pStatistics->Show( FALSE );
	m_pPerfWindow->Show( FALSE );
	m_pCrsrWindow->Show( FALSE );

	delete m_pHistorySystem;   
	delete m_pEvoCore4Display; 

	m_pModelDataWork   = nullptr;
	m_pGraphics		   = nullptr;
	m_pHistorySystem   = nullptr;
	m_pEvoCore4Display = nullptr;
}

AppWindow::~AppWindow( )
{
	m_pWorkThreadInterface->TerminateThread( );

	m_pPerfWindow->TerminateTextWindow();
	m_pCrsrWindow->TerminateTextWindow();
	m_pStatistics->TerminateTextWindow();
	m_pHistInfoWindow->TerminateTextWindow();

	delete m_pStatusBar;
	delete m_pWorkThreadInterface;
	delete m_pEvoHistGlue;
	delete m_pEvoController;
	delete m_pEvoHistWindow;
	delete m_pPerfWindow;
	delete m_pMiniGridWindow;
	delete m_pMainGridWindow;
	delete m_pStatistics;
	delete m_pEditorWindow;
	delete m_pDspOptWindow;
	delete m_pHistInfoWindow;
	delete m_pCrsrWindow;
	delete m_pColorManager;
	delete m_pWinManager;
	delete m_pFocusPoint;
	delete m_pD3d_driver;
	delete m_pCoreObservers;  
	delete m_pReadBuffer;     
	delete m_pScriptHook;  
	delete m_pAppMenu;

	m_pStatusBar           = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pEvoHistGlue		   = nullptr;
	m_pEvoController	   = nullptr;
	m_pEvoHistWindow	   = nullptr;
	m_pPerfWindow		   = nullptr;
	m_pMiniGridWindow	   = nullptr;
	m_pMainGridWindow	   = nullptr;
	m_pStatistics		   = nullptr;
	m_pEditorWindow		   = nullptr;
	m_pDspOptWindow		   = nullptr;
	m_pHistInfoWindow	   = nullptr;
	m_pCrsrWindow		   = nullptr;
	m_pColorManager		   = nullptr;
	m_pWinManager		   = nullptr;
	m_pFocusPoint		   = nullptr;
	m_pD3d_driver		   = nullptr;
	m_pCoreObservers       = nullptr;
	m_pReadBuffer	       = nullptr;
	m_pScriptHook	       = nullptr;
	m_pAppMenu             = nullptr;
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

    case WM_COMMAND:
    {
        int const wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            ShowAboutBox( GetWindowHandle( ) );
            break;

        case IDM_ADJUST_MINI_WIN:
			adjustMiniWinVisibility( static_cast<int>(lParam) );
            break;

		case IDM_RESET:
		{
			int iRes = ResetDialog::Show( m_hwndApp );
			switch ( iRes )
			{
			case IDM_SOFT_RESET:
			case IDM_HISTORY_RESET:
				m_pEvoController->ProcessCommand( iRes, 0 );
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
		}
			break;

		case IDM_EXIT:
			PostMessage( WM_CLOSE, 0, 0 );
			break;

		default:
			m_pEvoController->ProcessCommand( wParam, lParam );
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
			m_pWinManager->StoreWindowConfiguration( );
		DestroyWindow( GetWindowHandle( ) );        
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

    if ( pntAppClientSize.IsNotZero( ) && ( m_pStatusBar != nullptr ) )
    {
        m_pStatusBar->Resize( );
        pixAppClientWinHeight -= m_pStatusBar->GetHeight( );

        if ( m_pEvoHistWindow != nullptr )
        {
			pixAppClientWinHeight -= HIST_WINDOW_HEIGHT, 
            m_pEvoHistWindow->Move   // adapt history window to new size
			( 
				0_PIXEL, 
				pixAppClientWinHeight, 
				pixAppClientWinWidth, 
				HIST_WINDOW_HEIGHT, 
				TRUE 
			); 
        }

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

void AppWindow::adjustMiniWinVisibility( int const iMode )
{
	Config::tOnOffAuto onOffAuto = Config::tOnOffAuto::automatic;
	if ( iMode != 0 )
	{
		switch ( iMode )
		{
			case IDM_MINI_WINDOW_ON:
				onOffAuto = Config::tOnOffAuto::on; 
				break;
			case IDM_MINI_WINDOW_OFF:
				onOffAuto = Config::tOnOffAuto::off; 
				break;
			case IDM_MINI_WINDOW_AUTO:
				onOffAuto = Config::tOnOffAuto::automatic; 
				break;
			default:
				assert( false );
				break;
		}
		Config::SetConfigValue( Config::tId::miniGridDisplay, static_cast<long>( onOffAuto ) ); 
	}
	else 
	{
		onOffAuto = Config::GetConfigValueOnOffAuto( Config::tId::miniGridDisplay ); 
	}

	switch ( onOffAuto )
	{
		case Config::tOnOffAuto::on:
			m_pMiniGridWindow->Show( true );
			break;

		case Config::tOnOffAuto::off:
			m_pMiniGridWindow->Show( false );
			break;

		case Config::tOnOffAuto::automatic:
			m_pMiniGridWindow->Show( ! m_pMainGridWindow->IsFullGridVisible( ) );
			break;
	}
}
