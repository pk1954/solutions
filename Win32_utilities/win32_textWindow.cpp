// win32_textWindow.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "win32_util.h"
#include "win32_thread.h"
#include "win32_textWindow.h"

using namespace std;

TextWindow::TextWindow( ) :  
    BaseWindow( ),
    m_hwndParent( nullptr ),
    m_szClass( nullptr ),
    m_uiAlpha( 0 )
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

TextWindow::~TextWindow()
{
	DeleteObject( m_hBitmap );
	DeleteDC( m_hDC );
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
    m_hwndParent = hwndParent;
    m_szClass    = szClass;
    m_uiAlpha    = uiAlpha;

    HWND const hwnd = StartBaseWindow
    ( 
        m_hwndParent,
		xPos, yPos, width, height,
        CS_OWNDC | CS_DBLCLKS,
        m_szClass,
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CAPTION
    );

	PixelRectSize rectSize = GetClRectSize( );
	HDC     const hDC      = GetDC( hwnd );   assert( hDC != nullptr );
	m_hDC      = CreateCompatibleDC( hDC );
	m_hBitmap  = CreateCompatibleBitmap( hDC, rectSize.GetWidth(), rectSize.GetHeight() );
	SelectObject( m_hDC, m_hBitmap );
	ReleaseDC( hwnd, hDC );
	Util::MakeLayered( hwnd, TRUE, 0, m_uiAlpha );
    SetWindowText( hwnd, m_szClass );
//	Continue( );
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
		BitBlt( hDC, 0, 0, rectSize.GetWidth(), rectSize.GetHeight(), m_hDC, 0, 0, SRCCOPY );
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

////////////////////////////////////////////////////////////////

TextThread::TextThread( HWND hwnd ) : 
	m_hwnd( hwnd ),
    Util::Thread( ),
    m_hwndParent( nullptr ),
    m_szClass( nullptr ),
    m_uiAlpha( 0 )
{
        // setup text metrics

    TEXTMETRIC textMetric;
    HDC const hDC = GetDC( m_hwnd );   assert( hDC != nullptr );
    (void)GetTextMetrics( hDC, &textMetric );
    m_cxChar         = textMetric.tmAveCharWidth;
    m_cyChar         = textMetric.tmHeight + textMetric.tmExternalLeading;
    m_iHorizontalPos = 0;
    m_iVerticalPos   = 0;
    m_iHorRaster     = 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2) * m_cxChar;
    ReleaseDC( nullptr, hDC );
    m_wBuffer.imbue(std::locale(""));
}

TextThread::~TextThread()
{
	DeleteObject( m_hBitmap );
	DeleteDC( m_hDC );
}

void TextThread::ThreadStartupFunc()
{
	PixelRectSize rectSize = Util::GetClRectSize( m_hwnd );
	HDC     const hDC      = GetDC( m_hwnd );   assert( hDC != nullptr );
	m_hDC                  = CreateCompatibleDC( hDC );
	m_hBitmap              = CreateCompatibleBitmap( hDC, rectSize.GetWidth(), rectSize.GetHeight() );
	SelectObject( m_hDC, m_hBitmap );
	ReleaseDC( m_hwnd, hDC );
	Util::MakeLayered( m_hwnd, TRUE, 0, m_uiAlpha );
    SetWindowText( m_hwnd, m_szClass );
//	Continue( );
}

void TextThread::printBuffer( )
{
	wstring const wstr = m_wBuffer.str();
    (void)TextOut( m_hDC, m_iHorizontalPos, m_iVerticalPos, wstr.c_str(), static_cast<int>(wstr.size()) );
	m_wBuffer.str( wstring() );
	m_wBuffer.clear();
    m_iHorizontalPos += m_iHorRaster;
}

void TextThread::printString( wstring data )
{
	m_wBuffer << data;
	printBuffer( );
}

void TextThread::printNumber( int data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextThread::printNumber( unsigned int data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextThread::printFloat( float data )
{
    m_wBuffer << setprecision(3) << data << L"%";
    printBuffer();
}

void TextThread::printNumber( long long data )
{
    m_iHorizontalPos += m_iHorRaster;
    m_wBuffer << setw(13) << data;
    printBuffer();
}

void TextThread::printPercentage( unsigned int data )
{
    m_wBuffer << setw(5) << data << L"%";
    printBuffer();
}

void TextThread::printPercentage( unsigned int val, unsigned int max )
{
    int const iPercentage = ( max == 0 ) ? 0 : ( val * 100 ) / max;
    m_wBuffer << setw(5) << iPercentage << L"% (" << val << L" of " << max << L")";
    printBuffer();
}

void TextThread::printSpan( unsigned int min, unsigned int max )
{
    m_wBuffer << setw(5) << L" (" << min << L" - " << max << L")";
    printBuffer();
}

void TextThread::printAsDecValue( DWORD dwValue )
{
    DWORD const intPlaces = dwValue / 1000;
    DWORD const decPlaces = dwValue - 1000 * intPlaces;
    m_wBuffer << intPlaces << L"." << decPlaces / 100;
    printBuffer();
}



