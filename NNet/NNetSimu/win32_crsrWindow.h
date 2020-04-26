// win32_crsrWindow.h 
//
// NNetWindows

#pragma once

#include <iostream>
#include "Observable.h" 
#include "win32_textWindow.h"

using std::wostringstream;

class NNetWindow;
class NNetModel;

class CrsrWindow: public TextWindow
{
public:
	CrsrWindow( );
	virtual ~CrsrWindow( );

	void Start
	( 
		HWND               const, 
		Observable       * const,
		NNetWindow const * const,
		NNetModel  const * const
	);

	void Stop( );

	virtual void DoPaint( TextBuffer & );

private:
	NNetWindow const * m_pNNetWindow { nullptr };
	NNetModel  const * m_pModel      { nullptr };

	void printMicroMeter( TextBuffer &,	MicroMeter const );
	void printMilliSecs ( TextBuffer &, MilliSecs  const );

	template <typename T> void printFrequency( TextBuffer & textBuf, T const freq )
	{
		wostringstream wBuffer;
		wBuffer << freq.GetValue() << L" Hz";
		textBuf.printString( wBuffer.str() );
		textBuf.nextLine( );
	}

};
