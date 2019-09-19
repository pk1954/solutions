// win32_baseAppWindow.cpp
//
// Win32_appFramework


#include "stdafx.h"
#include <chrono>
#include "trace.h"
#include "HistorySystem.h"
#include "win32_util.h"
#include "win32_util_resource.h"
#include "UtilityWrappers.h"
#include "win32_modelWindow.h"
#include "win32_controller.h"
#include "win32_appMenu.h"
#include "win32_baseAppWindow.h"

using namespace std::literals::chrono_literals;

BaseAppWindow::BaseAppWindow
( 	
	WorkThreadInterface * const pWorkThreadInterface
) : 
	m_bStarted( FALSE ),
	m_hwndConsole( nullptr ),
	m_pController( nullptr ),
	m_pAppMenu( nullptr ),
	m_pModelWindow( nullptr ),
	m_pHistorySystem( nullptr ),
	m_pWorkThreadInterface( pWorkThreadInterface ),
	m_traceStream( )
{
	//	_CrtSetAllocHook( MyAllocHook );

	m_hwndConsole = GetConsoleWindow( );
	SetWindowPos( m_hwndConsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	m_StatusBar     .SetRefreshRate( 300ms );
	m_HistWindow    .SetRefreshRate( 200ms ); 
	m_HistInfoWindow.SetRefreshRate( 300ms );

	m_hwndApp = StartBaseWindow
	( 
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);

	DefineUtilityWrapperFunctions( );

	m_traceStream = OpenTraceFile( L"main_trace.out" );
}

BaseAppWindow::~BaseAppWindow() 
{
};

void BaseAppWindow::Start
( 
	ModelWindow * const pModelWindow, 
	HWND          const hwndParent,
	Controller  * const pController
)
{
	m_pModelWindow = pModelWindow;
	m_pController  = pController;

	m_pHistorySystem = HistorySystem::CreateHistorySystem( );  // deleted in Stop function

	m_pAppMenu->Start( );
	m_HistWindow    .Start( hwndParent, m_pHistorySystem, m_pWorkThreadInterface );
	m_StatusBar     .Start( hwndParent, m_pHistorySystem, m_pWorkThreadInterface );
	m_HistInfoWindow.Start( hwndParent, nullptr );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW", IDM_CONS_WINDOW, m_hwndConsole,                   TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, hwndParent,                      TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_StatusBar.GetWindowHandle(), FALSE, FALSE );
	m_WinManager.AddWindow( L"IDM_HIST_WINDOW", IDM_HIST_WINDOW, m_HistWindow,                    FALSE, FALSE ); 
	m_WinManager.AddWindow( L"IDM_HIST_INFO",   IDM_HIST_INFO,   m_HistInfoWindow,                TRUE,  FALSE );

	m_HistInfoWindow.SetHistorySystem( m_pHistorySystem );

	m_StatusBar.Show( TRUE );
	pModelWindow->Show( TRUE );

	AdjustChildWindows( );

	m_bStarted = TRUE;
}

void BaseAppWindow::Stop( )
{
	m_bStarted = FALSE;

	m_HistInfoWindow.Stop( );
	m_HistWindow    .Stop( );
	m_StatusBar     .Stop( );
	m_pAppMenu->Stop( );

	delete m_pHistorySystem;   

	m_pHistorySystem = nullptr;
}

void BaseAppWindow::AdjustChildWindows( )
{
	static PIXEL const HIST_WINDOW_HEIGHT = 30_PIXEL;

	HWND hwndApp = GetParent( m_pModelWindow->GetWindowHandle() );

	PixelRectSize pntAppClientSize( Util::GetClRectSize( hwndApp ) );
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

	if ( pntAppClientSize.IsNotZero( ) )
	{
		m_StatusBar.Resize( );
		pixAppClientWinHeight -= m_StatusBar.GetHeight( );
		pixAppClientWinHeight -= HIST_WINDOW_HEIGHT, 
		m_HistWindow.Move   // adapt history window to new size
		( 
			0_PIXEL, 
			pixAppClientWinHeight, 
			pixAppClientWinWidth, 
			HIST_WINDOW_HEIGHT, 
			TRUE 
		); 
		m_pModelWindow->Move
		( 
			0_PIXEL, 
			0_PIXEL, 
			pixAppClientWinWidth, 
			pixAppClientWinHeight, 
			TRUE 
		);
	}
}

LRESULT BaseAppWindow::UserProc
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
			m_pAppMenu->AdjustVisibility( );
		break;

	case WM_COMMAND:
		m_pController->ProcessAppCommand( wParam, lParam );
		return FALSE;

	case WM_PAINT:
	{
		static COLORREF const CLR_GREY = RGB( 128, 128, 128 );
		PAINTSTRUCT   ps;
		HDC           hDC = BeginPaint( &ps );
		FillBackground( hDC, CLR_GREY );
		(void)EndPaint( &ps );
		return FALSE;
	}

	case WM_SIZE:
	case WM_MOVE:
		AdjustChildWindows( );
		break;

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
