// win32_textBuffer.h 
//
// win32_utilities

#pragma once

#include "windef.h"

import TextBuffer;

using std::wostringstream;

class Win32_TextBuffer : public TextBuffer
{
public:
    Win32_TextBuffer(HDC const, PixelRectSize const &);

	~Win32_TextBuffer() override = default;

	void StartPainting() override;

	void PrintBuffer(wostringstream *, PIXEL const, PIXEL const)  override;

	void AlignLeft () override;
	void AlignRight() override;

private:
	PixelRect m_pixRect; // text buffer area 
    HDC       m_hDC;
};
