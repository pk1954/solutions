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
#include "PixCoordFp.h"
#include "win32_graphicsWindow.h"

using std::wostringstream;
using std::to_wstring;
using std::wstring;

class D2D_driver;

struct IDWriteTextFormat;

template <typename LogUnits>
class Scale : public GraphicsWindow
{
public:

	Scale
	(
		HWND             const hwndParent,
		bool             const bVertScale,
		PixCoordFp<LogUnits> & pixCoord
	)
 	: m_bVertScale(bVertScale),
	  m_pixCoord(pixCoord)
	{
		GraphicsWindow::Initialize(hwndParent, L"ClassScale", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_pTextFormat = m_upGraphics->NewTextFormat(12.f);
	}

	Scale(Scale  &) = delete;
	Scale(Scale &&) = delete;
	Scale       & operator=(const Scale  &) = delete;
	Scale const & operator=(const Scale &&) = delete;

	void SetOrthoOffset(fPixel const fPixOffset)
	{
		m_fPixOrthoOffset = fPixOffset;
	}

	void SetOrientation(bool const bMode) 
	{ 
		m_bOrientation = bMode; 
		if (m_bVertScale) 
			m_pTextFormat->SetTextAlignment(bMode ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING);
	};

private:

	inline static COLORREF const SCALE_COLOR    { RGB(0, 0, 0) };  // CLR_BLACK
	inline static fPixel   const LONG_TICK      { 10._fPixel };
	inline static fPixel   const MIDDLE_TICK    {  7._fPixel };
	inline static fPixel   const SMALL_TICK     {  5._fPixel };
	inline static fPixel   const TEXT_DIST2LINE { LONG_TICK + 2._fPixel };
	inline static fPixel   const TEXT_HORZ_EXT  { 20._fPixel };
	inline static fPixel   const TEXT_VERT_EXT  { 10._fPixel };

	PixCoordFp<LogUnits> & m_pixCoord;

	IDWriteTextFormat    * m_pTextFormat    { nullptr };
	bool                   m_bOrientation   { true };  // true: ticks on negative side of scale
	bool                   m_bVertScale     { false }; // true: vertical, false: horizontal
	fPixel                 m_fPixBorder     { 0._fPixel };
	fPixel                 m_fPixOrthoOffset{ 0._fPixel };
	fPixelPoint            m_fPixPntStart   {};
	fPixelPoint            m_fPixPntEnd     {};
	fPixel                 m_fPixTickDist   {};
	LogUnits               m_logStart       {};
	LogUnits               m_logEnd         {};
	LogUnits               m_logTickDist    {};
	float                  m_fUnitReduction {};
	wstring                m_wstrUnit       {};

	// private functions

	void DoPaint() final
	{
		static fPixel const MIN_TICK_DIST { 6._fPixel };  

		LogUnits    const logMinTickDist { m_pixCoord.Transform2logUnitSize(MIN_TICK_DIST) };
		float       const log10          { log10f(logMinTickDist.GetValue()) };
		float       const fExp           { floor(log10) };
		float       const fFractPart     { log10 - fExp };
		float       const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };
		fPixel      const fPixSizeA      { m_bVertScale ? m_upGraphics->GetClRectHeight() : m_upGraphics->GetClRectWidth() };
		fPixel      const fPixScaleLen   { fPixSizeA - m_pixCoord.GetPixelOffset() - m_fPixBorder }; 
		LogUnits    const logScaleLen    { m_pixCoord.Transform2logUnitSize(fPixScaleLen) };
		fPixelPoint const fPixPntOffset 
		{
			m_bVertScale
			? fPixelPoint(0._fPixel, - fPixScaleLen)
			: fPixelPoint(fPixScaleLen, 0._fPixel)
		};

		m_fPixPntStart = m_bVertScale
		? fPixelPoint(m_fPixOrthoOffset,           getClHeight() - m_pixCoord.GetPixelOffset())
		: fPixelPoint(m_pixCoord.GetPixelOffset(), getClHeight() - m_fPixOrthoOffset);
		m_fPixPntEnd   = m_fPixPntStart + fPixPntOffset;
		m_logStart     = LogUnits(0.0f);
		m_logEnd       = logScaleLen;
		m_logTickDist  = static_cast<LogUnits>(powf(10.0, fExp) * fFactor);
		m_fPixTickDist = m_pixCoord.Transform2fPixelSize(m_logTickDist);
		setScaleParams();
		renderScale();
	}

