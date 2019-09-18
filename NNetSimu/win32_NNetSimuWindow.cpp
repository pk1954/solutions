// win32_NNetSimuWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "WinUser.h"

using namespace std::literals::chrono_literals;

// Model interfaces

#include "NNetModel.h"

// interfaces of various windows

#include "win32_NNetWindow.h"
#include "win32_histWindow.h"
#include "win32_status.h"

// infrastructure

#include "util.h"
#include "ObserverInterface.h"

// scripting and tracing

#include "trace.h"
#include "UtilityWrappers.h"
#include "win32_stopwatch.h"

// system and resources

#include "resource.h"

// application

#include "win32_NNetSimuWindow.h"

NNetSimuWindow::NNetSimuWindow( ) :
    BaseAppWindow( & m_NNetWorkThreadInterface ),
	m_pMainNNetWindow( nullptr ),
	m_traceStream( )
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

    // create window objects

	stopwatch.Start();

	m_NNetWorkThreadInterface.Initialize( & m_traceStream ); 

	stopwatch.Stop( L"create window objects" );

	NNetWindow::InitClass( & m_NNetWorkThreadInterface, & m_atDisplay );

	m_pAppMenu        = new NNetSimuMenu( );
	m_pMainNNetWindow = new NNetWindow( );

	m_NNetSimuController.Initialize
	( 
		this,
		& m_NNetWorkThreadInterface,
		& m_WinManager,
		& m_Delay
	);

	m_pMainNNetWindow->SetRefreshRate( 100ms );
};

NNetSimuWindow::~NNetSimuWindow( )
{
	delete m_pMainNNetWindow;
	delete m_pAppMenu;
}

void NNetSimuWindow::Start( )
{
	NNetModel * pModelWork;

	BaseAppWindow::Start( m_pMainNNetWindow, m_hwndApp, & m_NNetSimuController );
	m_pAppMenu->Initialize
	( 
		m_hwndApp, 
		nullptr,   //TODO
		& m_WinManager 
	);

	m_pModelDataWork     = m_NNetHistGlue.Start( m_pHistorySystem, TRUE ); 
	pModelWork           = m_pModelDataWork->GetNNetModel();
	m_pNNetModel4Display = NNetModel::CreateCore( );

	m_NNetReadBuffer.Initialize( pModelWork, m_pNNetModel4Display );

	m_pMainNNetWindow->Start
	( 
		m_hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS, 
		[&]() { return true; }	
	);
		
	m_NNetWorkThreadInterface.Start
	( 
		m_hwndApp, 
		& m_atComputation,
		& m_event, 
		& m_Delay, 
		& m_NNetReadBuffer, 
		& m_NNetHistGlue
	);

	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainNNetWindow, TRUE,  FALSE );

	AdjustChildWindows( );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}
}

void NNetSimuWindow::Stop()
{
	BaseAppWindow::Stop();

	m_pMainNNetWindow->Stop( );

	m_Delay.Stop( );

	m_WinManager.RemoveAll( );
}
