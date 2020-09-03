// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "win32_monitorWindow.h"

void MonitorWindow::Start
( 
	HWND const hwndApp
)
{
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 
		L"ClassMonitorWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
	m_context.Start( hwnd );
	SetWindowText( hwnd, L"Monitor" );
}

void MonitorWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

void MonitorWindow::doPaint( )
{
	// draw signal
}

void MonitorWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_context.StartFrame( hDC ) )
		{
			doPaint( );
			m_context.EndFrame( );
		}
		EndPaint( &ps );
	}
}