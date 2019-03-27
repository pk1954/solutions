// textBuffer.h 
//
// Utilities

#pragma once

#include <iostream>
#include <sstream> 
#include <string> 
#include "PixelTypes.h"

using TEXT_POSITION = NamedType< int, struct TEXT_POSITION_Parameter >;

constexpr TEXT_POSITION operator"" _TEXT_POSITION( unsigned long long ull )
{
	return TEXT_POSITION( CastToInt( ull ) );
}

class TextBuffer
{
public:
	virtual ~TextBuffer( ) {}

    void Initialize( PIXEL const,	PIXEL const );

	virtual void PrintBuffer( std::wostringstream *, PIXEL const, PIXEL const ) = 0;

	virtual void StartPainting( );

    void nextLine( TEXT_POSITION iHorPos = 1_TEXT_POSITION )     
    { 
        setHorizontalPos( iHorPos );
        m_pixVerticalPos += m_pixVertRaster;
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
    void printNumber       ( unsigned long long );
    void printNumber       ( float );
    void printPercentage   ( unsigned int );
    void printPercentage   ( unsigned int, unsigned int );
    void printSpan         ( unsigned int, unsigned int );
    void printAsDecFraction( unsigned long );

private:
	void printBuffer();

	PIXEL const LEFT_MARGIN { 30_PIXEL };
	PIXEL const TOP_MARGIN  {  5_PIXEL };

    std::wostringstream m_wBuffer;

	PixelRect m_pixRect; // text buffer area 
    PIXEL   m_pixHorizontalPos;  
    PIXEL   m_pixVerticalPos;
    PIXEL   m_pixHorRaster;
    PIXEL   m_pixVertRaster;
};
