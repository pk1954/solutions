// win32_scale.h
//
// win32_utilities

#pragma once

#include <string> 
#include <sstream> 
#include "util.h"
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "Direct2D.h"
#include "dwrite.h"
#include "PixelTypes.h"
#include "PixCoordFp.h"

using std::wostringstream;
using std::to_wstring;
using std::wstring;

class D2D_driver;

struct IDWriteTextFormat;

template <typename LogUnits>
class Scale : public ObserverInterface
{
public:

	void InitHorzScale
	(
		PixCoordFp<LogUnits> * const   pPixCoord,
		D2D_driver           * const   pGraphics,
		wstring                const & wstrLogUnit,
		float                  const   fScaleFactor
	)
	{
		m_bScaleType = false;  
		init(pPixCoord, pGraphics, wstrLogUnit, fScaleFactor);
	}

	void InitVertScale
	(
		PixCoordFp<LogUnits> * const   pPixCoord,
		D2D_driver           * const   pGraphics,
		wstring                const & wstrLogUnit,
		float                  const   fScaleFactor
	)
	{
		m_bScaleType = true;  
		init(pPixCoord, pGraphics, wstrLogUnit, fScaleFactor);
	}

	virtual void Notify(bool const bImmediate)
	{
		Recalc();
	}

	void Recalc()
	{
		static fPixel const MIN_TICK_DIST { 6._fPixel };  

		LogUnits const logMinTickDist { m_pPixCoord->Transform2logUnitSize(MIN_TICK_DIST) };
		float    const log10          { log10f(logMinTickDist.GetValue()) };
		float    const fExp           { floor(log10) };
		float    const fFractPart     { log10 - fExp };
		float    const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };
		fPixel   const fPixScaleLen   { getClRectSizeA() - m_pPixCoord->GetPixelOffset() * 2.0f }; 
		LogUnits const logScaleLen    { m_pPixCoord->Transform2logUnitSize(fPixScaleLen) };

		m_fPixPntEnd   = m_fPixPntStart + makefPixelPnt(fPixScaleLen, 0._fPixel);
		m_logStart     = m_pPixCoord->Transform2logUnitPos(m_pPixCoord->GetPixelOffset());
		m_logEnd       = m_logStart + logScaleLen;
		m_logTickDist  = static_cast<LogUnits>(powf(10.0, fExp) * fFactor);
		m_fPixTickDist = m_pPixCoord->Transform2fPixelSize(m_logTickDist);
		setScaleParams();
		setTextBox();
	}

	void SetOrthoOffset(fPixel const fPixOffset)
	{
		m_fPixPntStart = makefPixelPnt(m_pPixCoord->GetPixelOffset(), fPixOffset);
	}

	void SetOrientation(bool const bMode) 
	{ 
		m_bOrientation = bMode; 
		if (m_bScaleType)  // vertical scale
			m_pTextFormat->SetTextAlignment(bMode ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING);
	};

	void Display() const
	{
		m_pGraphics->DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, SCALE_COLOR);
		displayTicks();
		display
		(
			m_fPixPntStart - fPixelPoint(20._fPixel, 0._fPixel), 
			m_unitPrefix + m_wstrLogUnit
		);
	}

private:

	inline static COLORREF const SCALE_COLOR { RGB(0, 0, 0) };  // CLR_BLACK

	PixCoordFp<LogUnits> * m_pPixCoord       { nullptr }; 
	D2D_driver           * m_pGraphics       { nullptr }; 
	IDWriteTextFormat    * m_pTextFormat     { nullptr };
	float                  m_fScaleFactor    { 1.0f };
	bool                   m_bOrientation    { true };  // true: ticks on positive side of scale
	bool                   m_bScaleType      { false }; // true: vertical, false: horizontal
	fPixelPoint            m_fPixPntStart    {};
	fPixelPoint            m_fPixPntEnd      {};
	fPixel                 m_fPixTickDist    {};
	LogUnits               m_logStart        {};
	LogUnits               m_logEnd          {};
	LogUnits               m_logTickDist     {};
	float                  m_fUnitReduction  {};
	wchar_t                m_unitPrefix      {};
	wstring                m_wstrLogUnit     {};
	fPixelRect             m_textBox         {};

	// private functions

	void init
	(
		PixCoordFp<LogUnits> * const   pPixCoord,
		D2D_driver           * const   pGraphics,
		wstring                const & wstrLogUnit,
		float                  const   fScaleFactor
	)
	{
		m_pPixCoord    = pPixCoord;
		m_pGraphics    = pGraphics;
		m_pTextFormat  = pGraphics->NewTextFormat(12.f);
		m_wstrLogUnit  = wstrLogUnit;
		m_fScaleFactor = fScaleFactor;
		m_pPixCoord->RegisterObserver(this);
	}

	fPixel const getClRectSizeA() const
	{
		fPixelRectSize const size { m_pGraphics->GetClRectSize() };
		return m_bScaleType ? size.GetY() : size.GetX();
	}

	fPixel const getValueA(fPixelPoint const & p) const
	{
		return m_bScaleType ? p.GetY() : p.GetX();
	}

	fPixel const getValueB(fPixelPoint const & p) const
	{
		return m_bScaleType ? p.GetX() : p.GetY();
	}

	void setScaleParams()
	{
		static wchar_t const prefix[] { L'k', L' ', L'm', L'\u03BC', L'\0' };

		wchar_t  const * pPrefix   { prefix };
		float    const   logDist10 { m_logTickDist.GetValue() * 10 };

		m_fUnitReduction = m_fScaleFactor * 1e6f;
		do
		{
			m_fUnitReduction *= 1e-3f;
			m_unitPrefix = *pPrefix++;
		} while ((logDist10 < round(m_fUnitReduction)) && *pPrefix);
	}

	void setTextBox()
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

	fPixelPoint const makefPixelPnt(fPixel const fPixA, fPixel const fPixB) const
	{
		return m_bScaleType ? fPixelPoint(fPixB, fPixA) : fPixelPoint(fPixA, fPixB);
	}

	void displayTick
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

	void displayTicks() const
	{
		static fPixel fLongTick  (10._fPixel);
		static fPixel fMiddleTick( 7._fPixel);
		static fPixel fSmallTick ( 5._fPixel);

		int iNrOfTicks { abs(static_cast<int>((m_logEnd - m_logStart) / m_logTickDist)) };
		for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
		{
			fPixel fTick { (iRun % 5 == 0) ? fLongTick : (iRun % 2 == 0) ? fMiddleTick : fSmallTick};
			fPixel fPix  { m_fPixTickDist * static_cast<float>(iRun) };
			fPixel fPixA { m_pPixCoord->GetPixelOffset() + (m_bScaleType ? -fPix : fPix) }; 
			displayTick(fPixA, fTick);
			if (iRun % 10 == 0)
			{
				LogUnits    lu   { m_logStart + m_logTickDist * static_cast<float>(iRun)};
				int         iLu  { static_cast<int>(round(lu.GetValue() / m_fUnitReduction)) };
				fPixelPoint fPos { makefPixelPnt(fPixA, getValueB(m_fPixPntStart)) };
				display(fPos, to_wstring(iLu));
			}
		}
	}

	void display
	(
		fPixelPoint const fPos,
		wstring     const wstr
	) const
	{
		m_pGraphics->DisplayText(m_textBox + fPos, wstr, SCALE_COLOR, m_pTextFormat);
	}
};
