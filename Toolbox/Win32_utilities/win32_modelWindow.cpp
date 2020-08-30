// win32_modelWindow.cpp : 
//
// Win32_utilities

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_modelWindow.h"

bool ModelWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
	switch (message)
	{

	case WM_MOUSEMOVE:
		OnMouseMove( wParam, lParam );
		return true;

	case WM_LBUTTONDBLCLK:
		OnLeftButtonDblClick( wParam, lParam );
		return true;

	case WM_MOUSEWHEEL:
		OnMouseWheel( wParam, lParam );
		return true;

	case WM_LBUTTONDOWN:
		OnLButtonDown( wParam, lParam );
		return false;

	case WM_LBUTTONUP:
		OnLButtonUp( wParam, lParam );
		return true;

	case WM_RBUTTONDOWN:
		OnRButtonDown( wParam, lParam );
		break;

	case WM_RBUTTONUP:
		if ( OnRButtonUp( wParam, lParam ) )
			return true;   // message completely handled, do not pass over to default processing
		break;

	case WM_SETCURSOR:
		OnSetCursor( wParam, lParam );
		return true;

	case WM_PAINT:
		m_pDisplayTimer->TimerStart( );
		OnPaint( );
		m_pDisplayTimer->TimerStop( );
		return true;

	default:
		break;
	}

	return BaseWindow::UserProc( message, wParam, lParam );
}
