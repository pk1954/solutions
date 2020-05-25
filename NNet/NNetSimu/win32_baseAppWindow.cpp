// win32_baseAppWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <chrono>
#include "trace.h"
#include "win32_util.h"
#include "Resource.h"
#include "ComputeThread.h"
#include "UtilityWrappers.h"
#include "win32_aboutBox.h"
#include "win32_modelWindow.h"
#include "win32_NNetAppMenu.h"
#include "win32_baseWindow.h"
#include "win32_winManager.h"
#include "win32_status.h"
#include "win32_baseAppWindow.h"

using namespace std::literals::chrono_literals;

void BaseAppWindow::Initialize( )
{
	//	_CrtSetAllocHook( MyAllocHook );

	m_hwndConsole = GetConsoleWindow( );
	BringWindowToTop( m_hwndConsole );

	DefineUtilityWrapperFunctions( );

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

void BaseAppWindow::Start
( 
	ModelWindow   * const pModelWindow, 
	ComputeThread * const pComputeThread
	)
{
	m_pModelWindow   = pModelWindow;
	m_pComputeThread = pComputeThread;

	m_pAppMenu->Start( );

	m_WinManager.AddWindow( L"IDM_CONS_WINDOW", IDM_CONS_WINDOW, m_hwndConsole,                 true,  true  );
	m_WinManager.AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, m_hwndApp,                     true,  true  );
	m_WinManager.AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_StatusBar.GetWindowHandle(), false, false );

	m_StatusBar.Show( true );
	pModelWindow->Show( true );

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
			true 
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
		if ( wParam == false )
			m_pAppMenu->AdjustVisibility( );
		break;

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
		OnClose( );
		return true;  

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		break;
	}

	return DefWindowProc( message, wParam, lParam );
}

bool BaseAppWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
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

	case IDM_FORWARD:
		m_pComputeThread->SingleStep( );
		break;

	case IDM_RUN:
		m_pComputeThread->RunComputation( );
		break;

	case IDM_STOP:
		m_pComputeThread->StopComputation( );
		break;

	default:
		return BaseWindow::OnCommand( wParam, lParam, pixPoint );
	}

	return true;  // command has been processed
}
