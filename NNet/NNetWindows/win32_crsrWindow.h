// win32_crsrWindow.h 
//
// NNetWindows

#pragma once

#include "Observable.h" 
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
		Observable       * const,
		NNetWindow const * const 
	);

	void Stop( );

	virtual void DoPaint( TextBuffer & );

private:
	NNetWindow const * m_pNNetWindow;

	void printMicroMeter( TextBuffer &,	MicroMeter const );
};
