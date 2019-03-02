// win32_genDisplayWindow.cpp
//

#include "stdafx.h"
#include "strsafe.h"
#include "Windowsx.h"
#include "win32_baseWindow.h"
#include "win32_genDisplayWindow.h"

GenDisplayWindow::GenDisplayWindow( ) :
    BaseWindow( )
{
}

void GenDisplayWindow::StartGenDisplayWindow( HWND const hwndParent )
{
    HWND const hwnd = StartBaseWindow
    ( 
        hwndParent,
        CS_OWNDC,
        L"GenDisplayWindow",
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE,
		nullptr
    );
    Util::MakeLayered( hwnd, TRUE, 0, 100 );
};

void GenDisplayWindow::DisplayGenerationBelowCursor( HIST_GENERATION const gen )
{
    m_genDisplay = gen;
    Invalidate( FALSE );
}

LRESULT GenDisplayWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch ( message )
    {

    case WM_PAINT:
        if ( m_genDisplay.GetLong( ) >= 0 )
        {
            wchar_t szBuffer[ 30 ];
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint( &ps );
            (void)FillBackground( hDC, CLR_BACK );
            (void)StringCchPrintf( szBuffer, sizeof( szBuffer ) / sizeof( *szBuffer ), L"%u", m_genDisplay.GetLong( ) );
            (void)SetTextAlign( hDC, TA_CENTER );
            (void)SetTextColor( hDC, CLR_TEXT );
            (void)TextOut( hDC, 25, 0, szBuffer, (int)wcslen( szBuffer ) );
            (void)EndPaint( &ps );
        }
        return FALSE;

    default:
        break;
    }

    return DefWindowProc( message, wParam, lParam );
}

