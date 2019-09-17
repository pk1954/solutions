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
    BaseAppWindow( ),
	m_pMainNNetWindow( nullptr ),
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

    // create window objects

	stopwatch.Start();

	m_NNetWorkThreadInterface.Initialize( & m_traceStream ); 

	stopwatch.Stop( L"create window objects" );

	NNetWindow::InitClass( & m_NNetWorkThreadInterface, & m_atDisplay );

	m_pMainNNetWindow = new NNetWindow( );

	m_NNetSimuController.Initialize
	( 
		this,
		& m_NNetWorkThreadInterface,
		& m_WinManager,
		& m_Delay, 
		& m_AppMenu
	);

	m_pMainNNetWindow->SetRefreshRate( 100ms );
};

NNetSimuWindow::~NNetSimuWindow( )
{
	delete m_pMainNNetWindow;
}

void NNetSimuWindow::Start( )
{
	NNetModel * pModelWork;

	m_AppMenu.Start( );

/////////////////////


///	m_pHistInfoWindow->SetHistorySystem( m_pHistorySystem );

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

	BaseAppWindow::Start( m_hwndApp, & m_NNetWorkThreadInterface );

	m_WinManager.AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW,   m_hwndApp,         TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, * m_pMainNNetWindow, TRUE,  FALSE );

	m_AppMenu.Initialize( m_hwndApp, & m_WinManager );

	AdjustChildWindows( m_pMainNNetWindow );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_pMainNNetWindow->Show( TRUE );

	m_bStarted = TRUE;
}

void NNetSimuWindow::Stop()
{
	BaseAppWindow::Stop();

	m_bStarted = FALSE;

	m_pMainNNetWindow->Stop( );

	m_AppMenu.Stop( );
	m_Delay  .Stop( );

	m_WinManager.RemoveAll( );
}

LRESULT NNetSimuWindow::UserProc
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
		m_NNetSimuController.ProcessCommand( wParam, lParam );
	    return FALSE;

    case WM_SIZE:
	case WM_MOVE:
		AdjustChildWindows( m_pMainNNetWindow );
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
