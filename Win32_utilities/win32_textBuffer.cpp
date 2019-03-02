// win32_textBuffer.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "util.h"
#include "win32_util.h"
#include "win32_textBuffer.h"

using std::setprecision;
using std::setw;

TextBuffer::TextBuffer( HDC const hdc, PixelRectSize const & size ) : 
    m_hDC( hdc ),
	m_pixSize( size )
{
    TEXTMETRIC textMetric;
    (void)GetTextMetrics( m_hDC, &textMetric );
    m_cxChar           = PIXEL(textMetric.tmAveCharWidth);
    m_cyChar           = PIXEL(textMetric.tmHeight + textMetric.tmExternalLeading);
    m_pixHorizontalPos = 0_PIXEL;
    m_pixVerticalPos   = 0_PIXEL;
    m_pixHorRaster     = m_cxChar * 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2);
    m_wBuffer.imbue(std::locale(""));
}

void TextBuffer::StartPainting( ) 
{ 
    PIXEL    const TOP_MARGIN = 5_PIXEL;
    COLORREF const CLR_BACK   = RGB( 200, 200, 200 );

    SetBkColor( m_hDC, CLR_BACK );
	Util::FastFill( m_hDC, PixelRect{ PixelPoint::ZERO_VAL(), m_pixSize } );
    setHorizontalPos( 1_TEXT_POSITION );
    m_pixVerticalPos = TOP_MARGIN;
    SetTextAlign( m_hDC, TA_RIGHT );
}

void TextBuffer::printBuffer( )
{
	std::wstring const wstr = m_wBuffer.str();
    (void)TextOut
	( 
		m_hDC, 
		m_pixHorizontalPos.GetValue(), 
		m_pixVerticalPos.GetValue(), 
		wstr.c_str(), 
		static_cast<int>(wstr.size()) 
	);
	m_wBuffer.str( std::wstring() );
	m_wBuffer.clear();
    m_pixHorizontalPos += m_pixHorRaster;
}

void TextBuffer::printString( std::wstring data )
{
	m_wBuffer << data;
	printBuffer( );
}

void TextBuffer::printNumber( int data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextBuffer::printNumber( unsigned int data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextBuffer::printFloat( float data )
{
    m_wBuffer << setprecision(3) << data << L"%";
    printBuffer();
}

void TextBuffer::printNumber( long long data )
{
    m_pixHorizontalPos += m_pixHorRaster;
    m_wBuffer << setw(13) << data;
    printBuffer();
}

void TextBuffer::printPercentage( unsigned int data )
{
    m_wBuffer << setw(5) << data << L"%";
    printBuffer();
}

void TextBuffer::printPercentage( unsigned int val, unsigned int max )
{
    int const iPercentage = ( max == 0 ) ? 0 : ( val * 100 ) / max;
    m_wBuffer << setw(5) << iPercentage << L"% (" << val << L" of " << max << L")";
    printBuffer();
}

void TextBuffer::printSpan( unsigned int min, unsigned int max )
{
    m_wBuffer << setw(5) << L" (" << min << L" - " << max << L")";
    printBuffer();
}

void TextBuffer::printAsDecFraction( DWORD dwValue )
{
	printString( DecFraction( dwValue ) );
}
