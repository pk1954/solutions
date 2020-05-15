// win32_textWindowThread.h 
//
// win32_utilities

#pragma once

#include "win32_textWindow.h"
#include "win32_textBuffer.h"
#include "win32_thread.h"

class TextWindowThread: public Util::Thread
{
public:
	TextWindowThread
	(
		HDC             hDC_Memory,
		PixelRectSize & pixSize,
		TextWindow    * pTextWindow,
		wstring const & strName,
		bool            bAsync
	) :
		m_pTextWindow( pTextWindow ),
		m_hDC( hDC_Memory )
	{ 
	    m_pTextBuffer = new Win32_TextBuffer( hDC_Memory, pixSize );
		if ( bAsync )
			StartThread( strName, bAsync );
		Trigger( );
	}

	~TextWindowThread()
	{
		delete m_pTextBuffer;
		m_pTextWindow = nullptr;
		m_pTextBuffer = nullptr;
	}

	virtual void Trigger( )
	{
		const unsigned int anyMessageWillDo = 42;

		PostThreadMsg( anyMessageWillDo );
	}

	virtual void ThreadMsgDispatcher( MSG const msg )
	{
        m_pTextBuffer->StartPainting( );
		m_pTextWindow->DoPaint( * m_pTextBuffer );
		m_pTextWindow->Invalidate( false );
	}

private:
	TextWindow * m_pTextWindow;
	TextBuffer * m_pTextBuffer;
    HDC          m_hDC;
};
