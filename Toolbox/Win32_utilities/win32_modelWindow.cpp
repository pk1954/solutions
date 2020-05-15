// win32_modelWindow.cpp : 
//
// Win32_utilities

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_modelWindow.h"

LRESULT ModelWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
	switch (message)
	{

	case WM_COMMAND:
		if ( OnCommand( wParam, lParam ) )
			PostCommand2Application( wParam, lParam ); // not handled here, delegate to application
//		PostCommand2Application( IDM_REFRESH, 0 );
		return false;

	case WM_MOUSEMOVE:
		OnMouseMove( wParam, lParam );
		return false;

	case WM_LBUTTONDBLCLK:
		OnLeftButtonDblClick( wParam, lParam );
		return false;

	case WM_MOUSEWHEEL:
		OnMouseWheel( wParam, lParam );
		return false;

	case WM_LBUTTONDOWN:
		OnLButtonDown( wParam, lParam );
		return true;

	case WM_LBUTTONUP:
		OnLButtonUp( wParam, lParam );
		return false;

	case WM_RBUTTONUP:
		return OnRButtonUp( wParam, lParam );

	case WM_RBUTTONDOWN:
		return OnRButtonDown( wParam, lParam );

	case WM_SETCURSOR:
		OnSetCursor( wParam, lParam );
		return false;

	case WM_PAINT:
		m_pDisplayTimer->TimerStart( );
		OnPaint( );
		m_pDisplayTimer->TimerStop( );
		return false;

	case WM_SIZE:
		OnSize( wParam, lParam );
		return false;

	default:
		break;
	}

	return DefWindowProc( message, wParam, lParam );
}
