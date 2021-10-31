// win32_scale.h
//
// win32_utilities

#pragma once

#include <string> 
#include "PixelTypes.h"
#include "PixCoordFp.h"

using std::wstring;

class D2D_driver;

struct IDWriteTextFormat;

class Scale
{
public:
	void InitHorzScale(D2D_driver * const, wstring const &, float const);
	void InitVertScale(D2D_driver * const, wstring const &, float const);
	void SetZoomFactor(float const);
	void SetPixelSize(float const);
	void SetPixelSizeLimits(float const, float const);
	void SetOffset(fPixel const, fPixel const);
	void SetOffset(fPixelPoint const);
	void SetOrientation(bool const);
	void Recalc();
	void Display() const;

	bool const Zoom(bool const);

	float const GetPixelSize() const { return m_pixelSize; }

private:
	using LogUnits = float;

	inline static COLORREF const SCALE_COLOR { RGB(0, 0, 0) };  // CLR_BLACK

	D2D_driver        * m_pGraphics    { nullptr }; 
	IDWriteTextFormat * m_pTextFormat  { nullptr };
	LogUnits            m_pixelSize    {   1.0f };
	LogUnits            m_pixelSizeMin {   1.0f };
	LogUnits            m_pixelSizeMax { 100.0f };
	float               m_fScaleFactor {   1.0f };
	float               m_fZoomFactor  {   1.1f };
	bool                m_bOrientation { true };  // true: ticks on positive side of scale
	bool                m_bScaleType   { false }; // true: vertical, false: horizontal
	fPixelPoint         m_fPixPntStart { fPP_NULL };
	fPixelPoint         m_fPixPntEnd   { fPP_NULL };
	fPixel              m_fPixTickDist {};
	LogUnits            m_logStart     {};
	LogUnits            m_logEnd       {};
	LogUnits            m_logTickDist  {};
	LogUnits            m_logReduction {};
	wchar_t             m_unitPrefix   {};
	wstring             m_wstrLogUnit  {};
	fPixelRect          m_textBox      {};

	// private functions

	void init(D2D_driver * const, wstring const &, float const);

	void setTextBox    ();
	void setScaleParams();

	void displayTicks  ()                                 const;
	void displayTick   (fPixel      const, fPixel  const) const;
	void display       (fPixelPoint const, wstring const) const;

	fPixelPoint const makefPixelPnt (fPixel const, fPixel const) const;
	fPixel      const getClRectSizeA()                           const;
	fPixel      const getValueA     (fPixelPoint const &)        const;
	fPixel      const getValueB     (fPixelPoint const &)        const;
	fPixel      const log2pix       (LogUnits    const  )        const;
	fPixel      const log2pixSize   (LogUnits    const  )        const;
	LogUnits    const pix2log       (fPixel      const  )        const;
	LogUnits    const pix2logSize   (fPixel      const  )        const;
};
