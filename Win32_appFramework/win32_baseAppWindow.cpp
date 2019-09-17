// win32_baseAppWindow.cpp
//
// Win32_appFramework


#include "stdafx.h"
#include <chrono>
#include "HistorySystem.h"
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_modelWindow.h"
#include "win32_histWindow.h"
#include "win32_status.h"
#include "win32_baseAppWindow.h"

using namespace std::literals::chrono_literals;

BaseAppWindow::BaseAppWindow( ) : 
	  m_hwndConsole( nullptr ),
	  m_pStatusBar( nullptr ),
      m_pHistWindow( nullptr ),
      m_pHistorySystem( nullptr )

{
	m_hwndConsole = GetConsoleWindow( );
	SetWindowPos( m_hwndConsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	m_pStatusBar  = new StatusBar( );
	m_pHistWindow = new HistWindow( );

	m_pStatusBar ->SetRefreshRate( 300ms );
	m_pHistWindow->SetRefreshRate( 200ms ); 
}

BaseAppWindow::~BaseAppWindow() 
{
	delete m_pStatusBar;     
	delete m_pHistWindow;
};

void BaseAppWindow::Start
( 
	HWND                  const hwndParent,
	WorkThreadInterface * const pWorkThreadInterface
)
{
	m_pHistorySystem = HistorySystem::CreateHistorySystem( );  // deleted in Stop function

	m_pHistWindow->Start( hwndParent, m_pHistorySystem, pWorkThreadInterface );
	m_pStatusBar ->Start( hwndParent, m_pHistorySystem, pWorkThreadInterface );

	m_WinManager.AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,    m_pStatusBar->GetWindowHandle(), FALSE, FALSE );
	m_WinManager.AddWindow( L"IDM_HIST_WINDOW", IDM_HIST_WINDOW, * m_pHistWindow,                   FALSE, FALSE ); 
}

void BaseAppWindow::Stop( )
{
	m_pHistWindow->Stop( );

	delete m_pHistorySystem;   

	m_pHistorySystem   = nullptr;
}

void BaseAppWindow::AdjustChildWindows(	ModelWindow * const pModelWindow )
{
	static PIXEL const HIST_WINDOW_HEIGHT = 30_PIXEL;

	HWND hwndApp = GetParent( pModelWindow->GetWindowHandle() );

	PixelRectSize pntAppClientSize( Util::GetClRectSize( hwndApp ) );
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

	if ( pntAppClientSize.IsNotZero( ) )
	{
		m_pStatusBar->Resize( );
		pixAppClientWinHeight -= m_pStatusBar->GetHeight( );
		pixAppClientWinHeight -= HIST_WINDOW_HEIGHT, 
		m_pHistWindow->Move   // adapt history window to new size
		( 
			0_PIXEL, 
			pixAppClientWinHeight, 
			pixAppClientWinWidth, 
			HIST_WINDOW_HEIGHT, 
			TRUE 
		); 
		pModelWindow->Move
		( 
			0_PIXEL, 
			0_PIXEL, 
			pixAppClientWinWidth, 
			pixAppClientWinHeight, 
			TRUE 
		);
	}
}

