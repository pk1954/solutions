// win32_textWindow.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include "win32_util.h"
#include "win32_textWindow.h"

using namespace std;

//lint -sem(TextWindow::setupTextMetrics,initializer)

TextWindow::TextWindow( ) :  
    BaseWindow( )
{
        // setup text metrics

    TEXTMETRIC textMetric;
    HDC const hDC = GetDC( nullptr );   assert( hDC != nullptr );
    (void)GetTextMetrics( hDC, &textMetric );
    m_cxChar         = textMetric.tmAveCharWidth;
    m_cyChar         = textMetric.tmHeight + textMetric.tmExternalLeading;
    m_iHorizontalPos = 0;
    m_iVerticalPos   = 0;
    m_iHorRaster     = 3 * (textMetric.tmPitchAndFamily & 1 ? 3 : 2) * m_cxChar;
    ReleaseDC( nullptr, hDC );
}

TextWindow::~TextWindow( )
{
}

HWND TextWindow::StartTextWindow
(
    HWND    const hWndParent,
    LPCTSTR const szClass,
    UINT    const uiAlpha
)
{
    HWND const hWnd = StartBaseWindow
    ( 
        hWndParent,
        CS_OWNDC | CS_DBLCLKS | CS_DROPSHADOW,
        szClass,
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CAPTION
    );
    Util::MakeLayered( hWnd, TRUE, 0, uiAlpha );
    SetWindowText( hWnd, szClass );
    return hWnd;
}

LRESULT TextWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        m_hDC = BeginPaint( &ps );
        startPainting( );
        DoPaint( );
        (void)EndPaint( &ps );
        return FALSE;
    }

    case WM_CLOSE:                           // Do not destroy, just hide  
        Show( tBoolOp::opToggle );
        return 1;

    default:
        break;
    }
    
    return DefWindowProc( message, wParam, lParam );
}


void TextWindow::printString( wchar_t const * const data )
{
    (void)TextOut( m_hDC, m_iHorizontalPos, m_iVerticalPos, data, (int)wcslen( data ) );
    m_iHorizontalPos += m_iHorRaster;
}

void TextWindow::printNumber( unsigned int data )
{
    (void)StringCchPrintf( m_szBuffer, sizeof( m_szBuffer ) / sizeof( *m_szBuffer ), L" %5u", data );
    printString( m_szBuffer );
}

void TextWindow::printPercentage( unsigned int data )
{
    (void)StringCchPrintf( m_szBuffer, sizeof( m_szBuffer ) / sizeof( *m_szBuffer ), L" %5u%%", data );
    printString( m_szBuffer );
}

void TextWindow::printPercentage( unsigned int val, unsigned int max )
{
    int const iPercentage = ( max == 0 ) ? 0 : ( val * 100 ) / max;
    (void)StringCchPrintf( m_szBuffer, sizeof( m_szBuffer ) / sizeof( *m_szBuffer ), L" %5u%% (%d of %d)", iPercentage, val, max );
    printString( m_szBuffer );
}

void TextWindow::printSpan( unsigned int min, unsigned int max )
{
    (void)StringCchPrintf( m_szBuffer, sizeof( m_szBuffer ) / sizeof( *m_szBuffer ), L" (%ld - %ld)", min, max );
    printString( m_szBuffer );
}

void TextWindow::printFloat( float data )
{
    (void)StringCchPrintf( m_szBuffer, sizeof( m_szBuffer ) / sizeof( *m_szBuffer ), L" %3.1f%%", data );
    printString( m_szBuffer );
}

void TextWindow::printAsDecValue( DWORD dwValue )
{
    DWORD const intPlaces = dwValue / 1000;
    DWORD const decPlaces = dwValue - 1000 * intPlaces;
    (void)StringCchPrintf( m_szBuffer, sizeof( m_szBuffer ) / sizeof( *m_szBuffer ), L"%d.%d", intPlaces, decPlaces / 100 );
    printString( m_szBuffer );
}
