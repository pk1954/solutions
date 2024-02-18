// Win32_TextWindow.ixx
//
// Toolbox\Win32_utilities

module;

#include <string>
#include <memory>
#include <Windows.h>

export module TextWindow;

import Util;
import TextBuffer;
import Types;
import Win32_TextBuffer;
import BaseWindow;
import Thread;

using std::unique_ptr;
using std::wstring;

class TextWindowThread;

export class TextWindow : public BaseWindow
{
public:
	TextWindow();
	~TextWindow() override;

	void StartTextWindow(HWND const, PixelRect const&, LPCTSTR const, UINT const, VisCrit const&);
	void StopTextWindow();

	virtual void PaintText(TextBuffer&) = 0;

	void     Trigger(bool const = false)           final;
	void     SetBackgroundColorRef(COLORREF const) final;
	COLORREF GetBackgroundColorRef() const         final;
	LPARAM   AddContextMenuEntries(HMENU const)    final;
	void     SetDefaultBackgroundColor()           override;

private:

	void OnPaint() final;

	unique_ptr<TextWindowThread> m_upTextWindowThread;

	HBITMAP m_hBitmap { nullptr };
};

class TextWindowThread : public ::Thread
{
public:
	TextWindowThread(HDC, PixelRectSize&, TextWindow&, wstring const&);
	~TextWindowThread()	override;

	void Terminate() override;
	virtual void Trigger();
	void ThreadMsgDispatcher(MSG const& msg) override;

	void     SetBackgroundColor(COLORREF const c) { m_pTextBuffer->SetBackgroundColor(c); }
	COLORREF GetBackgroundColor() const           { return m_pTextBuffer->GetBackgroundColor(); };

	HDC GetHDC_Memory() { return m_hDC_Memory; }

private:
	const unsigned int anyMessageWillDo = 42;

	TextWindow                 & m_textWindow;
	unique_ptr<Win32_TextBuffer> m_pTextBuffer;
	HDC                          m_hDC_Memory;
};
