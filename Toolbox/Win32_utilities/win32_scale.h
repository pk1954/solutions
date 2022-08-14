// win32_scale.h
//
// Toolbox\win32_utilities

#pragma once

#include <string> 
#include <sstream> 
#include "dwrite.h"
#include "win32_baseScale.h"
#include "win32_util_resource.h"
#include "win32_graphicsWindow.h"

import Util;
import ObserverInterface;
import MoreTypes;
import PixFpDimension;
import Direct2D;
import PixelTypes;

using std::fixed;
using std::setprecision;
using std::wostringstream;
using std::to_wstring;
using std::wstring;
using std::bit_cast;

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

	void OnMouseWheel(WPARAM const wParam, LPARAM const lParam) final
	{  
		bool             bResult    { true };
		int        const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
		bool       const bDirection { iDelta > 0 };
		PixelPoint const ptCrsr     { GetRelativeCrsrPosition() };  // screen coordinates
		fPixel           fPixCenter { Convert2fPixel(ptCrsr.GetX()) };

		for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
		{
			if (IsScaleLocked())
				bResult = m_pixCoord.ZoomDir(bDirection, LogUnits::ZERO_VAL());
			else
				bResult = m_pixCoord.ZoomDir(bDirection, fPixCenter);
		}

		if (!bResult)
			MessageBeep(MB_ICONWARNING);

		GraphicsWindow::OnMouseWheel(wParam, lParam);
	}

	bool ZoomCoordFactor(float const factor, fPixel const fPixCenter) final
	{
		return m_pixCoord.ZoomFactor(factor, fPixCenter);
	}

	void MoveCoord(PIXEL const pixDelta) final
	{
		m_pixCoord.Move(pixDelta);
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

		m_upGraphics->FillBackground(CrsrInClientRect() ? COL_HIGHLIGHTED : COL_NORMAL);

		m_upGraphics->DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, GetColor());

		displayTicks(textBox);
		fPixelPoint fPixPos 
		{ 
			IsVertScale()
			? fPixelPoint(0._fPixel, m_fPixPntEnd.GetY() - 16._fPixel)
			: fPixelPoint(m_fPixPntEnd.GetX() + 10._fPixel, 0._fPixel)
		};

		display(textBox + (m_fPixPntStart + fPixPos), m_wstrUnit);
	}

	void setScaleParams()
	{
		float   const fFactor   { TypeAttribute<LogUnits>::factor }; // numbers every 10 ticks (factor 10)
		float   const logDist10 { m_logTickDist.GetValue() * 100 };  // allow one decimal place (another factor 10)             
		int     const iSteps    { StepsOfThousand(logDist10 / fFactor) };  
		m_wstrUnit       = GetUnitPrefix(iSteps) + TypeAttribute<LogUnits>::unit;
		m_fUnitReduction = fFactor * powf(1e-3f, static_cast<float>(iSteps));
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