	void renderScale()
	{
		fPixelRect textBox {};
		setTextBox(textBox);

		if (CrsrInClientRect())
			m_upGraphics->FillBackground(D2D1::ColorF::Aquamarine);

		m_upGraphics->DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, SCALE_COLOR);

		displayTicks(textBox);
		fPixelPoint fPixPos 
		{ 
			m_bVertScale
			? fPixelPoint(0._fPixel, - 16._fPixel)
			: fPixelPoint(10._fPixel,   0._fPixel)
		};

		display(textBox + (m_fPixPntStart + fPixPos), m_wstrUnit);
	}

	void OnMouseMove(WPARAM const wParam, LPARAM const lParam) final
	{
		Trigger();   // cause repaint
		TrackMouse();
	}

	bool OnMouseLeave(WPARAM const wParam, LPARAM const lParam) final
	{
		Trigger();   // cause repaint
		return false;
	}

	bool OnSize(WPARAM const wParam, LPARAM const lParam) final
	{
		GraphicsWindow::OnSize(wParam, lParam);
		Notify(false);
		return true;
	}

	void OnMouseWheel(WPARAM const wParam, LPARAM const lParam) final
	{  
		int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
		bool const bDirection { iDelta > 0 };
		bool       bResult    { true };

		for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
			bResult = m_pixCoord.Zoom(bDirection);

		if (!bResult)
			MessageBeep(MB_ICONWARNING);

		Notify(false);
	}

	fPixel getClHeight() const
	{
		return m_upGraphics->GetClRectHeight();
	}

	void setScaleParams()
	{
		float   const fFactor   { TypeAttribute<LogUnits>::factor };
		float   const logDist10 { m_logTickDist.GetValue() * 10 };  // numbers every 10 ticks
		int     const iSteps    { StepsOfThousand(logDist10 / fFactor) };
		m_wstrUnit       = GetUnitPrefix(iSteps) + TypeAttribute<LogUnits>::unit;
		m_fUnitReduction = fFactor * powf(1e-3f, static_cast<float>(iSteps));
	}

	void setTextBox(fPixelRect & textBox) const
	{
		fPixel horzDist { 0._fPixel };
		fPixel vertDist { 0._fPixel };

		if (m_bVertScale)  // vertical scale
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
		fPixel      const fDir(m_bOrientation ? -fTickExt : fTickExt);
		fPixelPoint const fPixPntStart
		{
			m_bVertScale 
			? fPixelPoint(m_fPixPntStart.GetX(), getClHeight() - fTickA)
			: fPixelPoint(fTickA,                m_fPixPntStart.GetY())
		};
		fPixelPoint const fPixPntEnd
		{
			m_bVertScale 
			? fPixelPoint(m_fPixPntStart.GetX() + fDir, getClHeight() - fTickA)
			: fPixelPoint(fTickA,                       m_fPixPntStart.GetY() + fDir)
		};
		m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, 1._fPixel, SCALE_COLOR);
	}

	void displayTicks(fPixelRect const & textBox) const
	{
		int iNrOfTicks { abs(static_cast<int>((m_logEnd - m_logStart) / m_logTickDist)) };
		for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
		{
			fPixel fTickExt { (iRun % 5 == 0) ? LONG_TICK : (iRun % 2 == 0) ? MIDDLE_TICK : SMALL_TICK};
			fPixel fPix     { m_fPixTickDist * static_cast<float>(iRun) };
			fPixel fPixA    { m_pixCoord.GetPixelOffset() + fPix }; 
			displayTick(fPixA, fTickExt);
			if (iRun % 10 == 0)
			{
				LogUnits    lu   { m_logStart + m_logTickDist * static_cast<float>(iRun)};
				int         iLu  { static_cast<int>(round(lu.GetValue() / m_fUnitReduction)) };
				fPixelPoint fPos { m_bVertScale 
					               ? (fPixelPoint(m_fPixPntStart.GetX(), getClHeight() - fPixA)) 
					               : (fPixelPoint(fPixA,                 m_fPixPntStart.GetY())) 
				                 };
				display(textBox + fPos, to_wstring(iLu));
			}
		}
	}

	void display
	(
		fPixelRect const & textBox,
		wstring    const & wstr
	) const
	{
		m_upGraphics->DisplayText(textBox, wstr, SCALE_COLOR, m_pTextFormat);
	}
};
