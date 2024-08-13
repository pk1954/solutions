// Win32_Scale.ixx
//
// Toolbox\win32_utilities

module;

#include <Windows.h>

export module Scale;

import std;
import std.compat;
import Win32_Util_Resource;
import Win32_Sound;
import Util;
import ObserverInterface;
import Types;
import SaveCast;
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

export PIXEL const V_SCALE_WIDTH  { 35_PIXEL };
export PIXEL const H_SCALE_HEIGHT { 30_PIXEL };

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

	PIXEL GetFixedHeight() const final { return IsVertScale() ? PIXEL::NULL_VAL() : H_SCALE_HEIGHT; }
	PIXEL GetFixedWidth () const final { return IsVertScale() ? V_SCALE_WIDTH : PIXEL::NULL_VAL(); }

	void Apply2AllTicks(auto const & func) const
	{
		float  const fStartTicks { m_logStart / m_logTickDist };
		fPixel const fPixZero    { m_pixCoord.Transform2fPixelPos(LogUnits(0.0f)) };
		int    const iTickStart  { static_cast<int>(fStartTicks) };
		int    const iTickEnd    { static_cast<int>(m_logEnd / m_logTickDist) };

		for (int iTick = iTickStart; iTick <= iTickEnd; ++iTick)
		{
			func(fPixZero + m_fPixTickDist * Cast2Float(iTick), iTick);
		}
	}

	void Notify(bool const bImmediately) final
	{
		prepare();
		BaseScale::Notify(bImmediately);
	}

	void DrawAuxLines(D2D_driver& graphics,	float const fFactor = 1.0f)
	{
		drawAuxLines(graphics,  1, colAnimated(graphics.GetBackgroundColor(), D2D1::ColorF::LightGray, fFactor));  // 0xD3D3D3
		drawAuxLines(graphics,  5, colAnimated(graphics.GetBackgroundColor(), D2D1::ColorF::DarkGray,  fFactor));  // 0xA9A9A9
		drawAuxLines(graphics, 10, colAnimated(graphics.GetBackgroundColor(), D2D1::ColorF::Gray,      fFactor));  // 0x808080
	}

	void DisplayUnit(bool const bOn) { m_bDisplayUnit = bOn; }

	void RegisterObserver  (ObserverInterface       & observer)	{ m_pixCoord.RegisterObserver(observer); }
	void UnregisterObserver(ObserverInterface const & observer) { m_pixCoord.UnregisterObserver(observer); }

	PixFpDimension<LogUnits> & GetDimension() { return m_pixCoord; }

	LogUnits GetRaster() const { return m_logTickDist; }

