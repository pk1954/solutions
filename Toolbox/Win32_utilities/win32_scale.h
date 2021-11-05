// win32_scale.h
//
// Toolbox\win32_utilities

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

	fPixelRect const GetScaleRect() const
	{
		fPixel const fPixSize { 30.0_fPixel };
		fPixelRect   rect(m_fPixPntStart, m_fPixPntEnd);

		if (m_bScaleType)  //vertical
		{
			if (m_bOrientation)       
				rect.SetLeft (m_fPixPntStart.GetX() - fPixSize);
			else                      
				rect.SetRight(m_fPixPntEnd.GetX() + fPixSize);
		}
		else     // horizontal
		{
			if (m_bOrientation)       
				rect.SetTop   (m_fPixPntStart.GetY() - fPixSize);
			else                      
				rect.SetBottom(m_fPixPntEnd.GetY() + fPixSize);
		}
		return rect;
	}

private:

	inline static COLORREF const SCALE_COLOR    { RGB(0, 0, 0) };  // CLR_BLACK
	inline static fPixel   const LONG_TICK      { 10._fPixel };
	inline static fPixel   const MIDDLE_TICK    {  7._fPixel };
	inline static fPixel   const SMALL_TICK     {  5._fPixel };
	inline static fPixel   const TEXT_DIST2LINE { LONG_TICK + 2._fPixel };
	inline static fPixel   const TEXT_HORZ_EXT  { 20._fPixel };
	inline static fPixel   const TEXT_VERT_EXT  { 10._fPixel };

	PixCoordFp<LogUnits> * m_pPixCoord       { nullptr }; 
	D2D_driver           * m_pGraphics       { nullptr }; 
	IDWriteTextFormat    * m_pTextFormat     { nullptr };
	float                  m_fScaleFactor    { 1.0f };
	bool                   m_bOrientation    { true };  // true: ticks on negative side of scale
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
		fPixel horzDist { 0._fPixel };
		fPixel vertDist { 0._fPixel };

		if (m_bScaleType)  // vertical scale
		{
			horzDist = TEXT_DIST2LINE + TEXT_HORZ_EXT; 
			if (m_bOrientation)
				horzDist = - horzDist;
			vertDist -= 4._fPixel;
		}
		else  // horizontal scale
		{
			vertDist = TEXT_DIST2LINE + TEXT_VERT_EXT;
			if (m_bOrientation)
				vertDist = - vertDist;
		}

		m_textBox = fPixelRect
		(
			horzDist - TEXT_HORZ_EXT,    // left
			vertDist - TEXT_VERT_EXT,    // top
			horzDist + TEXT_HORZ_EXT,    // right
			vertDist + TEXT_VERT_EXT     // bottom
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
		int iNrOfTicks { abs(static_cast<int>((m_logEnd - m_logStart) / m_logTickDist)) };
		for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
		{
			fPixel fTick { (iRun % 5 == 0) ? LONG_TICK : (iRun % 2 == 0) ? MIDDLE_TICK : SMALL_TICK};
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
