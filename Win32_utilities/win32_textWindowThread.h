// win32_textWindowThread.h 
//

#pragma once

#include "win32_textWindow.h"
#include "win32_textBuffer.h"
#include "win32_thread.h"

using namespace std;

class TextWindowThread: public Util::Thread
{
public:
	TextWindowThread
	(
		HDC             hDC_Memory,
		int             iWidth,
		int             iHeight,
		TextWindow    * pTextWindow,
		wstring const & strName,
		BOOL            bAsync
	) :
		m_pTextWindow( pTextWindow ),
		m_bAsync( bAsync)
	{ 
	    m_pTextBuffer = new TextBuffer( hDC_Memory, iWidth, iHeight );
		if ( bAsync )
			StartThread( strName ); 
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

		if ( m_bAsync )
		{
			PostThreadMsg( anyMessageWillDo );
		}
		else
		{
			ThreadMsgDispatcher( MSG{ nullptr, anyMessageWillDo, 0, 0 } );
		}
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
	BOOL         m_bAsync;
};
