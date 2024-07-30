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
	m_pTextBuffer = make_unique<Win32_TextBuffer>(hDC_Memory, pixRectSize);
	m_hDC_Memory = hDC_Memory;
	SetDefaultBackgroundColor();
}

void TextWindow::Trigger(bool const bImmediately)
{
	m_pTextBuffer->StartPainting();
	PaintText(*m_pTextBuffer);
	Invalidate(false);
}

void TextWindow::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC      { BeginPaint(&ps) };
	PixelRectSize rectSize { GetClRectSize() };
	BitBlt
	(
		hDC, 0, 0, rectSize.GetXvalue(), rectSize.GetYvalue(), 
		m_hDC_Memory, 
		0, 0, SRCCOPY
	);
	(void)EndPaint(&ps);
}

LPARAM TextWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AddColorCtlMenu(hPopupMenu);
	return 0;
}
