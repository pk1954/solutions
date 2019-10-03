// win32_NNetAppWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "WinUser.h"

// Model interfaces

#include "MoreTypes.h"
#include "NNetModel.h"

// interfaces of various windows

#include "win32_NNetEditor.h"
#include "win32_NNetWindow.h"
#include "win32_status.h"
#include "win32_speedControl.h"
#include "win32_zoomControl.h"
#include "SpeedDisplay.h"
#include "TimeDisplay.h"

// infrastructure

#include "util.h"
#include "ObserverInterface.h"

// scripting and tracing

#include "trace.h"
#include "script.h"
#include "UtilityWrappers.h"
#include "win32_stopwatch.h"

// system and resources

#include "Resource.h"
#include "d3d_system.h"

// application

#include "win32_NNetAppWindow.h"

using namespace std::literals::chrono_literals;

NNetAppWindow::NNetAppWindow( ) :
	m_pMainNNetWindow( nullptr ),
	m_pModelDataWork( nullptr ),
	m_pGraphics( nullptr ),
	m_pSpeedDisplay( nullptr ),
	m_pTimeDisplay( nullptr ),
	m_pNNetModel4Display( nullptr )
{
	Stopwatch stopwatch;

	BaseAppWindow::Initialize( & m_NNetWorkThreadInterface, FALSE ),
		
	NNetWindow::InitClass
	( 
		& m_NNetReadBuffer,
		& m_NNetWorkThreadInterface, 
		& m_atDisplay 
	);

	m_pAppMenu          = new NNetAppMenu( );
	m_pMainNNetWindow   = new NNetWindow( );
	m_pNNetEditorWindow = new NNetEditorWindow( );

	m_pNNetController = new NNetController
	( 
		this,
		  m_pMainNNetWindow,
		  m_pNNetEditorWindow,
		& m_WinManager,
		& m_StatusBar, 
		& m_NNetWorkThreadInterface,
		& m_SlowMotionRatio
	);

	m_pMainNNetWindow  ->SetRefreshRate( 100ms );
	m_pNNetEditorWindow->SetRefreshRate( 300ms );
};

NNetAppWindow::~NNetAppWindow( )
{
	delete m_pNNetEditorWindow;
	delete m_pMainNNetWindow;
	delete m_pAppMenu;
	delete m_pTimeDisplay;
	delete m_pSpeedDisplay;
}

void NNetAppWindow::Start( )
{
	m_D3d_driver.Initialize
	( 
		m_hwndApp, 
		200, 
		100, 
		FALSE 
	);

	m_pGraphics = & m_D3d_driver;

	BaseAppWindow::Start( m_pMainNNetWindow );
	m_pAppMenu->Initialize
	( 
		m_hwndApp, 
		& m_NNetWorkThreadInterface, 
		& m_WinManager 
	);

	m_pModelDataWork     = NNetModel::CreateModel( );
	m_pNNetModel4Display = NNetModel::CreateModel( );

	m_NNetReadBuffer.Initialize( m_pModelDataWork, m_pNNetModel4Display );

	m_pMainNNetWindow->Start
	( 
		m_hwndApp, 
		m_pGraphics, 
		WS_CHILD | WS_CLIPSIBLINGS,
		DEFAULT_PIXEL_SIZE,
		nullptr  // no visibility criterion. Allways visible
	);
		
	m_NNetWorkThreadInterface.Start
	( 
		m_hwndApp, 
		& m_atComputation,
		& m_eventPOI, 
		& m_NNetReadBuffer,
		& m_SlowMotionRatio,
		m_pModelDataWork
	);

//	m_pNNetEditorWindow->Start( m_hwndApp, & m_NNetWorkThreadInterface, & m_NNetReadBuffer );

	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainNNetWindow,   TRUE, FALSE );
//	m_WinManager.AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, * m_pNNetEditorWindow, TRUE, FALSE );

	configureStatusBar( );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

//	m_pNNetEditorWindow->Show( TRUE );
}

void NNetAppWindow::Stop()
{
	m_pMainNNetWindow  ->Stop( );
//	m_pNNetEditorWindow->Stop( );

	m_NNetReadBuffer.UnregisterAllObservers( );
	m_NNetWorkThreadInterface.Stop( );

	BaseAppWindow::Stop();

	NNetModel::DestroyModel( m_pModelDataWork );
	NNetModel::DestroyModel( m_pNNetModel4Display );

	m_WinManager.RemoveAll( );
}

void NNetAppWindow::configureStatusBar( )
{
	int iPartScriptLine = 0;
	m_pTimeDisplay = new TimeDisplay( & m_StatusBar, & m_NNetReadBuffer, iPartScriptLine );

	//iPartScriptLine = m_StatusBar.NewPart( );
	//m_StatusBar.AddButton( L"Show editor", (HMENU)IDM_EDIT_WINDOW, BS_PUSHBUTTON );

	iPartScriptLine = m_StatusBar.NewPart( );
	ZoomControl::AddSizeControl
	( 
		& m_StatusBar, 
		LogarithmicTrackbar::Value2TrackbarD( MINIMUM_PIXEL_SIZE.GetValue() ), 
		LogarithmicTrackbar::Value2TrackbarD( MAXIMUM_PIXEL_SIZE.GetValue() ), 
		LogarithmicTrackbar::Value2TrackbarD( DEFAULT_PIXEL_SIZE.GetValue() ) 
	);
	EnableWindow( m_StatusBar.GetDlgItem( IDM_FIT_ZOOM ), FALSE );

	iPartScriptLine = m_StatusBar.NewPart( );
	SpeedControl::AddSimulationControl
	( 
		& m_StatusBar, 
		m_pHistorySystem, 
		LogarithmicTrackbar::Value2TrackbarL( CastToLong( SlowMotionRatio::MIN) ), 
		LogarithmicTrackbar::Value2TrackbarL( CastToLong( SlowMotionRatio::MAX) ), 
		LogarithmicTrackbar::Value2TrackbarL( CastToLong( SlowMotionRatio::DEFAULT ) ) 
	);

	iPartScriptLine = m_StatusBar.NewPart( );
	m_pSpeedDisplay = new SpeedDisplay( & m_StatusBar, & m_SlowMotionRatio, iPartScriptLine );

	iPartScriptLine = m_StatusBar.NewPart( );
	m_ScriptHook.Initialize( & m_StatusBar, iPartScriptLine );
	m_StatusBar.DisplayInPart( iPartScriptLine, L"" );
	Script::ScrSetWrapHook( & m_ScriptHook );

	m_StatusBar.LastPart( );
	m_pTimeDisplay->Notify( true );
	m_pSpeedDisplay->Notify( true );
}

void NNetAppWindow::ProcessAppCommand( WPARAM const wParam, LPARAM const lParam )
{
	int const wmId = LOWORD( wParam );

	if ( m_pNNetController->ProcessUIcommand( wmId, lParam ) )     // handle all commands that affect the UI
		return;                                                    // but do not concern the model

	if ( m_pNNetController->ProcessModelCommand( wmId, lParam ) )
		ProcessFrameworkCommand( wmId, lParam );                   // Some commands are handled by the framework controller
}
