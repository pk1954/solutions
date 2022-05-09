// win32_scale.h
//
// Toolbox\win32_utilities

#pragma once

#include <string> 
#include <sstream> 
#include "util.h"
#include "observerInterface.h"
#include "MoreTypes.h"
#include "Direct2D.h"
#include "dwrite.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "win32_baseScale.h"
#include "win32_util_resource.h"
#include "win32_graphicsWindow.h"

using std::fixed;
using std::setprecision;
using std::wostringstream;
using std::to_wstring;
using std::wstring;
using std::bit_cast;

class D2D_driver;

struct IDWriteTextFormat;

template <typename LogUnits>
class Scale : public BaseScale
{
public:

	Scale
	(
		HWND                 const hwndParent,
		bool                 const bVertScale,
		PixFpDimension<LogUnits> & pixCoord
	)
 	: BaseScale(hwndParent, bVertScale),
	  m_pixCoord(pixCoord)
	{
		pixCoord.RegisterObserver(*this);
	}

	Scale(Scale  &) = delete;
	Scale(Scale &&) = delete;
	Scale       & operator=(const Scale  &) = delete;
	Scale const & operator=(const Scale &&) = delete;

private:

	inline static fPixel const LONG_TICK      { 10._fPixel };
	inline static fPixel const MIDDLE_TICK    {  7._fPixel };
	inline static fPixel const SMALL_TICK     {  5._fPixel };
	inline static fPixel const TEXT_DIST2LINE { LONG_TICK + 2._fPixel };
	inline static fPixel const TEXT_HORZ_EXT  { 20._fPixel };
	inline static fPixel const TEXT_VERT_EXT  { 10._fPixel };

	PixFpDimension<LogUnits> & m_pixCoord;

	wstring     m_wstrUnit       {};
	float       m_fUnitReduction {};
	fPixelPoint m_fPixPntStart   {};
	fPixelPoint m_fPixPntEnd     {};
	fPixel      m_fPixTickDist   {};

	LogUnits m_logStart    {};
	LogUnits m_logEnd      {};
	LogUnits m_logTickDist {};

	// private functions

	bool Zoom(bool const bDirection) final
	{
		return m_pixCoord.Zoom(bDirection);
	}

	void Zoom(float const factor) final
	{
		m_pixCoord.Zoom(factor);
	}

	void DoPaint() final
	{
		static fPixel const MIN_TICK_DIST { 6._fPixel };  

		LogUnits const logMinTickDist { m_pixCoord.Transform2logUnitSize(MIN_TICK_DIST) };
		float    const log10          { log10f(logMinTickDist.GetValue()) };
		float    const fExp           { floor(log10) };
		float    const fFractPart     { log10 - fExp };
		float    const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };
		fPixel   const fPixSizeA      { IsVertScale() ? m_upGraphics->GetClRectHeight() : m_upGraphics->GetClRectWidth() };
		fPixel   const fPixScaleLen   { fPixSizeA - GetRightBorder() }; 
		LogUnits const logScaleLen    { m_pixCoord.Transform2logUnitSize(fPixScaleLen) };

