// win32_textWindow.h 
//

#pragma once

#include <iostream>
#include <sstream> 
#include <string> 
#include "win32_baseWindow.h"
#include "win32_textBuffer.h"
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

protected:

    virtual void DoPaint( TextBuffer & ) = 0;

private:

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) override;
	virtual void Trigger( )
	{
        m_pTextBuffer->StartPainting( );
        DoPaint( * m_pTextBuffer );
		Invalidate( FALSE );
	}

	TextBuffer * m_pTextBuffer;
    HDC          m_hDC_Memory;
	HBITMAP      m_hBitmap;
};
