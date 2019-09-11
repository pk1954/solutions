// win32_NNetSimuWindow.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "WinUser.h"

using namespace std::literals::chrono_literals;

// Model interfaces


// interfaces of various windows


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
    BaseWindow( ),
	m_hwndConsole( nullptr ),
    m_traceStream( ),
	m_bStarted( FALSE )
{
	Stopwatch stopwatch;

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

    // create window objects

	stopwatch.Start();

	stopwatch.Stop( L"create window objects" );

	m_NNetSimuController.Initialize
	( 
		this,
		& m_traceStream,
		& m_WinManager,
		& m_Delay, 
		& m_AppMenu
	);
};

NNetSimuWindow::~NNetSimuWindow( )
{
}

void NNetSimuWindow::Start( )
{
	m_AppMenu.Start( );

	m_WinManager.AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, m_hwndApp, TRUE,  TRUE  );

	m_AppMenu.Initialize( m_hwndApp, & m_WinManager );

	if ( ! m_WinManager.GetWindowConfiguration( ) )
	{
		std::wcout << L"Using default window positions" << std::endl;
		Show( TRUE );
	}

	m_bStarted = TRUE;
}

void NNetSimuWindow::Stop()
{
	m_bStarted = FALSE;

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

void NNetSimuWindow::adjustChildWindows( )
{
    static PIXEL const HIST_WINDOW_HEIGHT = 30_PIXEL;

    PixelRectSize pntAppClientSize( GetClRectSize( ) );
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

    if ( pntAppClientSize.IsNotZero( ) )
    {
    }
}
