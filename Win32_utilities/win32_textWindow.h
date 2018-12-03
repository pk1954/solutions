// win32_textWindow.h 
//

#pragma once

#include "win32_baseWindow.h"
#include "win32_textBuffer.h"

using namespace std;

class TextWindowThread;
class RefreshTimer;

class TextWindow : public BaseWindow
{
public:
    TextWindow( );

    void StartTextWindow( HWND const, int const, int const, int const, int const, LPCTSTR const, UINT const, BOOL const );
	void TerminateTextWindow( );

    virtual void DoPaint( TextBuffer & ) = 0;

private:

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) override;
	virtual void    AddContextMenuEntries( HMENU const, POINT const );
	virtual void    Trigger( );

	RefreshTimer     * m_pRefreshTimer;
	TextWindowThread * m_pTextWindowThread;
    HDC                m_hDC_Memory;
	HBITMAP            m_hBitmap;
};
