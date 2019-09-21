// win32_NNetAppWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "WinUser.h"

using namespace std::literals::chrono_literals;

// Model interfaces

#include "NNetCoord.h"
#include "NNetModel.h"

// interfaces of various windows

#include "win32_NNetEditor.h"
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

#include "win32_NNetAppWindow.h"

NNetAppWindow::NNetAppWindow( ) :
	m_pMainNNetWindow( nullptr ),
	m_pModelDataWork( nullptr ),
	m_pNNetModel4Display( nullptr )
{
	Stopwatch stopwatch;

	Initialize( & m_NNetWorkThreadInterface ),
		
	NNetWindow::InitClass( & m_NNetWorkThreadInterface, & m_atDisplay );

	m_pAppMenu          = new NNetAppMenu( );
	m_pMainNNetWindow   = new NNetWindow( );
	m_pNNetEditorWindow = new NNetEditorWindow( );

	m_pNNetController = new NNetController
	( 
		& m_WinManager,
		  m_pMainNNetWindow,
		  m_pNNetEditorWindow
	);

	m_pNNetController->Initialize
	( 
		this,
		& m_NNetWorkThreadInterface,
		& m_Delay
	);

	//DefineWin32HistWrapperFunctions( & m_NNetWorkThreadInterface );
	//DefineWin32WrapperFunctions    ( & m_NNetWorkThreadInterface );
	//DefineWin32EditorWrapperFunctions( m_pNNetEditorWindow );

	m_pMainNNetWindow  ->SetRefreshRate( 100ms );
	m_pNNetEditorWindow->SetRefreshRate( 300ms );
};

NNetAppWindow::~NNetAppWindow( )
{
	delete m_pNNetEditorWindow;
	delete m_pMainNNetWindow;
	delete m_pAppMenu;
}

void NNetAppWindow::Start( )
{
	NNetModel * pModelWork;

	BaseAppWindow::Start( m_pMainNNetWindow );
	m_pAppMenu->Initialize
	( 
		m_hwndApp, 
		& m_NNetWorkThreadInterface, 
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
		DEFAULT_PIXEL_SIZE,
		[&]() { return true; }	
	);
		
	m_NNetWorkThreadInterface.Start
	( 
		m_hwndApp, 
		& m_atComputation,
		& m_eventPOI, 
		& m_Delay, 
		& m_NNetReadBuffer, 
		& m_NNetHistGlue
	);

	m_pNNetEditorWindow->Start( m_hwndApp, & m_NNetWorkThreadInterface, & m_NNetReadBuffer );

	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainNNetWindow,   TRUE, FALSE );
	m_WinManager.AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, * m_pNNetEditorWindow, TRUE, FALSE );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_pNNetEditorWindow->Show( TRUE );
}

void NNetAppWindow::Stop()
{
	BaseAppWindow::Stop();

	m_pMainNNetWindow  ->Stop( );
	m_pNNetEditorWindow->Stop( );

	m_Delay.Stop( );

	m_WinManager.RemoveAll( );
}

void NNetAppWindow::ProcessAppCommand( WPARAM const wParam, LPARAM const lParam )
{
	int const wmId = LOWORD( wParam );

	if ( m_pNNetController->ProcessUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                                                    // but do not concern the model

	if ( m_pNNetController->ProcessModelCommand( wmId, lParam ) )
		ProcessFrameworkCommand( wmId, lParam );                   // Some commands are handled by the framework controller
}
