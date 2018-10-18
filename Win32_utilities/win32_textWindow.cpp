// win32_textWindow.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "win32_util.h"
#include "win32_thread.h"
#include "win32_textWindowThread.h"
#include "win32_textWindow.h"

using namespace std;


TextWindow::TextWindow( ) :  
    BaseWindow( ),
	m_hDC_Memory( 0 ),
	m_hBitmap( 0 )
{ }

TextWindow::~TextWindow()
{
	DeleteObject( m_hBitmap );
	DeleteDC( m_hDC_Memory );
}

void TextWindow::StartTextWindow
(
    HWND    const   hwndParent,
	int		const	xPos, 
	int		const	yPos, 
	int		const	width, 
	int		const	height,    
	LPCTSTR const   szClass,
    UINT    const   uiAlpha,
	BOOL    const   bAsync
)
{
    HWND const hwnd = StartBaseWindow
    ( 
        hwndParent,
		xPos, yPos, width, height,
        CS_OWNDC | CS_DBLCLKS,
        szClass,
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CAPTION
    );

	PixelRectSize rectSize = GetClRectSize( );
	HDC     const hDC      = GetDC( hwnd );   assert( hDC != nullptr );
	m_hDC_Memory = CreateCompatibleDC( hDC );
	m_hBitmap    = CreateCompatibleBitmap( hDC, rectSize.GetWidth(), rectSize.GetHeight() );
	SelectObject( m_hDC_Memory, m_hBitmap );
	ReleaseDC( hwnd, hDC );
	Util::MakeLayered( hwnd, TRUE, 0, uiAlpha );
    SetWindowText( hwnd, szClass );
	m_pTextWindowThread = new TextWindowThread( m_hDC_Memory, width, height, this, szClass, bAsync );
}

void TextWindow::Trigger( )
{
	m_pTextWindowThread->Trigger( );
}

LRESULT TextWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT   ps;
        HDC           hDC      = BeginPaint( &ps );
		PixelRectSize rectSize = GetClRectSize( );
		BitBlt( hDC, 0, 0, rectSize.GetWidth(), rectSize.GetHeight(), m_hDC_Memory, 0, 0, SRCCOPY );
        (void)EndPaint( &ps );
        return FALSE;
    }

    case WM_CLOSE:    // Do not destroy, just hide  
        Show( FALSE );
        return TRUE;

    default:
        break;
    }
    
    return DefWindowProc( message, wParam, lParam );
}