		if (IsVertScale())
		{
			auto const fPixPntOffset { fPixelPoint(0._fPixel, -fPixScaleLen) };
			m_fPixPntStart = fPixelPoint(GetOrthoOffset(), getClHeight() - m_pixCoord.GetPixelOffset());
			m_fPixPntEnd   = m_fPixPntStart + fPixPntOffset;
			m_logStart     = LogUnits(0.0f);
			m_logEnd       = logScaleLen;
		}
		else
		{
			fPixel const fPixVertPos   { getClHeight() - GetOrthoOffset() };
			fPixel const fPixHorzStart { GetLeftBorder() };
			fPixel const fPixHorzEnd   { m_upGraphics->GetClRectWidth() - GetRightBorder() };
			m_fPixPntStart = fPixelPoint(fPixHorzStart, fPixVertPos);
			m_fPixPntEnd   = fPixelPoint(fPixHorzEnd,   fPixVertPos);
			m_logStart     = m_pixCoord.Transform2logUnitPos(fPixHorzStart);
			m_logEnd       = m_pixCoord.Transform2logUnitPos(fPixHorzEnd);
		}
		m_logTickDist  = static_cast<LogUnits>(powf(10.0, fExp) * fFactor);
		m_fPixTickDist = m_pixCoord.Transform2fPixelSize(m_logTickDist);
		setScaleParams();
		renderScale();
	}

	void renderScale()
	{
		fPixelRect textBox {};
		setTextBox(textBox);

		m_upGraphics->FillBackground(CrsrInClientRect() ? D2D1::ColorF::Aquamarine : D2D1::ColorF::Azure);

		m_upGraphics->DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, GetColor());

		displayTicks(textBox);
		fPixel const fPixZeroPos = m_pixCoord.Transform2fPixelPos(LogUnits(0.0f));
		fPixelPoint fPixPos 
		{ 
			IsVertScale()
			? fPixelPoint(0._fPixel, fPixZeroPos - 16._fPixel)
			: fPixelPoint(fPixZeroPos + 10._fPixel, 0._fPixel)
		};

		display(textBox + (m_fPixPntStart + fPixPos), m_wstrUnit);
	}

	fPixel getClHeight() const
	{
		return m_upGraphics->GetClRectHeight();
	}

	void setScaleParams()
	{
		float   const fFactor   { TypeAttribute<LogUnits>::factor }; // numbers every 10 ticks (factor 10)
		float   const logDist10 { m_logTickDist.GetValue() * 100 };  // allow one decimal place (another factor 10)             
		int     const iSteps    { StepsOfThousand(logDist10 / fFactor) };  
		m_wstrUnit       = GetUnitPrefix(iSteps) + TypeAttribute<LogUnits>::unit;
		m_fUnitReduction = fFactor * powf(1e-3f, static_cast<float>(iSteps));
	}

	void setTextBox(fPixelRect & textBox) const
	{
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

	void displayTick
	(
		fPixel const fTickA,
		fPixel const fTickExt
	) const
	{
		fPixel      const fDir(GetOrientation() ? -fTickExt : fTickExt);
		fPixelPoint const fPixPntStart
		{
			IsVertScale() 
			? fPixelPoint(m_fPixPntStart.GetX(), getClHeight() - fTickA)
			: fPixelPoint(fTickA,                m_fPixPntStart.GetY())
		};
		fPixelPoint const fPixPntEnd
		{
			IsVertScale() 
			? fPixelPoint(m_fPixPntStart.GetX() + fDir, getClHeight() - fTickA)
			: fPixelPoint(fTickA,                       m_fPixPntStart.GetY() + fDir)
		};
		m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, 1._fPixel, GetColor());
	}

	void displayTicks(fPixelRect const & textBox) const
	{
		float    const fStartTicks   { m_logStart / m_logTickDist };
		LogUnits const logFirstTick  { m_logTickDist * floor(fStartTicks) };
		float    const fUnitTickDist { m_logTickDist.GetValue() / m_fUnitReduction };
		fPixel   const fPixZero      { m_pixCoord.Transform2fPixelPos(LogUnits(0.0f)) };
		int      const iTickStart    { static_cast<int>(fStartTicks) };
		int      const iTickEnd      { static_cast<int>(m_logEnd / m_logTickDist) };

		wostringstream wstrBuffer;

		for (int iTick = iTickStart; iTick <= iTickEnd; ++iTick)
		{
			fPixel fTickExt { (iTick % 5 == 0) ? LONG_TICK : (iTick % 2 == 0) ? MIDDLE_TICK : SMALL_TICK};
			float  fTick    { static_cast<float>(iTick) };
			fPixel fPix     { fPixZero + m_fPixTickDist * fTick }; 
			displayTick(fPix, fTickExt);
			if (iTick % 10 == 0)
			{
				fPixelPoint fPos { IsVertScale() 
					               ? (fPixelPoint(m_fPixPntStart.GetX(), getClHeight() - fPix)) 
					               : (fPixelPoint(fPix,                 m_fPixPntStart.GetY())) 
				                 };
				float const fLu  { round(fTick * fUnitTickDist * 1000.f) / 1000.f };	
				int   const prec { (fLu == floor(fLu)) ? 0 : 1};
				wstrBuffer.str(L"");
				wstrBuffer << fixed << setprecision(prec) << fLu;
				display(textBox + fPos, wstrBuffer.str());
			}
		}
	}
};
