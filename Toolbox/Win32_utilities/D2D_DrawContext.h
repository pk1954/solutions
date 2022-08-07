// D2D_DrawContext.h
//
// Win32_utilities

#pragma once

#include "DrawContext.h"
#include "MoreTypes.h"

import Direct2D;
import PixelTypes;

using std::wstring;
using D2D1::ColorF;

class D2D_DrawContext: public DrawContext
{
public:

	void Start(D2D_driver * const);

	void SetStdFontSize(MicroMeter const &);

	void DrawLine
	(
		MicroMeterPnt const &, 
		MicroMeterPnt const &,
		MicroMeter    const,
		ColorF        const,
		fPixel        const = 1._fPixel
	) const override;

	void DrawLine
	(
		MicroMeterLine const &, 
		MicroMeter     const,
		ColorF         const  
	) const override;

	void FillCircle
	(
		MicroMeterCircle const &,
		ColorF           const,
		fPixel           const = 1._fPixel
	) const override;

	void FillGradientCircle
	(
		MicroMeterCircle const &,
		ColorF           const,  
		ColorF           const  
	) const override;

	void DrawCircle
	(
		MicroMeterCircle const &,
		ColorF           const,
		MicroMeter       const
	) const override;

	void FillEllipse
	(
		MicroMeterEllipse const &,
		ColorF            const  
	) const override;

	void DrawEllipse
	(
		MicroMeterEllipse const &,
		ColorF            const,
		MicroMeter        const   
	) const override;

	void FillArrow
	(
		MicroMeterPnt const &,
		MicroMeterPnt const &,
		MicroMeter    const,
		MicroMeter    const,
		ColorF        const  
	) const override;

	void FillRectangle (MicroMeterRect const &, ColorF) const override; 
	void DrawTranspRect(MicroMeterRect const &, ColorF) const override; 

	void DisplayText
	(
		MicroMeterRect      const &,
		wstring             const &,
		ColorF              const,
		IDWriteTextFormat * const = nullptr
	) const override;

private:
	inline static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

	D2D_driver * m_pGraphics { nullptr };
};