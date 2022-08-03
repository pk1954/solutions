// Win32_TextBuffer.ixx
//
// Toolbox\win32_utilities

module;

#include <sstream>
#include "Windows.h"
#include "PixelTypes.h"

export module Win32_TextBuffer;

import TextBuffer;

using std::wostringstream;

export class Win32_TextBuffer : public TextBuffer
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
