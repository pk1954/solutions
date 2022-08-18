// win32_textWindow.h 
//
// Toolbox\Win32_utilities

#pragma once

#include <string>
#include <Windows.h>

import Util;
import TextBuffer;
import PixelTypes;
import Win32_TextBuffer;
import BaseWindow;
import Thread;

using std::unique_ptr;
using std::make_unique;
using std::wstring;

class TextWindowThread;

class TextWindow : public BaseWindow
{
public:
    TextWindow();
	~TextWindow() override;

    void StartTextWindow
	(
		HWND      const, 
		PixelRect const &, 
		LPCTSTR   const, 
		UINT      const, 
		bool      const, 
		VisCrit   const &
	);
	void StopTextWindow();

    virtual void DoPaint(TextBuffer &) = 0;

	void Trigger() final;

private:

	void OnPaint() final;

	unique_ptr<TextWindowThread> m_upTextWindowThread;

    HDC     m_hDC_Memory    { nullptr };
	HBITMAP m_hBitmap       { nullptr };
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
		m_hDC(hDC_Memory)
	{
		m_pTextBuffer = make_unique<Win32_TextBuffer>(hDC_Memory, pixSize);
		if (bAsync)
			StartThread(strName, bAsync);
		PostThreadMsg(anyMessageWillDo);
	}

	~TextWindowThread()	override = default;

	virtual void Trigger()
	{
		PostThreadMsg(anyMessageWillDo);
	}

	void ThreadMsgDispatcher(MSG const& msg) override
	{
		m_pTextBuffer->StartPainting();
		m_textWindow.DoPaint(*m_pTextBuffer);
		m_textWindow.Invalidate(false);
	}

private:
	const unsigned int anyMessageWillDo = 42;

	TextWindow           & m_textWindow;
	unique_ptr<TextBuffer> m_pTextBuffer;
	HDC                    m_hDC;
};
