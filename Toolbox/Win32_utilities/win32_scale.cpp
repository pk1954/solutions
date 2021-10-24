// win32_scale.cpp
//
// win32_utilities

#include "stdafx.h"
#include <sstream> 
#include "MoreTypes.h"
#include "Direct2D.h"
#include "dwrite.h"
#include "win32_scale.h"

using std::wostringstream;
using std::to_wstring;

void Scale::init
(
	D2D_driver * const   pGraphics, 
	wstring      const & wstrLogUnit,
	float        const   fScaleFactor
)
{
	m_pGraphics    = pGraphics;
	m_pTextFormat  = pGraphics->NewTextFormat(12.f);
	m_wstrLogUnit  = wstrLogUnit;
	m_fScaleFactor = fScaleFactor;
}

void Scale::InitHorzScale
(
	D2D_driver * const   pGraphics, 
	wstring      const & wstrLogUnit,
	float        const   fScaleFactor
)
{
	m_bScaleType = false;  
	init(pGraphics, wstrLogUnit, fScaleFactor);
}

void Scale::InitVertScale
(
	D2D_driver * const   pGraphics, 
	wstring      const & wstrLogUnit,
	float        const   fScaleFactor
)
{
	m_bScaleType = true;  
	init(pGraphics, wstrLogUnit, fScaleFactor);
}

void Scale::SetOffset(fPixelPoint const fPixOffset)
{
	m_fPixPntStart = fPixOffset;
	Recalc();
}

void Scale::SetOffset(fPixel const fPixX, fPixel const fPixY)
{
	SetOffset(fPixelPoint(fPixX, fPixY));
}

void Scale::SetPixelSize(float const fSize) 
{ 
	m_fPixelSize = fSize; 
	Recalc();
};

void Scale::SetOrientation(bool const bMode) 
{ 
	m_bOrientation = bMode; 
	if (m_bScaleType)  // vertical scale
		m_pTextFormat->SetTextAlignment(bMode ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING);
	Recalc();
};

fPixel const Scale::getClRectSizeA() const
{
	fPixelRectSize const size { m_pGraphics->GetClRectSize() };
	return m_bScaleType ? size.GetY() : size.GetX();
}

fPixel const Scale::getValueA(fPixelPoint const & p) const
{
	return m_bScaleType ? p.GetY() : p.GetX();
}

fPixel const Scale::getValueB(fPixelPoint const & p) const
{
	return m_bScaleType ? p.GetX() : p.GetY();
}

fPixel const Scale::log2pixSize(LogUnits const lu) const 
{ 
	return fPixel(lu / m_fPixelSize); 
}

fPixel const Scale::log2pix(LogUnits const lu) const 
{ 
	fPixel const fPntPos { getValueA(m_fPixPntStart) };
	fPixel const fPntExt { log2pixSize(lu) }; 
	return fPntPos + (m_bScaleType ? -fPntExt : fPntExt);
}

Scale::LogUnits const Scale::pix2logSize(fPixel const fp) const	
{ 
	return LogUnits(fp.GetValue() * m_fPixelSize); 
}

Scale::LogUnits const Scale::pix2log(fPixel const fp) const	
{ 
	fPixel const fPntPos { getValueA(m_fPixPntStart) };
	fPixel const fPntExt { fp - fPntPos }; 
	return pix2logSize(m_bScaleType ? -fPntExt : fPntExt); 
}

void Scale::Recalc()
{
	static fPixel const MIN_TICK_DIST {  6._fPixel };  

	LogUnits const logMinTickDist { pix2logSize(MIN_TICK_DIST) };
	float    const log10          { log10f(logMinTickDist) };
	float    const fExp           { floor(log10) };
	float    const fFractPart     { log10 - fExp };
	float    const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };
	fPixel   const fPixScaleLen   { getClRectSizeA() - getValueA(m_fPixPntStart) * 2.0f };

	m_fPixPntEnd  = m_fPixPntStart + makefPixelPnt(fPixScaleLen, 0._fPixel);
	m_logStart    = pix2log(getValueA(m_fPixPntStart));
	m_logEnd      = pix2log(getValueA(m_fPixPntEnd  ));
	m_logTickDist = powf(10.0, fExp) * fFactor;

	setScaleParams();
	setTextBox();
}

