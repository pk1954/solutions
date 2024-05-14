// Win32_TextWindow.cpp :
//
// Toolbox\Win32_utilities

module;

#include <locale>
#include <cassert>
#include <functional>
#include <memory>
#include <Windows.h>

module TextWindow;

import Win32_Util_Resource;
import Thread;
import Win32_Util;
import BaseWindow;

using std::function;
using std::make_unique;

TextWindow::TextWindow() :  
	BaseWindow()
{ }

TextWindow::~TextWindow() = default;

void TextWindow::StartTextWindow
(
    HWND      const   hwndParent,
	PixelRect const & rect,
	LPCTSTR   const   szClass,
    UINT      const   uiAlpha,
	VisCrit   const & visibilityCriterion
)
{
    HWND const hwnd = StartBaseWindow
    (
        hwndParent,
        CS_OWNDC | CS_DBLCLKS,
        szClass,
        WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION,
		& rect,
		visibilityCriterion
   );

    HDC const hDC { GetDC(hwnd) };
    assert(hDC != nullptr);
	HDC const hDC_Memory { CreateCompatibleDC(hDC) };
	m_hBitmap = CreateCompatibleBitmap(hDC);
	SelectObject(hDC_Memory, m_hBitmap);
	ReleaseDC(hwnd, hDC);
	::MakeLayered(hwnd, true, 0, uiAlpha);
	PixelRectSize pixRectSize { rect.GetSize() };
	m_upTextWindowThread = make_unique<TextWindowThread>
	(
		hDC_Memory, 
		pixRectSize,
		* this, 
		szClass
	);
	SetDefaultBackgroundColor();
}

void TextWindow::SetDefaultBackgroundColor()
{
	m_upTextWindowThread->SetBackgroundColor(RGB(240, 240, 240));  // default background color of static control, can't change
}

//void TextWindow::StopTextWindow()
//{
//	if (m_upTextWindowThread)
//	{
//		m_upTextWindowThread->Terminate();
//		m_upTextWindowThread.release();
//	}
//
//	if (m_hBitmap)
//	{
//		DeleteObject(m_hBitmap);
//		m_hBitmap = nullptr;
//	}
//}

void TextWindow::Trigger(bool const bImmediately)
{
	if (m_upTextWindowThread)
		m_upTextWindowThread->Trigger();
}

void TextWindow::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC      { BeginPaint(&ps) };
	PixelRectSize rectSize { GetClRectSize() };
	BitBlt
	(
		hDC, 0, 0, rectSize.GetXvalue(), rectSize.GetYvalue(), 
		m_upTextWindowThread->GetHDC_Memory(), 
		0, 0, SRCCOPY
	);
	(void)EndPaint(&ps);
}

void TextWindow::SetBackgroundColorRef(COLORREF const c) 
{ 
	m_upTextWindowThread->SetBackgroundColor(c); 
}

COLORREF TextWindow::GetBackgroundColorRef() const 
{ 
	return m_upTextWindowThread->GetBackgroundColor(); 
}

LPARAM TextWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AddColorCtlMenu(hPopupMenu);
	return 0;
}

TextWindowThread::TextWindowThread
(
	HDC             hDC_Memory,
	PixelRectSize & pixSize,
	TextWindow    & textWindow,
	wstring const & strName
) :
	m_textWindow(textWindow),
	m_hDC_Memory(hDC_Memory)
{
	m_pTextBuffer = make_unique<Win32_TextBuffer>(hDC_Memory, pixSize);
	StartThread(strName);
	PostThreadMsg(anyMessageWillDo);
}

TextWindowThread::~TextWindowThread()
{
	Terminate();
}

void TextWindowThread::Terminate()
{
	DeleteDC(m_hDC_Memory);
	m_hDC_Memory = nullptr;
	::Thread::Terminate();
}

void TextWindowThread::Trigger()
{
	PostThreadMsg(anyMessageWillDo);
}

void TextWindowThread::ThreadMsgDispatcher(MSG const& msg)
{
	m_pTextBuffer->StartPainting();
	m_textWindow.PaintText(*m_pTextBuffer);
	m_textWindow.Invalidate(false);
}
