// win32_evoAppWindow.cpp
//
// Evolutopn



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

#include "win32_evoAppWindow.h"
#include "win32_statistics.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_displayOptions.h"
#include "win32_status.h"
#include "GenerationDisplay.h"
#include "win32_histWindow.h"
#include "win32_gridWindow.h"
#include "win32_EvoEditor.h"
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
#include "win32_EvoEditorWrappers.h"
#include "win32_histWrappers.h"

// system and resources

#include "Resource.h"
#include "d3d_system.h"

// application

#include "win32_evoAppWindow.h"

EvoAppWindow::EvoAppWindow( ) :
	m_pGraphics( nullptr ),
	m_pModelDataWork( nullptr ),
	m_pEvoCore4Display( nullptr ),
    m_pMainGridWindow( nullptr ),
    m_pMiniGridWindow( nullptr ),
    m_pPerfWindow( nullptr ),
    m_pCrsrWindow( nullptr ),
    m_pStatistics( nullptr ),
	m_pDspOptWindow( nullptr ),
	m_pGenerationDisplay( nullptr )
{
	Stopwatch stopwatch;

	m_pEvoWorkThreadInterface = new EvoWorkThreadInterface();

	BaseAppWindow::Initialize( m_pEvoWorkThreadInterface, TRUE );

	m_pEvoReadBuffer = new EvoReadBuffer( TRUE );  // use double buffering

	DUMP::SetDumpStream( & std::wcout );
	Config::SetDefaultConfiguration( );
    Config::DefineConfigWrapperFunctions( );
	Script::ProcessScript( L"std_configuration.in" );

	stopwatch.Start();

	m_ColorManager.Initialize( );

	m_pAppMenu         = new EvoAppMenu( );
	m_pDspOptWindow    = new DspOptWindow( );
	m_pFocusPoint      = new FocusPoint( );
	m_pPerfWindow      = new PerformanceWindow( );
	m_pStatistics      = new StatisticsWindow( );
	m_pCrsrWindow      = new CrsrWindow( );
	m_pEvoEditorWindow = new EvoEditorWindow( );

	GridWindow::InitClass
	( 
		  m_pEvoReadBuffer, 
		  m_pEvoWorkThreadInterface, 
		  m_pFocusPoint, 
		  m_pDspOptWindow, 
		& m_atDisplay, 
		& m_ColorManager 
	);

	m_pMainGridWindow = new GridWindow( );
	m_pMiniGridWindow = new GridWindow( );

	m_pEvoController = new EvoController
	( 
		& m_WinManager,
		& m_EvoHistGlue,
		& m_Delay, 
		& m_ColorManager,
		& m_StatusBar, 
		  m_pMainGridWindow, 
		  m_pEvoEditorWindow
	);

	stopwatch.Stop( L"create window objects" );

	m_pEvoController->Initialize( this, m_pEvoWorkThreadInterface );

	m_pMiniGridWindow->Observe( m_pMainGridWindow );  // mini window observes main grid window

	DefineWin32HistWrapperFunctions  ( m_pEvoWorkThreadInterface );
	DefineWin32WrapperFunctions      ( m_pEvoWorkThreadInterface );
	DefineWin32EditorWrapperFunctions( m_pEvoEditorWindow );

    m_pCrsrWindow     ->SetRefreshRate( 100ms );
    m_pStatistics     ->SetRefreshRate( 100ms );
    m_pPerfWindow     ->SetRefreshRate( 100ms );
	m_pMiniGridWindow ->SetRefreshRate( 300ms );
    m_pMainGridWindow ->SetRefreshRate( 100ms );
	m_pEvoEditorWindow->SetRefreshRate( 300ms );
	m_pDspOptWindow   ->SetRefreshRate( 300ms );

	GridDimensions::DefineGridSize
	( 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridWidth ) }, 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridHeight ) }, 
		Config::GetConfigValue( Config::tId::nrOfNeighbors ) 
	);
};

EvoAppWindow::~EvoAppWindow( )
{
	delete m_pEvoController;
	delete m_pMainGridWindow; 
	delete m_pMiniGridWindow; 
	delete m_pPerfWindow;     
	delete m_pStatistics;     
	delete m_pCrsrWindow;
	delete m_pFocusPoint;     
	delete m_pEvoEditorWindow;
	delete m_pDspOptWindow;
	delete m_pGenerationDisplay;
	delete m_pAppMenu;
	delete m_pEvoWorkThreadInterface;
}

