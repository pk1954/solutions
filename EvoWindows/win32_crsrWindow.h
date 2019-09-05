// win32_crsrWindow.h 
//

#pragma once

#include "EvoReadBuffer.h"
#include "win32_textWindow.h"

class FocusPoint;

class CrsrWindow: public TextWindow
{
public:
    CrsrWindow( );
    virtual ~CrsrWindow( );

    void Start
	( 
		HWND            const, 
		EvoReadBuffer * const,
		FocusPoint    * const 
	);

	void Stop( );

    virtual void DoPaint( TextBuffer & );

private:
    FocusPoint    * m_pFocusPoint;
	EvoReadBuffer * m_pReadBuffer;
};
