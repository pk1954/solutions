// Win32_TextBuffer.cpp :
//
// Toolbox\win32_utilities

module Win32_TextBuffer;

import std;
import Util;
import Types;
import WinBasics;

using std::wstring;
using std::wostringstream;
using std::setprecision;
using std::setw;

Win32_TextBuffer::Win32_TextBuffer(HDC const hdc, PixelRectSize const & pixSize) : 
	m_pixRect(PixelRect{ PP_ZERO, pixSize }),
	m_hDC(hdc)
{ 
	TEXTMETRIC textMetric;
	GetTextMetricsW(m_hDC, &textMetric);
	PIXEL cxChar     = PIXEL(PIXEL(textMetric.tmAveCharWidth));
	PIXEL horRaster  = cxChar * 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2);
	PIXEL vertRaster = PIXEL(PIXEL(textMetric.tmHeight + textMetric.tmExternalLeading));
	Initialize(horRaster, vertRaster);
}

void Win32_TextBuffer::SetBackgroundColor(COLORREF const c)
{
	SetBkColor(m_hDC, c);
}

void Win32_TextBuffer::StartPainting()
{
	::FastFill(m_hDC, m_pixRect);
	TextBuffer::StartPainting();
}

void Win32_TextBuffer::AlignLeft()
{
	SetTextAlign(m_hDC, TA_LEFT);
}

void Win32_TextBuffer::AlignRight()
{
	SetTextAlign(m_hDC, TA_RIGHT);
}

void Win32_TextBuffer::PrintBuffer
(
	wostringstream * pwBuffer,
	PIXEL const xPos,
	PIXEL const yPos
)
{
	wstring const wstr { pwBuffer->str() };
    TextOutW
	(
		m_hDC, 
		xPos.GetValue(), 
		yPos.GetValue(), 
		wstr.c_str(), 
		static_cast<int>(wstr.size()) 
	);
}
