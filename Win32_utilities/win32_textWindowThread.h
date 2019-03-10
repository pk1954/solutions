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
		m_pixRect( PixelRect{ PixelPoint::ZERO_VAL(), pixSize } ),
		m_pTextWindow( pTextWindow ),
		m_bAsync( bAsync ),
		m_hDC( hDC_Memory )
	{ 
		TEXTMETRIC textMetric;
		(void)GetTextMetrics( hDC_Memory, &textMetric );
		PIXEL_X cxChar    = PIXEL_X(PIXEL(textMetric.tmAveCharWidth));
		PIXEL_Y cyChar    = PIXEL_Y(PIXEL(textMetric.tmHeight + textMetric.tmExternalLeading));
		PIXEL_X horRaster = cxChar * 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2);
	    m_pTextBuffer = new Win32_TextBuffer( hDC_Memory, cxChar, cyChar, horRaster );
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
		{ 
			COLORREF const CLR_BACK { RGB( 200, 200, 200 ) };
			SetBkColor( m_hDC, CLR_BACK );
			Util::FastFill( m_hDC, m_pixRect );
			SetTextAlign( m_hDC, TA_RIGHT );
		}
        m_pTextBuffer->StartPainting( );
		m_pTextWindow->DoPaint( * m_pTextBuffer );
		m_pTextWindow->Invalidate( FALSE );
	}

private:
	TextWindow * m_pTextWindow;
	TextBuffer * m_pTextBuffer;
	PixelRect    m_pixRect; // text buffer area 
	BOOL         m_bAsync;
    HDC          m_hDC;
};
