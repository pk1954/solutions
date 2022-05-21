// win32_baseScale.cpp
//
// Toolbox\win32_utilities

#include "stdafx.h"
#include "win32_baseScale.h"

BaseScale::BaseScale
(
	HWND const hwndParent,
	bool const bVertScale
)
	: m_bVertScale(bVertScale)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassScale", 
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN
	);
	m_pTextFormat = m_upGraphics->NewTextFormat(12.f);
}

void BaseScale::SetOrientation(bool const bMode) 
{ 
	m_bOrientation = bMode; 
	if (m_bVertScale) 
		m_pTextFormat->SetTextAlignment(bMode ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING);
};

void BaseScale::setTextBox(fPixelRect & textBox) const
{
	static fPixel const TEXT_DIST2LINE { LONG_TICK + 2._fPixel };
	static fPixel const TEXT_HORZ_EXT  { 20._fPixel };
	static fPixel const TEXT_VERT_EXT  { 10._fPixel };

	fPixel horzDist { 0._fPixel };
	fPixel vertDist { 0._fPixel };

	if (IsVertScale())  // vertical scale
	{
		horzDist = TEXT_DIST2LINE + TEXT_HORZ_EXT; 
		if (GetOrientation())
			horzDist = - horzDist;
		vertDist -= 4._fPixel;
	}
	else  // horizontal scale
	{
		vertDist = TEXT_DIST2LINE + TEXT_VERT_EXT;
		if (GetOrientation())
			vertDist = - vertDist;
		horzDist += 4._fPixel; 
	}

	textBox = fPixelRect
	(
		horzDist - TEXT_HORZ_EXT,    // left
		vertDist - TEXT_VERT_EXT,    // top
		horzDist + TEXT_HORZ_EXT,    // right
		vertDist + TEXT_VERT_EXT     // bottom
	);
}

void BaseScale::display
(
	fPixelRect const & textBox,
	wstring    const & wstr
) const
{
	m_upGraphics->DisplayText(textBox, wstr, GetColor(), m_pTextFormat);
}

void BaseScale::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	Trigger();   // cause repaint
	GraphicsWindow::OnMouseMove(wParam, lParam);
}

void BaseScale::OnMouseLeave() 
{
	Trigger();   // cause repaint
	GraphicsWindow::OnMouseLeave();
}

bool BaseScale::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	Notify(false);
	return true;
}

void BaseScale::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
	PostMessage2Parent(WM_APP_SCALE_COMMAND, SC_LBUTTONDBLCLK, bit_cast<LPARAM>(this));
}

void BaseScale::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
		bResult = Zoom(bDirection);

	if (!bResult)
		MessageBeep(MB_ICONWARNING);

	UpdateWindow(GetWindowHandle());
//	Notify(true);
}
