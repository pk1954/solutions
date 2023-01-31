// Win32_Scale.ixx
//
// Toolbox\win32_utilities

module;

#include <bit> 
#include <string> 
#include <sstream> 
#include <iomanip> 
#include "dwrite.h"

export module Scale;

import Win32_Util_Resource;
import Util;
import ObserverInterface;
import Types;
import PixFpDimension;
import Direct2D;
import GraphicsWindow;
import BaseScale;

using std::fixed;
using std::streamsize;
using std::setprecision;
using std::wostringstream;
using std::to_wstring;
using std::wstring;
using std::bit_cast;

export template <typename LogUnits>
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

	Scale                 (      Scale& ) = delete;
	Scale                 (      Scale&&) = delete;
	Scale& operator=      (const Scale& ) = delete;
	Scale const& operator=(const Scale&&) = delete;

private:

	PixFpDimension<LogUnits>& m_pixCoord;

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
		if (IsZoomAllowed())
		{
			bool             bResult    { true };
			int        const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
			bool       const bDirection { iDelta > 0 };
			PixelPoint const ptCrsr     { GetRelativeCrsrPosition() };
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
		}
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
		static D2D1::ColorF const colBackGround { CrsrInClientRect() ? COL_HIGHLIGHTED : COL_NORMAL };
		static fPixel       const MIN_TICK_DIST { 6._fPixel };

		LogUnits const logMinTickDist { m_pixCoord.Transform2logUnitSize(MIN_TICK_DIST) };
		float    const log10          { log10f(logMinTickDist.GetValue()) };
		float    const fExp           { floor(log10) };
		float    const fFractPart     { log10 - fExp };
		float    const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };

		fPixel fPixStart;
		fPixel fPixEnd;
		fPixel fPixPosOrtho;

		m_upGraphics->FillBackground(colBackGround);

		if (IsVertScale())
		{
			fPixPosOrtho   = GetOrthoOffset();
			fPixStart      = GetBottomBorder();
			fPixEnd        = getClHeight() - GetTopBorder();
			m_fPixPntStart = fPixelPoint(fPixPosOrtho, fPixStart);
			m_fPixPntEnd   = fPixelPoint(fPixPosOrtho, fPixEnd);
		}
		else
		{
			fPixPosOrtho   = getClHeight() - GetOrthoOffset();
			fPixStart      = GetLeftBorder();
			fPixEnd        = getClWidth() - GetRightBorder();
			m_fPixPntStart = fPixelPoint(fPixStart, fPixPosOrtho);
			m_fPixPntEnd   = fPixelPoint(fPixEnd,   fPixPosOrtho);
			m_upGraphics->DrawLine
			(
				fPixelPoint(GetLeftBorder(), getClHeight() - 1.0_fPixel),
				fPixelPoint(getClWidth(), getClHeight() - 1.0_fPixel),
				1._fPixel
			);
		}

		m_logStart     = m_pixCoord.Transform2logUnitPos(fPixStart);
		m_logEnd       = m_pixCoord.Transform2logUnitPos(fPixEnd);
		m_logTickDist  = static_cast<LogUnits>(powf(10.0, fExp) * fFactor);
		m_fPixTickDist = m_pixCoord.Transform2fPixelSize(m_logTickDist);
		
		setScaleParams();
		renderScale(colBackGround);

		if (!IsVertScale())
			m_upGraphics->DrawLine
			(
				fPixelPoint(fPixStart, getClHeight() - 1.0_fPixel),
				fPixelPoint(fPixEnd,   getClHeight() - 1.0_fPixel),
				1._fPixel
			);
	}

	void renderScale(D2D1::ColorF const colBackGround)
	{
		fPixelRect textBox{};
		setTextBox(textBox);

		displayTicks(textBox);
		fPixelPoint fPixPos
		{
			IsVertScale()
			? fPixelPoint(0._fPixel, m_fPixPntEnd.GetY() - 16._fPixel)
			: fPixelPoint(m_fPixPntStart.GetX() + 12._fPixel, -1._fPixel)
		};

		display(textBox + (m_fPixPntStart + fPixPos), m_wstrUnit, colBackGround);
	}

	void setScaleParams()
	{
		float   const fFactor   { TypeAttribute<LogUnits>::factor }; // numbers every 10 ticks (factor 10)
		float   const logDist10 { m_logTickDist.GetValue() * 100 };  // allow one decimal place (another factor 10)             
		int     const iSteps    { StepsOfThousand(logDist10 / fFactor) };
		m_wstrUnit = GetUnitPrefix(iSteps);
		m_wstrUnit += TypeAttribute<LogUnits>::unit;
		m_fUnitReduction = fFactor * powf(1e-3f, static_cast<float>(iSteps));
	}

	void displayTick
	(
		fPixel const fTickA,
		fPixel const fTickExt
	) const
	{
		fPixel      const fDir(GetTicksDir() ? -fTickExt : fTickExt);
		fPixelPoint const fPixPntStart
		{
			IsVertScale()
			? fPixelPoint(m_fPixPntStart.GetX(), yPos(fTickA))
			: fPixelPoint(xPos(fTickA),          m_fPixPntStart.GetY())
		};
		fPixelPoint const fPixPntEnd
		{
			IsVertScale()
			? fPixelPoint(m_fPixPntStart.GetX() + fDir, yPos(fTickA))
			: fPixelPoint(xPos(fTickA),                 m_fPixPntStart.GetY() + fDir)
		};
		m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, 1._fPixel);
	}

	void displayTicks
	(
		fPixelRect const& textBox
	) const
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
			fPixel fTickExt { (iTick % 5 == 0) ? LONG_TICK : (iTick % 2 == 0) ? MIDDLE_TICK : SMALL_TICK };
			float  fTick    { static_cast<float>(iTick) };
			fPixel fPix     { fPixZero + m_fPixTickDist * fTick };
			displayTick(fPix, fTickExt);
			if (iTick % 10 == 0)
			{
				fPixelPoint fPos { IsVertScale()
								    ? (fPixelPoint(m_fPixPntStart.GetX(), yPos(fPix)))
								    : (fPixelPoint(xPos(fPix),            m_fPixPntStart.GetY()))
				                 };
				float  const fLu  { round(fTick * fUnitTickDist * 1000.f) / 1000.f };
				wstrBuffer.str(L"");
				wstrBuffer << fixed;
				if (fLu == floor(fLu))
					wstrBuffer << setprecision(0);
				else
					wstrBuffer << setprecision(1);
				wstrBuffer << fLu;
				display(textBox + fPos, wstrBuffer.str());
			}
		}
	}
};
