// win32_textBuffer.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "util.h"
#include "PixelTypes.h"
#include "textBuffer.h"

using std::wstring;
using std::setprecision;
using std::setw;

void TextBuffer::Initialize
( 
	PIXEL_X const cxChar,
	PIXEL_Y const cyChar,
	PIXEL_X const horRaster
) 
{
	m_cxChar       = cxChar;
	m_cyChar       = cyChar;
	m_pixHorRaster = horRaster;
    m_wBuffer.imbue(std::locale(""));
}

void TextBuffer::StartPainting( ) 
{ 
    setHorizontalPos( 1_TEXT_POSITION );
    m_pixVerticalPos = TOP_MARGIN;
}

void TextBuffer::printBuffer( )
{ 
	PrintBuffer( & m_wBuffer, m_pixHorizontalPos, m_pixVerticalPos );
	m_wBuffer.str( wstring() );
	m_wBuffer.clear();
    m_pixHorizontalPos += m_pixHorRaster;
};

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

void TextBuffer::printNumber( unsigned long long data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextBuffer::printFloat( float data )
{
	if ( data > 0 )
		m_wBuffer << setprecision(3) << data << L"%";
	else 
		m_wBuffer << L"-";
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

void TextBuffer::printAsDecFraction( unsigned long ulValue )
{
	printString( DecFraction( ulValue ) );
}
