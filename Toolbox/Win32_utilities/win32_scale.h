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
#include "win32_baseWindow.h"

using std::wostringstream;
using std::to_wstring;
using std::wstring;

class D2D_driver;

struct IDWriteTextFormat;

template <typename LogUnits>
class Scale : public BaseWindow
{
public:

	Scale
	(
		HWND                   const hwndParent,
		bool                   const bVertScale,
		PixCoordFp<LogUnits> * const pPixCoord
	)
 	: m_bVertScale(bVertScale),
	  m_pPixCoord(pPixCoord)
	{
		HWND hwnd = StartBaseWindow
		(
			hwndParent,
			CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
			L"ClassScale",
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			nullptr,
			nullptr
		);
		m_graphics.Initialize(hwnd);
		m_trackStruct.hwndTrack = hwnd;
		m_pTextFormat = m_graphics.NewTextFormat(12.f);
	}

	Scale(Scale  &) = delete;
	Scale(Scale &&) = delete;
	Scale       & operator=(const Scale  &) = delete;
	Scale const & operator=(const Scale &&) = delete;

	~Scale() final
	{
		m_graphics.ShutDown();
		DestroyWindow();
	}

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

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	D2D_driver             m_graphics       { };
	PixCoordFp<LogUnits> * m_pPixCoord      { nullptr }; 
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
	wchar_t                m_unitPrefix     {};

	// private functions

	void doPaint()
	{
		static fPixel const MIN_TICK_DIST { 6._fPixel };  

		LogUnits    const logMinTickDist { m_pPixCoord->Transform2logUnitSize(MIN_TICK_DIST) };
		float       const log10          { log10f(logMinTickDist.GetValue()) };
		float       const fExp           { floor(log10) };
		float       const fFractPart     { log10 - fExp };
		float       const fFactor        { (fFractPart >= log10f(5.f)) ? 10.f : (fFractPart >= log10f(2.f)) ? 5.f : 2.f };
		fPixel      const fPixSizeA      { m_bVertScale ? m_graphics.GetClRectHeight() : m_graphics.GetClRectWidth() };
		fPixel      const fPixScaleLen   { fPixSizeA - m_pPixCoord->GetPixelOffset() - m_fPixBorder }; 
		LogUnits    const logScaleLen    { m_pPixCoord->Transform2logUnitSize(fPixScaleLen) };
		fPixelPoint const fPixPntOffset 
		{
			m_bVertScale
			? fPixelPoint(0._fPixel, - fPixScaleLen)
			: fPixelPoint(fPixScaleLen, 0._fPixel)
		};
		fPixelRect textBox {};

		m_fPixPntStart = m_bVertScale
		? fPixelPoint(m_fPixOrthoOffset,             getClHeight() - m_pPixCoord->GetPixelOffset())
		: fPixelPoint(m_pPixCoord->GetPixelOffset(), getClHeight() - m_fPixOrthoOffset);
		m_fPixPntEnd   = m_fPixPntStart + fPixPntOffset;
		m_logStart     = m_pPixCoord->Transform2logUnitPos(m_pPixCoord->GetPixelOffset());
		m_logEnd       = m_logStart + logScaleLen;
		m_logTickDist  = static_cast<LogUnits>(powf(10.0, fExp) * fFactor);
		m_fPixTickDist = m_pPixCoord->Transform2fPixelSize(m_logTickDist);
		setScaleParams();
		setTextBox(textBox);

		if (CrsrInClientRect())
			m_graphics.FillBackground(D2D1::ColorF::Aquamarine);

		m_graphics.DrawLine(m_fPixPntStart, m_fPixPntEnd, 1._fPixel, SCALE_COLOR);

		displayTicks(textBox);
		fPixelPoint fPixPos 
		{ 
			m_bVertScale
			? fPixelPoint(0._fPixel, - 16._fPixel)
			: fPixelPoint(10._fPixel,   0._fPixel)
		};

		display
		(
			textBox + (m_fPixPntStart + fPixPos), 
			m_unitPrefix + TypeAttribute<LogUnits>::unit 
		);
	}

	void OnMouseMove(WPARAM const wParam, LPARAM const lParam) final
	{
		Trigger();   // cause repaint
		(void)TrackMouseEvent(& m_trackStruct);
	}

	bool OnMouseLeave(WPARAM const wParam, LPARAM const lParam) final
	{
		Trigger();   // cause repaint
		return false;
	}

	void OnPaint()
	{
		if (IsWindowVisible())
		{
			PAINTSTRUCT ps;
			BeginPaint(&ps);
			if (m_graphics.StartFrame())
			{
				doPaint();
				m_graphics.EndFrame();
			}
			EndPaint(&ps);
		}
	}

	bool OnSize(WPARAM const wParam, LPARAM const lParam) final
	{
		UINT width  = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		m_graphics.Resize(width, height);
		Notify(false);
		return true;
	}

	void OnMouseWheel(WPARAM const wParam, LPARAM const lParam) final
	{  
		int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
		bool const bDirection { iDelta > 0 };
		bool       bResult    { true };

		for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
			bResult = m_pPixCoord->Zoom(bDirection);

		if (!bResult)
			MessageBeep(MB_ICONWARNING);

		Notify(false);
	}

	fPixel getClHeight() const
	{
		return m_graphics.GetClRectHeight();
	}

	void setScaleParams()
	{
		static wchar_t const prefix[] { L'k', L' ', L'm', L'\u03BC', L'\0' };

		wchar_t  const * pPrefix   { prefix };
		float    const   logDist10 { m_logTickDist.GetValue() * 10 };

		m_fUnitReduction = TypeAttribute<LogUnits>::factor * 1e6f;
		do
		{
			m_fUnitReduction *= 1e-3f;
			m_unitPrefix = *pPrefix++;
		} while ((logDist10 < round(m_fUnitReduction)) && *pPrefix);
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
		m_graphics.DrawLine(fPixPntStart, fPixPntEnd, 1._fPixel, SCALE_COLOR);
	}

	void displayTicks(fPixelRect const & textBox) const
	{
		int iNrOfTicks { abs(static_cast<int>((m_logEnd - m_logStart) / m_logTickDist)) };
		for (int iRun = 0; iRun <= iNrOfTicks; ++iRun)
		{
			fPixel fTickExt { (iRun % 5 == 0) ? LONG_TICK : (iRun % 2 == 0) ? MIDDLE_TICK : SMALL_TICK};
			fPixel fPix     { m_fPixTickDist * static_cast<float>(iRun) };
			fPixel fPixA    { m_pPixCoord->GetPixelOffset() + fPix	}; 
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
		wstring    const   wstr
	) const
	{
		m_graphics.DisplayText(textBox, wstr, SCALE_COLOR, m_pTextFormat);
	}
};
