// win32_modelWindow.cpp : 
//
// Win32_appFramework

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_modelWindow.h"

ActionTimer * ModelWindow::m_pDisplayTimer = nullptr;
HCURSOR       ModelWindow::m_hCrsrArrow    = nullptr;
HCURSOR       ModelWindow::m_hCrsrMove     = nullptr;

LRESULT ModelWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
	switch (message)
	{

	case WM_COMMAND:
		if ( OnCommand( wParam, lParam ) )
			PostCommand2Application( wParam, lParam ); // not handled here, delegate to application
		PostCommand2Application( IDM_REFRESH, 0 );
		return FALSE;

	case WM_MOUSEMOVE:
		OnMouseMove( wParam, lParam );
		return FALSE;

	case WM_MOUSEWHEEL:
		OnMouseWheel( wParam, lParam );
		return FALSE;

	case WM_LBUTTONDOWN:
		OnLButtonDown( wParam, lParam );
		return TRUE;

	case WM_LBUTTONUP:
		OnLButtonUp( wParam, lParam );
		return FALSE;

	case WM_SETCURSOR:
		OnSetCursor( wParam, lParam );
		return FALSE;

	case WM_PAINT:
		m_pDisplayTimer->TimerStart( );
		OnPaint( );
		m_pDisplayTimer->TimerStop( );
		return FALSE;

	default:
		break;
	}

	return DefWindowProc( message, wParam, lParam );
}