private:

	PixFpDimension<LogUnits>& m_pixCoord;

	bool        m_bDisplayUnit   { true };
	wstring     m_wstrUnit       {};
	float       m_fUnitReduction {};
	fPixelPoint m_fPixPntStart   {};
	fPixelPoint m_fPixPntEnd     {};
	fPixel      m_fPixTickDist   {};
	float       m_fTickDist      {};
	fPixelRect  m_textBox        {};

	LogUnits m_logStart    {};
	LogUnits m_logEnd      {};
	LogUnits m_logTickDist {};

	mutable wostringstream m_wstrBuffer;

	// private functions

	Color colAnimated
	(
		Color const colBase,
		Color const colTarget,
		float const fFactor
	)
	{
		Color const colorfRes
		(
			colBase.r + (colTarget.r - colBase.r) * fFactor,
			colBase.g + (colTarget.g - colBase.g) * fFactor,
			colBase.b + (colTarget.b - colBase.b) * fFactor
		);
		return colorfRes;
	}

	void drawAuxLines
	(
		D2D_driver& graphics,
		int    const iStep,
		Color  const col
	)
	{
		fPixelRectSize const fPixSize { graphics.GetClRectSize() };
		if (IsVertScale())
			Apply2AllTicks
			(
				[this, &graphics, &fPixSize, iStep, col](fPixel const fPix, int const iTick)
				{
					if (iTick % iStep == 0)
						graphics.DrawLine
						(
							fPixelPoint(0._fPixel, yPos(fPix)),
							fPixelPoint(fPixSize.GetX(), yPos(fPix)),
							1._fPixel,
							col
						);
				}
		);
		else
			Apply2AllTicks
			(
				[this, &graphics, &fPixSize, iStep, col](fPixel const fPix, int const iTick)
				{
					if (iTick % iStep == 0)
						graphics.DrawLine
						(
							fPixelPoint(fPix, 0._fPixel),
							fPixelPoint(fPix, fPixSize.GetY()),
							1._fPixel,
							col
						);
				}
		);
	}

	bool OnMouseWheel(WPARAM const wParam, LPARAM const lParam) final
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
		return true;
	}

	bool ZoomCoordFactor(float const factor, fPixel const fPixCenter) final
	{
		return m_pixCoord.ZoomFactor(factor, fPixCenter);
	}

	void MoveCoord(PIXEL const pixDelta) final
	{
		m_pixCoord.Move(pixDelta);
	}

	void prepare()
	{
		static fPixel const MIN_TICK_DIST { 6._fPixel };

		LogUnits const logMinTickDist { m_pixCoord.Transform2logUnitSize(MIN_TICK_DIST) };
		float    const log10          { log10f(logMinTickDist.GetValue()) };
		float    const fExp           { floor(log10) };
		float    const fFractPart     { log10 - fExp };
		float    const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };

		fPixel fPixStart;
		fPixel fPixEnd;
		fPixel fPixPosOrtho;

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
			m_fPixPntEnd   = fPixelPoint(fPixEnd, fPixPosOrtho);
		}

		m_logStart     = m_pixCoord.Transform2logUnitPos(fPixStart);
		m_logEnd       = m_pixCoord.Transform2logUnitPos(fPixEnd);
		m_logTickDist  = static_cast<LogUnits>(powf(10.0, fExp) * fFactor);
		m_fPixTickDist = m_pixCoord.Transform2fPixelSize(m_logTickDist);

		setScaleParams();
	}

	void setScaleParams()
	{
		float const fFactor   { TypeAttribute<LogUnits>::factor }; // numbers every 10 ticks (factor 10)
		float const logDist10 { m_logTickDist.GetValue() * 100 };  // allow one decimal place (another factor 10)             
		int   const iSteps    { StepsOfThousand(logDist10 / fFactor) };
		if (m_bDisplayUnit)
		{
			m_wstrUnit = GetUnitPrefix(iSteps);
			m_wstrUnit += TypeAttribute<LogUnits>::unit;
		}
		m_fUnitReduction = fFactor * powf(1e-3f, static_cast<float>(iSteps));
		m_fTickDist      = m_logTickDist.GetValue() / m_fUnitReduction;
	}

	void PaintGraphics() final
	{
		setTextBox(m_textBox);
		Apply2AllTicks
		(
			[this](fPixel const fPix, int const iTick)
			{
				displayTick(fPix, iTick);
				if (iTick % 10 == 0)
					displayNumber(iTick, fPix);
			}
		);
		fPixelPoint fPixPos
		{
			m_fPixUnitOffset.IsNotNull()
				? m_fPixUnitOffset
				: IsVertScale()
					? fPixelPoint(0._fPixel, m_fPixPntEnd.GetY() - 16._fPixel)
					: fPixelPoint(m_fPixPntStart.GetX() + 12._fPixel, -1._fPixel)
		};
		if (m_bDisplayUnit)
			display(m_textBox + (m_fPixPntStart + fPixPos), m_wstrUnit);
	}

	void displayTick
	(
		fPixel const fTickA,
		int    const iTick
	) const
	{
		fPixel const fTickExt { (iTick % 5 == 0) ? LONG_TICK : (iTick % 2 == 0) ? MIDDLE_TICK : SMALL_TICK };
		fPixel const fDir(GetTicksDir() ? -fTickExt : fTickExt);
		if (IsVertScale())
		{
			fPixelPoint const fPixPntStart(m_fPixPntStart.GetX(), yPos(fTickA));
			m_upGraphics->DrawLine(fPixPntStart, fPixPntStart.Horz(fDir), 1._fPixel);
		}
		else
		{
			fPixelPoint const fPixPntStart(xPos(fTickA), m_fPixPntStart.GetY());
			m_upGraphics->DrawLine(fPixPntStart, fPixPntStart.Vert(fDir), 1._fPixel);
		};
	}

	void displayNumber
	(
		int    const iTick,
		fPixel const fPix
	) const
	{
		float       const fTick { m_fTickDist * static_cast<float>(iTick) };
		float       const fLu   { round(fTick * 1000.f) / 1000.f };
		fPixelPoint const fPos  { 
			                       IsVertScale()
							       ? (fPixelPoint(m_fPixPntStart.GetX(), yPos(fPix)))
							       : (fPixelPoint(xPos(fPix),            m_fPixPntStart.GetY()))
                                };
		m_wstrBuffer.str(L"");
		m_wstrBuffer << fixed;
		if (fLu == floor(fLu))
			m_wstrBuffer << setprecision(0);
		else
			m_wstrBuffer << setprecision(1);
		m_wstrBuffer << fLu;
		display(m_textBox + fPos, m_wstrBuffer.str());
	}
};
