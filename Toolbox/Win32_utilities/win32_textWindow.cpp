// Win32_TextWindow.cpp :
//
// Toolbox\Win32_utilities

module;

#include <locale>
#include <cassert>
#include <functional>
#include "Windows.h"

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
	bool      const   bAsync,
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
	Util::MakeLayered(hwnd, true, 0, uiAlpha);
	PixelRectSize pixRectSize { rect.GetSize() };
	m_upTextWindowThread = make_unique<TextWindowThread>
	(
		hDC_Memory, 
		pixRectSize,
		* this, 
		szClass, 
		bAsync 
	);
}

void TextWindow::StopTextWindow()
{
    if (m_upTextWindowThread)
	    m_upTextWindowThread->Terminate();

	DeleteObject(m_hBitmap);
	m_hBitmap = nullptr;
}

void TextWindow::Trigger()
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

COLORREF TextWindow::SetBackgroundColorRef(COLORREF const c) 
{ 
	return m_upTextWindowThread->SetBackgroundColor(c); 
}

COLORREF TextWindow::GetBackgroundColorRef() const 
{ 
	return m_upTextWindowThread->GetBackgroundColor(); 
}
