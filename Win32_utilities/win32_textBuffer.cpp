// win32_textBuffer.cpp :
//

#include "stdafx.h"
#include <locale>
#include <iomanip>
#include "util.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_textBuffer.h"

using std::wstring;
using std::setprecision;
using std::setw;

Win32_TextBuffer::Win32_TextBuffer( HDC const hdc, PixelRectSize const & pixSize ) : 
	m_pixRect( PixelRect{ PixelPoint::ZERO_VAL(), pixSize } ),
	m_hDC( hdc )
{ 
	TEXTMETRIC textMetric;
	(void)GetTextMetrics( m_hDC, &textMetric );
	PIXEL_X cxChar     = PIXEL_X(PIXEL(textMetric.tmAveCharWidth));
	PIXEL_X horRaster  = cxChar * 3 * (textMetric.tmPitchAndFamily & TMPF_FIXED_PITCH ? 3 : 2);
	PIXEL_Y vertRaster = PIXEL_Y(PIXEL(textMetric.tmHeight + textMetric.tmExternalLeading));
	Initialize( horRaster, vertRaster );
}

void Win32_TextBuffer::StartPainting()
{
	COLORREF const CLR_BACK { RGB( 200, 200, 200 ) };
	SetBkColor( m_hDC, CLR_BACK );
	Util::FastFill( m_hDC, m_pixRect );
	SetTextAlign( m_hDC, TA_RIGHT );
	TextBuffer::StartPainting();
}

void Win32_TextBuffer::PrintBuffer
( 
	std::wostringstream * pwBuffer,
	PIXEL_X const xPos,
	PIXEL_Y const yPos
)
{
	wstring const wstr { pwBuffer->str() };
    (void)TextOut
	( 
		m_hDC, 
		xPos.GetBaseValue(), 
		yPos.GetBaseValue(), 
		wstr.c_str(), 
		static_cast<int>(wstr.size()) 
	);
}
