// win32_textBuffer.h 
//

#pragma once

#include <iostream>
#include <sstream> 
#include <string> 
#include "win32_util.h"

class TextBuffer
{
public:
    TextBuffer( HDC, int const, int const );

	virtual ~TextBuffer( ) 
	{
	}

	void StartPainting( );

    void nextLine( int iHorPos = 1 )     
    { 
        setHorizontalPos( iHorPos );
        m_iVerticalPos += m_cyChar;
    }

    void setHorizontalPos( unsigned int uiPos )
    {
        m_iHorizontalPos = LEFT_MARGIN + uiPos * m_iHorRaster;
    }

    void nextLine( std::wstring data, int iHorPos = 1 )
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

    int const LEFT_MARGIN = 30;

    std::wostringstream m_wBuffer;
    HDC                 m_hDC;
	PIXEL               m_width;
	PIXEL        	    m_height;
	int                 m_cyChar;
    int                 m_cxChar;
    int                 m_iHorizontalPos;
    int                 m_iVerticalPos;
    int                 m_iHorRaster;
};
