// win32_textWindowThread.h 
//

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
		BOOL            bAsync
	) :
		m_pTextWindow( pTextWindow ),
		m_hDC( hDC_Memory )
	{ 
	    m_pTextBuffer = new Win32_TextBuffer( hDC_Memory, pixSize );
		if ( bAsync )
			StartThread( strName, bAsync ); 
	}

	~TextWindowThread()
	{
		delete m_pTextBuffer;
		m_pTextWindow = nullptr;
		m_pTextBuffer = nullptr;
	}

	void Trigger( )
	{
		const unsigned int anyMessageWillDo = 42;

		PostThreadMsg( anyMessageWillDo );
	}

	virtual void ThreadMsgDispatcher( MSG const msg )
	{
        m_pTextBuffer->StartPainting( );
		m_pTextWindow->DoPaint( * m_pTextBuffer );
		m_pTextWindow->Invalidate( FALSE );
	}

private:
	TextWindow * m_pTextWindow;
	TextBuffer * m_pTextBuffer;
    HDC          m_hDC;
};
