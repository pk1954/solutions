// win32_crsrWindow.h 
//
// NNetWindows

#pragma once

#include <iostream>
#include "win32_textWindow.h"

using std::wostringstream;

class NNetWindow;
class NNetModelReaderInterface;

class CrsrWindow: public TextWindow
{
public:
	CrsrWindow( );
	virtual ~CrsrWindow( );

	void Start
	( 
		HWND                             const, 
		NNetWindow               const * const,
		NNetModelReaderInterface const * const
	);

	void Stop( );

	virtual void DoPaint( TextBuffer & );

private:
	NNetWindow               const * m_pNNetWindow           { nullptr };
	NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };

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
