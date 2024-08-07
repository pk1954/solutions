// Win32_TextBuffer.ixx
//
// Toolbox\win32_utilities

module;

#include <Windows.h>

export module Win32_TextBuffer;

import std;
import TextBuffer;
import Types;

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

	void     SetBackgroundColor(COLORREF const);
	COLORREF GetBackgroundColor() const { return GetBkColor(m_hDC); }

private:
	PixelRect m_pixRect; // text buffer area 
	HDC       m_hDC;
};
