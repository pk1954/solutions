// win32_textBuffer.h 
//
// win32_utilities

#pragma once

#include "windef.h"
#include "TextBuffer.h"

class Win32_TextBuffer : public TextBuffer
{
public:
    Win32_TextBuffer
	( 
		HDC const, 
		PIXEL_X const,
		PIXEL_Y const,
		PIXEL_X const
	);

	virtual ~Win32_TextBuffer( ) {}

	virtual void PrintBuffer( std::wostringstream *, PIXEL_X const, PIXEL_Y const );

private:
    HDC m_hDC;
};
