// win32_textWindow.h 
//

#pragma once

#include "win32_baseWindow.h"
#include "win32_textBuffer.h"

class TextWindowThread;
class RefreshTimer;

class TextWindow : public BaseWindow
{
public:
    TextWindow( );
	virtual ~TextWindow( ) { };

    void StartTextWindow( HWND const, PixelRect const &, LPCTSTR const, UINT const, bool const, function<bool()> const );
	void StopTextWindow( );

    virtual void DoPaint( TextBuffer & ) = 0;
	virtual void Trigger( );

private:

    virtual bool UserProc( UINT const, WPARAM const, LPARAM const );

	RefreshTimer     * m_pRefreshTimer;
	TextWindowThread * m_pTextWindowThread;
    HDC                m_hDC_Memory;
	HBITMAP            m_hBitmap;
};
