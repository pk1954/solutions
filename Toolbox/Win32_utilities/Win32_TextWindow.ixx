// Win32_TextWindow.ixx
//
// Toolbox\Win32_utilities

module;

#include <Windows.h>

export module TextWindow;

import std;
import Util;
import TextBuffer;
import Types;
import Win32_TextBuffer;
import BaseWindow;

using std::unique_ptr;
using std::wstring;

class TextWindowThread;

export class TextWindow : public BaseWindow
{
public:
	TextWindow();
	~TextWindow() override;

	void StartTextWindow(HWND const, PixelRect const&, LPCTSTR const, UINT const, VisCrit const&);

	virtual void PaintText(TextBuffer&) = 0;

	void     Trigger(bool const = false)           final;
	LPARAM   AddContextMenuEntries(HMENU const)    final;

	void     SetBackgroundColor(COLORREF const c) { m_pTextBuffer->SetBackgroundColor(c); }
	COLORREF GetBackgroundColor() const           { return m_pTextBuffer->GetBackgroundColor(); };

private:

	void OnPaint() final;

	HBITMAP                      m_hBitmap { nullptr };
	unique_ptr<Win32_TextBuffer> m_pTextBuffer;
	HDC                          m_hDC_Memory;
};
