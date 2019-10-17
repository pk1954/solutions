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

    void StartTextWindow( HWND const, PixelRect const &, LPCTSTR const, UINT const, BOOL const, std::function<bool()> const );
	void StopTextWindow( );

    virtual void DoPaint( TextBuffer & ) = 0;
	virtual void Trigger( );

private:

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) override;
	virtual void    AddContextMenuEntries( HMENU const, PixelPoint const );

	RefreshTimer     * m_pRefreshTimer;
	TextWindowThread * m_pTextWindowThread;
    HDC                m_hDC_Memory;
	HBITMAP            m_hBitmap;
};