void EvoAppWindow::Start( )
{
	EvolutionCore * pCoreWork;

	EvolutionCore::InitClass
	( 
		GridDimensions::GetNrOfNeigbors(), 
		  m_pEvoReadBuffer, 
		& m_eventPOI
	);

	m_D3d_driver.Initialize
	( 
		m_hwndApp, 
		GridDimensions::GridWidthVal(), 
		GridDimensions::GridHeightVal(), 
		GridDimensions::IsHexMode() 
	);

	m_pGraphics = & m_D3d_driver;

	BaseAppWindow::Start( m_pMainGridWindow );
	m_pAppMenu->Initialize( m_hwndApp, m_pEvoWorkThreadInterface, & m_WinManager );

	m_pModelDataWork = m_EvoHistGlue.Start( m_pHistorySystem, TRUE ); 
	m_protocolServer.Start( m_pHistorySystem );

	pCoreWork          = m_pModelDataWork->GetEvolutionCore();
	m_pEvoCore4Display = EvolutionCore::CreateCore( );

	DefineCoreWrapperFunctions( pCoreWork );  // Core wrappers run in work thread
	m_pEvoReadBuffer->Initialize( pCoreWork, m_pEvoCore4Display );

	m_pEvoWorkThreadInterface->Start
	( 
		  m_hwndApp, 
		& m_ColorManager, 
		& m_atComputation,
		& m_eventPOI, 
		& m_Delay, 
		  m_pEvoReadBuffer, 
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

	m_pDspOptWindow   ->Start( m_hwndApp );
	m_pEvoEditorWindow->Start( m_hwndApp, m_pEvoWorkThreadInterface, m_pEvoReadBuffer, m_pDspOptWindow );
	m_pCrsrWindow     ->Start( m_hwndApp, m_pEvoReadBuffer, m_pFocusPoint );
	m_pStatistics     ->Start( m_hwndApp, m_pEvoReadBuffer );
	m_pPerfWindow     ->Start( m_hwndApp, m_Delay, m_atComputation, m_atDisplay, [&](){ return m_pEvoWorkThreadInterface->IsRunning(); } );

	m_WinManager.AddWindow( L"IDM_PERF_WINDOW", IDM_PERF_WINDOW, * m_pPerfWindow,      TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_CRSR_WINDOW", IDM_CRSR_WINDOW, * m_pCrsrWindow,      TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_STAT_WINDOW", IDM_STAT_WINDOW, * m_pStatistics,      TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_DISP_WINDOW", IDM_DISP_WINDOW, * m_pDspOptWindow,    TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, * m_pEvoEditorWindow, TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_MINI_WINDOW", IDM_MINI_WINDOW, * m_pMiniGridWindow,  TRUE, FALSE );
    m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainGridWindow,  TRUE, FALSE );

	configureStatusBar( );

	m_pMiniGridWindow->Size( );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_pEvoEditorWindow->Show( TRUE );

	(void)m_pMainGridWindow->SendMessage( WM_COMMAND, IDM_FIT_ZOOM, 0 );
//	Script::ProcessScript( L"std_script.in" );
}

void EvoAppWindow::Stop()
{
	m_pMiniGridWindow        ->Stop( );
	m_pMainGridWindow        ->Stop( );
	m_pEvoEditorWindow       ->Stop( );
	m_pDspOptWindow          ->Stop( );
	m_pPerfWindow            ->Stop( );
	m_pStatistics            ->Stop( );
	m_pCrsrWindow            ->Stop( );
	m_pEvoWorkThreadInterface->Stop( );

	m_pEvoReadBuffer->UnregisterAllObservers( );
	m_Delay          .UnregisterAllObservers( );

	m_EvoHistGlue.Stop( );

	m_WinManager.RemoveAll( );

	BaseAppWindow::Stop();

	delete m_pEvoCore4Display; 

	m_pModelDataWork   = nullptr;
	m_pGraphics		   = nullptr;  // Set in Start
	m_pEvoCore4Display = nullptr;
}

void EvoAppWindow::configureStatusBar( )
{
	int iPartScriptLine = 0;

	m_pGenerationDisplay = new GenerationDisplay( & m_StatusBar, m_pEvoReadBuffer, iPartScriptLine );

	m_StatusBar.NewPart( );
	m_StatusBar.AddButton( L"Show editor", (HMENU)IDM_EDIT_WINDOW, BS_PUSHBUTTON );

	m_StatusBar.NewPart( );
	ZoomControl::AddSizeControl
	( 
		& m_StatusBar, 
		LogarithmicTrackbar::Value2TrackbarD( MINIMUM_FIELD_SIZE.GetValue() ), 
		LogarithmicTrackbar::Value2TrackbarD( MAXIMUM_FIELD_SIZE.GetValue() ), 
		LogarithmicTrackbar::Value2TrackbarD( DEFAULT_FIELD_SIZE.GetValue() ) 
	);

	m_StatusBar.NewPart( );
	SpeedControl::Add
	( 
		& m_StatusBar, 
		m_pHistorySystem, 
		LogarithmicTrackbar::Value2TrackbarL( 0 ), 
		LogarithmicTrackbar::Value2TrackbarL( CastToUnsignedLong(MAX_DELAY.count()) ), 
		LogarithmicTrackbar::Value2TrackbarL( CastToUnsignedLong(DEFAULT_DELAY.count()) ) 
	);

	iPartScriptLine = m_StatusBar.NewPart( );
	m_ScriptHook.Initialize( & m_StatusBar, iPartScriptLine );
	m_StatusBar.DisplayInPart( iPartScriptLine, L"" );
	Script::ScrSetWrapHook( & m_ScriptHook );

	m_StatusBar.LastPart( );
}

void EvoAppWindow::ProcessAppCommand( WPARAM const wParam, LPARAM const lParam )
{
	int const wmId = LOWORD( wParam );

	if ( m_pEvoController->ProcessUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                                               // but do not concern the model

	if ( m_pEvoController->ProcessModelCommand( wmId, lParam ) )
		ProcessFrameworkCommand( wmId, lParam ); 
}
