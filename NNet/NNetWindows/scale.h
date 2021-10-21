// scale.h
//
// NNetWindows

#pragma once

#include <string> 
#include "PixelTypes.h"

using std::wstring;

class D2D_driver;

struct IDWriteTextFormat;

class Scale
{
public:
	void Initialize(D2D_driver * const, wstring const &);

	void SetClientRectSize(PIXEL const, PIXEL const);
	void SetHorzPixelSize(float const);
	void SetOffset(fPixel const, fPixel const);
	void SetOffset(fPixelPoint const);
	void SetCentered(bool const);
	void SetBelowMode(bool const);

	void DisplayStaticScale() const;

private:
	using LogUnits = float;

	inline static COLORREF const SCALE_COLOR { RGB(0, 0, 0) };  // CLR_BLACK

	inline static fPixel const MIN_TICK_DIST { 6._fPixel };  

	fPixel m_fPixClientWidth { 0.0_fPixel };
	fPixel m_fPixClientHeight{ 0.0_fPixel };

	D2D_driver        * m_pGraphics      { nullptr }; 
	IDWriteTextFormat * m_pTextFormat    { nullptr };
	float               m_fHorzPixelSize { 1.0f };
	wstring             m_wstrLogUnit;

	// parameters for fixed scale

	bool        m_bCentered    { false };
	bool        m_bBelowMode   { false };
	fPixelPoint m_fPixOffset   { fPP_NULL };
	fPixelPoint m_fPixPntStart { fPP_NULL };
	fPixelPoint m_fPixPntEnd   { fPP_NULL };
	LogUnits    m_logStart     {};
	LogUnits    m_logEnd       {};
	LogUnits    m_logTickDist  {};
	LogUnits    m_logReduction {};
	wstring     m_unitPrefix   {};

	// private functions

	void calcScaleParams   ();
	void displayTick       (fPixel const, fPixel const) const;
	void displayTicks      () const;
	void display           (fPixelPoint const, wstring const) const;

	fPixel   const log2pix    (LogUnits const) const;
	fPixel   const log2pixSize(LogUnits const) const;
	LogUnits const pix2log    (fPixel   const) const;
	LogUnits const pix2logSize(fPixel   const) const;
};
