// win32_textBuffer.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "util.h"
#include "win32_util.h"
#include "win32_textBuffer.h"

using namespace std;

TextBuffer::TextBuffer
( 
	HDC       hdc,
	int	const width, 
	int const height
) : 
    m_hDC( hdc ),
	m_width( width ),
	m_height( height )
{
    TEXTMETRIC textMetric;
    (void)GetTextMetrics( m_hDC, &textMetric );
    m_cxChar         = textMetric.tmAveCharWidth;
    m_cyChar         = textMetric.tmHeight + textMetric.tmExternalLeading;
    m_iHorizontalPos = 0;
    m_iVerticalPos   = 0;
    m_iHorRaster     = 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2) * m_cxChar;
    m_wBuffer.imbue(std::locale(""));
}

void TextBuffer::StartPainting( ) 
{ 
    int      const TOP_MARGIN =  5;
    COLORREF const CLR_BACK   = RGB( 200, 200, 200 );

    SetBkColor( m_hDC, CLR_BACK );
	Util::FastFill( m_hDC, RECT{ 0, 0, m_width, m_height } );
    setHorizontalPos( 1 );
    m_iVerticalPos = TOP_MARGIN;
    SetTextAlign( m_hDC, TA_RIGHT );
}

void TextBuffer::printBuffer( )
{
	wstring const wstr = m_wBuffer.str();
    (void)TextOut( m_hDC, m_iHorizontalPos, m_iVerticalPos, wstr.c_str(), static_cast<int>(wstr.size()) );
	m_wBuffer.str( wstring() );
	m_wBuffer.clear();
    m_iHorizontalPos += m_iHorRaster;
}

void TextBuffer::printString( wstring data )
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
    m_iHorizontalPos += m_iHorRaster;
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
