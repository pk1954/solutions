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
		HWND                 const hwndParent,
		bool                 const bVertScale,
		PixFpDimension<LogUnits> & pixCoord
	)
 	: m_bVertScale(bVertScale),
	  m_pixCoord(pixCoord)
	{
		GraphicsWindow::Initialize(hwndParent, L"ClassScale", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		m_pTextFormat = m_upGraphics->NewTextFormat(12.f);
		pixCoord.RegisterObserver(*this);
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

	void SetRightBorder(fPixel const fPixBorder)
	{
		m_fPixRightBorder = fPixBorder;
	}

	void SetColor(D2D1::ColorF const col)
	{
		m_scaleColor = col;
	}

	D2D1::ColorF GetColor() const
	{ 
		return m_scaleColor; 
	}

private:

	inline static fPixel   const LONG_TICK      { 10._fPixel };
	inline static fPixel   const MIDDLE_TICK    {  7._fPixel };
	inline static fPixel   const SMALL_TICK     {  5._fPixel };
	inline static fPixel   const TEXT_DIST2LINE { LONG_TICK + 2._fPixel };
	inline static fPixel   const TEXT_HORZ_EXT  { 20._fPixel };
	inline static fPixel   const TEXT_VERT_EXT  { 10._fPixel };

	PixFpDimension<LogUnits> & m_pixCoord;

	IDWriteTextFormat * m_pTextFormat    { nullptr };
	bool                m_bOrientation   { true };  // true: ticks on negative side of scale
	bool                m_bVertScale     { false }; // true: vertical, false: horizontal
	fPixel              m_fPixRightBorder{ 0._fPixel };
	fPixel              m_fPixLeftBorder { 0._fPixel };
	fPixel              m_fPixOrthoOffset{ 0._fPixel };
	D2D1::ColorF        m_scaleColor     { D2D1::ColorF::Black };
	fPixelPoint         m_fPixPntStart   {};
	fPixelPoint         m_fPixPntEnd     {};
	fPixel              m_fPixTickDist   {};
	LogUnits            m_logStart       {};
	LogUnits            m_logEnd         {};
	LogUnits            m_logTickDist    {};
	float               m_fUnitReduction {};
	wstring             m_wstrUnit       {};

	// private functions

	void DoPaint() final
	{
		static fPixel const MIN_TICK_DIST { 6._fPixel };  

		LogUnits const logMinTickDist { m_pixCoord.Transform2logUnitSize(MIN_TICK_DIST) };
		float    const log10          { log10f(logMinTickDist.GetValue()) };
		float    const fExp           { floor(log10) };
		float    const fFractPart     { log10 - fExp };
		float    const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };
		fPixel   const fPixSizeA      { m_bVertScale ? m_upGraphics->GetClRectHeight() : m_upGraphics->GetClRectWidth() };
		fPixel   const fPixScaleLen   { fPixSizeA - m_fPixRightBorder }; 
		LogUnits const logScaleLen    { m_pixCoord.Transform2logUnitSize(fPixScaleLen) };

		if ( m_bVertScale )
		{
			auto const fPixPntOffset { fPixelPoint(0._fPixel, -fPixScaleLen) };
			m_fPixPntStart = fPixelPoint(m_fPixOrthoOffset, getClHeight() - m_pixCoord.GetPixelOffset());
			m_fPixPntEnd   = m_fPixPntStart + fPixPntOffset;
			m_logStart     = LogUnits(0.0f);
			m_logEnd       = logScaleLen;
		}
		else
		{
			fPixel const fPixVertPos   { getClHeight() - m_fPixOrthoOffset };
			fPixel const fPixHorzStart { m_fPixLeftBorder };
			fPixel const fPixHorzEnd   { m_upGraphics->GetClRectWidth() - m_fPixRightBorder };
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

		if (CrsrInClientRect())
			m_upGraphics->FillBackground(D2D1::ColorF::Aquamarine);

		m_upGraphics->DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, m_scaleColor);

		displayTicks(textBox);
		fPixel const fPixZeroPos = m_pixCoord.Transform2fPixelPos(LogUnits(0.0f));
		fPixelPoint fPixPos 
		{ 
			m_bVertScale
			? fPixelPoint(0._fPixel, fPixZeroPos - 16._fPixel)
			: fPixelPoint(fPixZeroPos + 10._fPixel, 0._fPixel)
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

	bool OnSize(PIXEL const width, PIXEL const height) final
	{
		GraphicsWindow::OnSize(width, height);
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

		Notify(true);
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
		m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, 1._fPixel, m_scaleColor);
	}

	void displayTicks(fPixelRect const & textBox) const
	{
		float    const fStartTicks   { m_logStart / m_logTickDist };
		LogUnits const logFirstTick  { m_logTickDist * floor(fStartTicks) };
		float    const fUnitTickDist { m_logTickDist.GetValue() / m_fUnitReduction };
		fPixel   const fPixZero      { m_pixCoord.Transform2fPixelPos(LogUnits(0.0f)) };
		int      const iTickStart    { static_cast<int>(fStartTicks) };
		int      const iTickEnd      { static_cast<int>(m_logEnd / m_logTickDist) };

		for (int iTick = iTickStart; iTick <= iTickEnd; ++iTick)
		{
			fPixel fTickExt { (iTick % 5 == 0) ? LONG_TICK : (iTick % 2 == 0) ? MIDDLE_TICK : SMALL_TICK};
			float  fTick    { static_cast<float>(iTick) };
			fPixel fPix     { fPixZero + m_fPixTickDist * fTick }; 
			displayTick(fPix, fTickExt);
			if (iTick % 10 == 0)
			{
				fPixelPoint fPos { m_bVertScale 
					               ? (fPixelPoint(m_fPixPntStart.GetX(), getClHeight() - fPix)) 
					               : (fPixelPoint(fPix,                 m_fPixPntStart.GetY())) 
				                 };
				int         iLu  { static_cast<int>(round(fTick * fUnitTickDist)) };
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
		m_upGraphics->DisplayText(textBox, wstr, m_scaleColor, m_pTextFormat);
	}
};
