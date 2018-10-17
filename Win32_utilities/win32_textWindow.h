// win32_textWindow.h 
//

#pragma once

#include <iostream>
#include <sstream> 
#include <string> 
#include "win32_baseWindow.h"
#include "win32_thread.h"
#include "win32_util.h"

using namespace std;

class GridPoint;

class TextWindow : public BaseWindow
{
public:
    TextWindow( );
	virtual ~TextWindow( ); 

    void StartTextWindow( HWND const, int const, int const, int const, int const, LPCTSTR const, UINT const, BOOL const );

    void nextLine( int iHorPos = 1 )     
    { 
        setHorizontalPos( iHorPos );
        m_iVerticalPos += m_cyChar;
    }

    void setHorizontalPos( unsigned int uiPos )
    {
        m_iHorizontalPos = LEFT_MARGIN + uiPos * m_iHorRaster;
    }

    void nextLine( wstring data, int iHorPos = 1 )
    {
        nextLine( iHorPos );
        printString( data );
    }

    void printString    ( wstring );
    void printNumber    ( int );
    void printNumber    ( unsigned int );
    void printNumber    ( long long );
    void printPercentage( unsigned int );
    void printPercentage( unsigned int, unsigned int );
    void printSpan      ( unsigned int, unsigned int );
    void printAsDecValue( DWORD );
    void printFloat     ( float );

protected:

    virtual void DoPaint( ) = 0;

private:

	void printBuffer( );

    void startPainting( ) 
    { 
        FillBackground( m_hDC, CLR_BACK );
        setHorizontalPos( 1 );
        m_iVerticalPos = TOP_MARGIN;
        SetTextAlign( m_hDC, TA_RIGHT );
    }

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) override;
	virtual void Trigger( )
	{
        startPainting( );
        DoPaint( );
		Invalidate( FALSE );
	}

    COLORREF const CLR_BACK    = RGB( 200, 200, 200 );
    int      const LEFT_MARGIN = 30;
    int      const TOP_MARGIN  =  5;

    wostringstream m_wBuffer;
    HDC            m_hDC;
	HBITMAP        m_hBitmap;
    int            m_cyChar;
    int            m_cxChar;
    int            m_iHorizontalPos;
    int            m_iVerticalPos;
    int            m_iHorRaster;
    HWND           m_hwndParent;
    LPCTSTR        m_szClass;
    UINT           m_uiAlpha;
};

class TextThread: public Util::Thread
{
public:
    TextThread( HWND );
	virtual ~TextThread( ); 

protected:

    void nextLine( int iHorPos = 1 )     
    { 
        setHorizontalPos( iHorPos );
        m_iVerticalPos += m_cyChar;
    }

    void setHorizontalPos( unsigned int uiPos )
    {
        m_iHorizontalPos = LEFT_MARGIN + uiPos * m_iHorRaster;
    }

    void nextLine( wstring data, int iHorPos = 1 )
    {
        nextLine( iHorPos );
        printString( data );
    }

    void printString    ( wstring );
    void printNumber    ( int );
    void printNumber    ( unsigned int );
    void printNumber    ( long long );
    void printPercentage( unsigned int );
    void printPercentage( unsigned int, unsigned int );
    void printSpan      ( unsigned int, unsigned int );
    void printAsDecValue( DWORD );
    void printFloat     ( float );

private:
	void printBuffer( );

	virtual void ThreadStartupFunc();

    int      const LEFT_MARGIN = 30;

	HWND           m_hwnd;
    wostringstream m_wBuffer;
    HDC            m_hDC;
	HBITMAP        m_hBitmap;
	int            m_cyChar;
    int            m_cxChar;
    int            m_iHorizontalPos;
    int            m_iVerticalPos;
    int            m_iHorRaster;
    HWND           m_hwndParent;
    LPCTSTR        m_szClass;
    UINT           m_uiAlpha;
};
