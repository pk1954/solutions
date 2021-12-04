// D2D_DrawContext.h
//
// NNetWindows

#pragma once

#include "NNetColors.h"
#include "DrawContext.h"
#include "Direct2D.h"
#include "PixelTypes.h"
#include "MoreTypes.h"
#include "NobId.h"

class D2D_DrawContext: public DrawContext
{
public:

	void Start(D2D_driver * const);
	void Stop();

	bool StartFrame() { return m_pGraphics->StartFrame(); }
	void EndFrame  () { m_pGraphics->EndFrame(); }

	void Resize(int const, int const);
	void SetStdFontSize(MicroMeter const &);

	virtual void DrawLine
	(
		MicroMeterPnt const &, 
		MicroMeterPnt const &,
		MicroMeter    const,
		D2D1::ColorF  const,
		fPixel        const = 1._fPixel
	) const;

	void DrawLine
	(
		MicroMeterLine const &, 
		MicroMeter     const,
		D2D1::ColorF   const  
	) const override;

	void FillCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const    
	) const override;

	void FillGradientCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const,  
		D2D1::ColorF     const  
	) const override;

	void DrawCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const,
		MicroMeter       const
	) const override;

	void FillEllipse
	(
		MicroMeterEllipse const &,
		D2D1::ColorF      const  
	) const override;

	void DrawEllipse
	(
		MicroMeterEllipse const &,
		D2D1::ColorF      const,
		MicroMeter        const   
	) const override;

	void FillArrow
	(
		MicroMeterPnt const &,
		MicroMeterPnt const &,
		MicroMeter      const,
		MicroMeter      const,
		D2D1::ColorF    const  
	) const override;

	void FillRectangle (MicroMeterRect const &, D2D1::ColorF) const override; 
	void DrawTranspRect(MicroMeterRect const &, D2D1::ColorF) const override; 

	void DisplayText
	(
		MicroMeterRect      const &,
		std::wstring        const &,
		D2D1::ColorF        const,
		IDWriteTextFormat * const = nullptr
	) const override;

private:
	inline static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

	D2D_driver * m_pGraphics;
};