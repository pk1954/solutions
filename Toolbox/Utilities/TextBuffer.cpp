// win32_textBuffer.cpp :
//
// Utilities

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
	PIXEL const horRaster,
	PIXEL const vertRaster
) 
{
	m_pixHorRaster  = horRaster;
	m_pixVertRaster = vertRaster;
    m_wBuffer.imbue(std::locale(""));
}

void TextBuffer::StartPainting() 
{ 
	AlignRight();
	setHorizontalPos( 1_TEXT_POSITION );
    m_pixVerticalPos = TOP_MARGIN;
}

void TextBuffer::printBuffer()
{ 
	PrintBuffer( & m_wBuffer, m_pixHorizontalPos, m_pixVerticalPos );
	m_wBuffer.str( wstring() );
	m_wBuffer.clear();
    m_pixHorizontalPos += m_pixHorRaster;
};

void TextBuffer::printString( wstring const & data )
{ 
	m_wBuffer << data;
	printBuffer();
}

void TextBuffer::printNumber( int const data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextBuffer::printNumber( unsigned int const data )
{
    m_wBuffer << data;
    printBuffer();
}

void TextBuffer::printNumber( long const data )
{
	m_wBuffer << data;
	printBuffer();
}

void TextBuffer::printNumber( unsigned long long const data )
{
	m_wBuffer << data;
	printBuffer();
}

void TextBuffer::printNumber( float const data )
{
	if ( data > 0 )
		m_wBuffer << setprecision(3) << data << L"%";
	else 
		m_wBuffer << L"-";
	printBuffer();
}

void TextBuffer::printFloat( float const data )
{
	m_wBuffer << setprecision(3) << data;
	printBuffer();
}

void TextBuffer::printNumber( long long const data )
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

void TextBuffer::printPercentage( unsigned int const val, unsigned int const max )
{
    int const iPercentage = ( max == 0 ) ? 0 : ( val * 100 ) / max;
    m_wBuffer << setw(5) << iPercentage << L"% (" << val << L" of " << max << L")";
    printBuffer();
}

void TextBuffer::printSpan( unsigned int const min, unsigned int const max )
{
    m_wBuffer << setw(5) << L" (" << min << L" - " << max << L")";
    printBuffer();
}

void TextBuffer::printAsMillisecs( microseconds const microSecs )
{
	float millisecs = Cast2Float( microSecs.count() ) / 1000.0f;
	m_wBuffer << std::fixed << std::setprecision(2) << millisecs << L"ms";
	printBuffer();
}
