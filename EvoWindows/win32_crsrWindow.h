// win32_crsrWindow.h 
//

#pragma once

#include "win32_textWindow.h"

class FocusPoint;
class ReadBuffer;

class CrsrWindow: public TextWindow
{
public:
    CrsrWindow( );
    virtual ~CrsrWindow( );

    void Start
	( 
		HWND         const, 
		FocusPoint * const, 
		ReadBuffer * const 
	);

    virtual void DoPaint( TextBuffer & );

private:
    FocusPoint * m_pFocusPoint;
	ReadBuffer * m_pReadBuffer;
};
