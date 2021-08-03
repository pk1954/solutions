// win32_textBuffer.h 
//
// win32_utilities

#pragma once

#include "windef.h"
#include "TextBuffer.h"

class Win32_TextBuffer : public TextBuffer
{
public:
    Win32_TextBuffer(HDC const, PixelRectSize const &);

	virtual ~Win32_TextBuffer() {}

	virtual void StartPainting();

	virtual void PrintBuffer(std::wostringstream *, PIXEL const, PIXEL const);

	virtual void AlignLeft();
	virtual void AlignRight();

private:
	PixelRect m_pixRect; // text buffer area 
    HDC       m_hDC;
};
