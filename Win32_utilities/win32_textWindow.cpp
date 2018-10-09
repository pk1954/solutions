// win32_textWindow.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
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
    m_iHorRaster     = 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2) * m_cxChar;
    ReleaseDC( nullptr, hDC );
    m_wBuffer.imbue(std::locale(""));
}

HWND TextWindow::StartTextWindow
(
    HWND    const hWndParent,
    LPCTSTR const szClass,
    UINT    const uiAlpha
)
{
    HWND const hwnd = StartBaseWindow
    ( 
        hWndParent,
        CS_OWNDC | CS_DBLCLKS,
        szClass,
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CAPTION
    );
    Util::MakeLayered( hwnd, TRUE, 0, uiAlpha );
    SetWindowText( hwnd, szClass );
    return hwnd;
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

    case WM_CLOSE:    // Do not destroy, just hide  
        Show( FALSE );
        return TRUE;

    default:
        break;
    }
    
    return DefWindowProc( message, wParam, lParam );
}

void TextWindow::printBuffer( )
{
	wstring const wstr = m_wBuffer.str();
    (void)TextOut( m_hDC, m_iHorizontalPos, m_iVerticalPos, wstr.c_str(), static_cast<int>(wstr.size()) );
	m_wBuffer.str( wstring() );
	m_wBuffer.clear();
    m_iHorizontalPos += m_iHorRaster;
}

void TextWindow::printString( wstring data )
{
	m_wBuffer << data;
	printBuffer( );
}

void TextWindow::printNumber( int data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextWindow::printNumber( unsigned int data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextWindow::printFloat( float data )
{
    m_wBuffer << setprecision(3) << data << L"%";
    printBuffer();
}

void TextWindow::printNumber( long long data )
{
    m_iHorizontalPos += m_iHorRaster;
    m_wBuffer << setw(13) << data;
    printBuffer();
}

void TextWindow::printPercentage( unsigned int data )
{
    m_wBuffer << setw(5) << data << L"%";
    printBuffer();
}

void TextWindow::printPercentage( unsigned int val, unsigned int max )
{
    int const iPercentage = ( max == 0 ) ? 0 : ( val * 100 ) / max;
    m_wBuffer << setw(5) << iPercentage << L"% (" << val << L" of " << max << L")";
    printBuffer();
}

void TextWindow::printSpan( unsigned int min, unsigned int max )
{
    m_wBuffer << setw(5) << L" (" << min << L" - " << max << L")";
    printBuffer();
}

void TextWindow::printAsDecValue( DWORD dwValue )
{
    DWORD const intPlaces = dwValue / 1000;
    DWORD const decPlaces = dwValue - 1000 * intPlaces;
    m_wBuffer << intPlaces << L"." << decPlaces / 100;
    printBuffer();
}