void Scale::setScaleParams()
{
	static wchar_t const prefix[] { L'k', L' ', L'm', L'\u03BC', L'\0' };

	wchar_t const * pPrefix   { prefix };
	float   const   logDist10 { m_logTickDist * 10 };

	m_logReduction = m_fScaleFactor * 1e6f;
	do
	{
		m_logReduction *= 1e-3f;
		m_unitPrefix = *pPrefix++;
	} while ((logDist10 < round(m_logReduction)) && *pPrefix);
}

void Scale::setTextBox()
{
	static fPixel const DIST2TICK { 12._fPixel };
	static fPixel const HORZ_EXT  { 20._fPixel };
	static fPixel const VERT_EXT  { 10._fPixel };

	fPixel horzDist { 0._fPixel };
	fPixel vertDist { 0._fPixel };

	if (m_bScaleType)  // vertical scale
	{
		horzDist = DIST2TICK + HORZ_EXT; 
		if (m_bOrientation)
			horzDist = - horzDist;
		vertDist -= 4._fPixel;
	}
	else  // horizontal scale
	{
		vertDist = DIST2TICK + VERT_EXT;
		if (m_bOrientation)
			vertDist = - vertDist;
	}

	m_textBox = fPixelRect
	(
		horzDist - HORZ_EXT,    // left
		vertDist - VERT_EXT,    // top
		horzDist + HORZ_EXT,    // right
		vertDist + VERT_EXT     // bottom
	);
}

void Scale::Display() const
{
	m_pGraphics->DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, SCALE_COLOR);
	displayTicks();
	display
	(
		m_fPixPntStart - fPixelPoint(20._fPixel, 0._fPixel), 
		m_unitPrefix + m_wstrLogUnit
	);
}

fPixelPoint const Scale::makefPixelPnt(fPixel const fPixA, fPixel const fPixB) const
{
	return m_bScaleType ? fPixelPoint(fPixB, fPixA) : fPixelPoint(fPixA, fPixB);
}

void Scale::displayTick
(
	fPixel const fTickA,
	fPixel const fTickExt
) const
{
	fPixel      const fDir(m_bOrientation ? -fTickExt : fTickExt);
	fPixelPoint const fTickStart { makefPixelPnt(fTickA, getValueB(m_fPixPntStart)) };
	fPixelPoint const fTickEnd   { makefPixelPnt(fTickA, getValueB(m_fPixPntStart) + fDir) };
	m_pGraphics->DrawLine(fTickStart, fTickEnd, 1._fPixel, SCALE_COLOR);
}

void Scale::displayTicks() const
{
	static fPixel fLongTick  (10._fPixel);
	static fPixel fMiddleTick( 7._fPixel);
	static fPixel fSmallTick ( 5._fPixel);

	int iNrOfTicks { static_cast<int>((m_logEnd - m_logStart) / m_logTickDist) };
	for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
	{
		LogUnits lu    { m_logStart + iRun * m_logTickDist };
		fPixel   fTick { (iRun % 5 == 0) ? fLongTick : (iRun % 2 == 0) ? fMiddleTick : fSmallTick};
		fPixel   fPixA { log2pix(lu) };
		displayTick(fPixA, fTick);
		if (iRun % 10 == 0)
			display
			(
				makefPixelPnt(fPixA, getValueB(m_fPixPntStart)), 
				to_wstring(static_cast<int>(round(lu / m_logReduction)))
			);
	}
}

void Scale::display
(
	fPixelPoint const fPos,
    wstring     const wstr
) const
{
	m_pGraphics->DisplayText(m_textBox + fPos, wstr, SCALE_COLOR, m_pTextFormat);
}
