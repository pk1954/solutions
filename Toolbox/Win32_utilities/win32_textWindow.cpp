// win32_textWindow.cpp :
//
// Win32_utilities

#include "stdafx.h"
#include <locale>
#include "win32_util.h"
#include "win32_thread.h"
#include "win32_util_resource.h"
#include "win32_textWindowThread.h"
#include "win32_textWindow.h"

using std::function;
using std::make_unique;

TextWindow::TextWindow() :  
	BaseWindow()
{ }

TextWindow::~TextWindow() = default;

void TextWindow::StartTextWindow
(
    HWND             const   hwndParent,
	PixelRect        const & rect,
	LPCTSTR          const   szClass,
    UINT             const   uiAlpha,
	bool             const   bAsync,
	function<bool()> const & visibilityCriterion
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
	m_hDC_Memory = CreateCompatibleDC(hDC);
	m_hBitmap    = CreateCompatibleBitmap(hDC);
	SelectObject(m_hDC_Memory, m_hBitmap);
	ReleaseDC(hwnd, hDC);
	Util::MakeLayered(hwnd, true, 0, uiAlpha);
    SetWindowText(hwnd, szClass);
	PixelRectSize pixRectSize { rect.GetSize() };
	m_upTextWindowThread = make_unique<TextWindowThread>
	(
		m_hDC_Memory, 
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

	DeleteDC(m_hDC_Memory);
	m_hDC_Memory = nullptr;
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
	BitBlt(hDC, 0, 0, rectSize.GetXvalue(), rectSize.GetYvalue(), m_hDC_Memory, 0, 0, SRCCOPY);
	(void)EndPaint(&ps);
}
