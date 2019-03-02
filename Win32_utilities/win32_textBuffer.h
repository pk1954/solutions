// win32_textBuffer.h 
//

#pragma once

#include <iostream>
#include <sstream> 
#include <string> 
#include "NamedType.h"
#include "win32_util.h"

using TEXT_POSITION = NamedType< int, struct TEXT_POSITION_Parameter >;

constexpr TEXT_POSITION operator"" _TEXT_POSITION( unsigned long long ull )
{
	return TEXT_POSITION( CastToInt( ull ) );
}

class TextBuffer
{
public:
    TextBuffer( HDC const, PixelRectSize const & );

	virtual ~TextBuffer( ) {}

	void StartPainting( );

    void nextLine( TEXT_POSITION iHorPos = 1_TEXT_POSITION )     
    { 
        setHorizontalPos( iHorPos );
        m_pixVerticalPos += m_cyChar;
    }

    void setHorizontalPos( TEXT_POSITION pos )
    {
        m_pixHorizontalPos = LEFT_MARGIN + m_pixHorRaster * pos.GetValue();
    }

    void nextLine( std::wstring data, TEXT_POSITION iHorPos = 1_TEXT_POSITION )
    {
        nextLine( iHorPos );
        printString( data );
    }

    void printString       ( std::wstring );
    void printNumber       ( int );
    void printNumber       ( unsigned int );
    void printNumber       ( long long );
    void printPercentage   ( unsigned int );
    void printPercentage   ( unsigned int, unsigned int );
    void printSpan         ( unsigned int, unsigned int );
    void printAsDecFraction( DWORD );
    void printFloat        ( float );

private:
	void printBuffer( );

    PIXEL const LEFT_MARGIN = 30_PIXEL;

    std::wostringstream m_wBuffer;
    HDC                 m_hDC;
	PixelRectSize       m_pixSize;
	PIXEL               m_cyChar;
    PIXEL               m_cxChar;
    PIXEL               m_pixHorizontalPos;
    PIXEL               m_pixVerticalPos;
    PIXEL               m_pixHorRaster;
};
