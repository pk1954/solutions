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
using std::make_unique;
using std::wstring;

class TextWindowThread;

export class TextWindow : public BaseWindow
{
public:
	TextWindow();
	~TextWindow() override;

	void StartTextWindow
	(
		HWND      const,
		PixelRect const&,
		LPCTSTR   const,
		UINT      const,
		bool      const,
		VisCrit   const&
	);
	void StopTextWindow();

	virtual void PaintText(TextBuffer&) = 0;

	void Trigger() final;

	void     SetBackgroundColorRef(COLORREF const) final;
	COLORREF GetBackgroundColorRef() const         final;

	void SetDefaultBackgroundColor() override;

	LPARAM AddContextMenuEntries(HMENU const) final;

private:

	void OnPaint() final;

	unique_ptr<TextWindowThread> m_upTextWindowThread;

	HBITMAP m_hBitmap { nullptr };
};

class TextWindowThread : public Util::Thread
{
public:
	TextWindowThread
	(
		HDC             hDC_Memory,
		PixelRectSize & pixSize,
		TextWindow    & textWindow,
		wstring const & strName,
		bool            bAsync
	) :
		m_textWindow(textWindow),
		m_hDC_Memory(hDC_Memory)
	{
		m_pTextBuffer = make_unique<Win32_TextBuffer>(hDC_Memory, pixSize);
		if (bAsync)
			StartThread(strName, bAsync);
		PostThreadMsg(anyMessageWillDo);
	}

	~TextWindowThread()	override
	{
		Terminate();
	}

	void Terminate() override
	{
		DeleteDC(m_hDC_Memory);
		m_hDC_Memory = nullptr;
		Util::Thread::Terminate();
	}

	virtual void Trigger()
	{
		PostThreadMsg(anyMessageWillDo);
	}

	void ThreadMsgDispatcher(MSG const& msg) override
	{
		m_pTextBuffer->StartPainting();
		m_textWindow.PaintText(*m_pTextBuffer);
		m_textWindow.Invalidate(false);
	}

	void     SetBackgroundColor(COLORREF const c) { m_pTextBuffer->SetBackgroundColor(c); }
	COLORREF GetBackgroundColor() const           { return m_pTextBuffer->GetBackgroundColor(); };

	HDC GetHDC_Memory() { return m_hDC_Memory; }

private:
	const unsigned int anyMessageWillDo = 42;

	TextWindow                 & m_textWindow;
	unique_ptr<Win32_TextBuffer> m_pTextBuffer;
	HDC                          m_hDC_Memory;
};
