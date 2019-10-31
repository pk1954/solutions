// win32_crsrWindow.h 
//
// NNetWindows

#pragma once

#include "NNetReadBuffer.h" 
#include "win32_textWindow.h"

class NNetWindow;

class CrsrWindow: public TextWindow
{
public:
	CrsrWindow( );
	virtual ~CrsrWindow( );

	void Start
	( 
		HWND               const, 
		NNetReadBuffer   * const,
		NNetWindow const * const 
	);

	void Stop( );

	virtual void DoPaint( TextBuffer & );

private:
	NNetReadBuffer   * m_pReadBuffer;
	NNetWindow const * m_pNNetWindow;

	void printMicroMeter( TextBuffer &,	MicroMeter const );
};
