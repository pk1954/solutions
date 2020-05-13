// win32_baseAppWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <chrono>
#include "trace.h"
#include "win32_util.h"
#include "Resource.h"
#include "UtilityWrappers.h"
#include "win32_aboutBox.h"
#include "win32_modelWindow.h"
#include "win32_appMenu.h"
#include "win32_baseWindow.h"
#include "win32_winManager.h"
#include "win32_status.h"
#include "win32_WorkThreadInterface.h"
#include "win32_baseAppWindow.h"

using namespace std::literals::chrono_literals;

void BaseAppWindow::Initialize( WorkThreadInterface * const pWorkThreadInterface )
{
	//	_CrtSetAllocHook( MyAllocHook );

	m_pWorkThreadInterface = pWorkThreadInterface;

	m_hwndConsole = GetConsoleWindow( );
	BringWindowToTop( m_hwndConsole );

	DefineUtilityWrapperFunctions( );

	m_traceStream = OpenTraceFile( L"main_trace.out" );

	m_pWorkThreadInterface->Initialize( & m_traceStream );

	m_StatusBar.SetRefreshRate( 300ms );

	m_hwndApp = StartBaseWindow
	( 
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
}

void BaseAppWindow::Start( ModelWindow * const pModelWindow )
{
	m_pModelWindow = pModelWindow;

	m_pAppMenu->Start( );
	m_StatusBar.Start( m_hwndApp, m_pWorkThreadInterface );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW", IDM_CONS_WINDOW, m_hwndConsole,                 TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, m_hwndApp,                     TRUE,  TRUE  );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_StatusBar.GetWindowHandle(), FALSE, FALSE );

	m_StatusBar.Show( TRUE );
	pModelWindow->Show( TRUE );

	adjustChildWindows( );
}

void BaseAppWindow::Stop( )
{
	m_StatusBar.Stop( );
	m_pAppMenu->Stop( );
}

void BaseAppWindow::adjustChildWindows( )
{
	HWND hwndApp = GetParent( m_pModelWindow->GetWindowHandle() );

	PixelRectSize pntAppClientSize( Util::GetClRectSize( hwndApp ) );
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

	if ( pntAppClientSize.IsNotZero( ) )
	{
		m_StatusBar.Resize( );
		pixAppClientWinHeight -= m_StatusBar.GetHeight( );

		m_pModelWindow->Move  // use all available space for model window
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
		return ProcessAppCommand( wParam, lParam );

	case WM_PAINT:
	{
		static COLORREF const CLR_GREY = RGB( 128, 128, 128 );
		PAINTSTRUCT   ps;
		HDC           hDC = BeginPaint( &ps );
		FillBackground( hDC, CLR_GREY );
		(void)EndPaint( &ps );
		return false;
	}

	case WM_SIZE:
	case WM_MOVE:
		adjustChildWindows( );
		break;

	case WM_CLOSE:
		ProcessCloseMessage( );
		return true;  

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		break;
	}

	return DefWindowProc( message, wParam, lParam );
}

bool BaseAppWindow::ProcessFrameworkCommand( WPARAM const wParam, LPARAM const lParam )
{
	int const wmId = LOWORD( wParam );

	switch (wmId)
	{
	case IDM_ABOUT:
		ShowAboutBox( GetWindowHandle( ) );
		break;

	case IDM_EXIT:
		PostMessage( WM_CLOSE, 0, 0 );
		break;

	case IDM_MAIN_WINDOW:
		::SendMessage( m_WinManager.GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
		break;

	case IDM_FORWARD:
		m_pWorkThreadInterface->PostGenerationStep( );
		break;

	case IDM_RUN:
		m_pWorkThreadInterface->PostRunGenerations( true );
		break;

	case IDM_STOP:
		m_pWorkThreadInterface->PostStopComputation( );
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}
