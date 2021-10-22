// scale.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "MoreTypes.h"
#include "Direct2D.h"
#include "scale.h"

using std::wostringstream;
using std::to_wstring;

void Scale::Initialize
(
	D2D_driver * const   pGraphics, 
	wstring      const & wstrLogUnit
)
{
	m_pGraphics   = pGraphics;
	m_pTextFormat = pGraphics->NewTextFormat(12.f);
	m_wstrLogUnit = wstrLogUnit;
}

void Scale::SetOffset(fPixelPoint const fPixOffset)
{
	m_fPixOffset = fPixOffset;
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

void Scale::SetCentered(bool const bCentered) 
{ 
	m_bCentered = bCentered; 
	Recalc();
};

void Scale::SetOrientation(bool const bMode) 
{ 
	m_bOrientation = bMode; 
	Recalc();
};

fPixel const Scale::log2pixSize(LogUnits const lu) const 
{ 
	return fPixel(lu / m_fPixelSize); 
}

fPixel const Scale::log2pix(LogUnits const lu) const 
{ 
	return log2pixSize(lu) + m_fPixOffset.GetX(); 
}

Scale::LogUnits const Scale::pix2logSize(fPixel const fp) const	
{ 
	return LogUnits(fp.GetValue() * m_fPixelSize); 
}

Scale::LogUnits const Scale::pix2log(fPixel const fp) const	
{ 
	return pix2logSize(fp - m_fPixOffset.GetX()); 
}

void Scale::Recalc()
{
	fPixelRectSize const fPixClRectSize { m_pGraphics->GetClRectSize() };
	LogUnits       const logMinTickDist { pix2logSize(MIN_TICK_DIST) };
	float          const log10          { log10f(logMinTickDist) };
	float          const fExp           { floor(log10) };
	float          const fFractPart     { log10 - fExp };
	float          const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };

	m_logTickDist = powf(10.0, fExp) * fFactor;

	if (m_logTickDist >= 1e5f)
	{ 
		m_unitPrefix   = L" ";
		m_logReduction = 1e6f;
	}
	else if (m_logTickDist >= 1e2f)
	{
		m_unitPrefix = L"m";
		m_logReduction = 1e3f;
	}
	else
	{
		m_unitPrefix = L"\u03BC";
		m_logReduction = 1e0f;
	}

	if (m_bCentered)
		m_fPixOffset = fPixelPoint(fPixClRectSize.GetX() * 0.1f, fPixClRectSize.GetY() - 20._fPixel);

	fPixel m_fPixScaleLen { fPixClRectSize.GetX() - m_fPixOffset.GetX() * 2.0f };

	m_fPixPntStart = m_fPixOffset;
	m_fPixPntEnd   = m_fPixPntStart + fPixelPoint(m_fPixScaleLen, 0._fPixel);
	m_logStart     = pix2log(m_fPixPntStart.GetX());
	m_logEnd       = pix2log(m_fPixPntEnd.  GetX());
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

void Scale::displayTick
(
	fPixel const fTickX,
	fPixel const fTickExt
) const
{
	fPixel      const fDir(m_bOrientation ? -fTickExt : fTickExt);
	fPixelPoint const fTickStart(fTickX, m_fPixOffset.GetY());
	fPixelPoint const fTickEnd  (fTickX, m_fPixOffset.GetY() + fDir);
	m_pGraphics->DrawLine(fTickStart, fTickEnd, 1._fPixel, SCALE_COLOR);
}

void Scale::displayTicks() const
{
	static fPixel fLongTick  (10._fPixel);
	static fPixel fMiddleTick( 7._fPixel);
	static fPixel fSmallTick ( 5._fPixel);

	fPixel fPixTickDist = log2pixSize(m_logTickDist);

	int iNrOfTicks { static_cast<int>((m_logEnd - m_logStart) / m_logTickDist) };
	for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
	{
		LogUnits lu    { m_logStart + iRun * m_logTickDist };
		fPixel   fTick { (iRun % 5 == 0) ? fLongTick : (iRun % 2 == 0) ? fMiddleTick : fSmallTick};
		fPixel   fPixX { log2pix(lu) };
		displayTick(fPixX, fTick);
		if (iRun % 10 == 0)
			display
			(
				fPixelPoint(fPixX, m_fPixOffset.GetY()), 
				to_wstring(static_cast<int>(lu / m_logReduction))
			);
	}
}

void Scale::display
(
	fPixelPoint const fPos,
    wstring     const wstr
) const
{
	static PIXEL const textWidth  = 40_PIXEL;
	static PIXEL const textHeight = 20_PIXEL;
	static PIXEL const horzDist   =  0_PIXEL;
	static PIXEL const vertDist   = 12_PIXEL;

	PIXEL      const vDist  { m_bOrientation ? -vertDist : (vertDist+textHeight) };
	PixelPoint const pixPos { Convert2PixelPoint(fPos) };
	PixelRect  const pixRect
	{ 
		pixPos.GetX() + horzDist - textWidth,  // left
		pixPos.GetY() + vDist    - textHeight, // top
		pixPos.GetX() + horzDist + textWidth,  // right
		pixPos.GetY() + vDist                  // bottom
	};
	m_pGraphics->DisplayText(pixRect, wstr, SCALE_COLOR, m_pTextFormat);
}
