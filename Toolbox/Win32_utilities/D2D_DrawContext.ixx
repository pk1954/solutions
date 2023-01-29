// D2D_DrawContext.ixx
//
// Win32_utilities

module;

#include <string>

export module D2D_DrawContext;

import Direct2D;
import Types;
import DrawContext;

using std::wstring;

export class D2D_DrawContext: public DrawContext
{
public:

	void Start(D2D_driver * const);

	void SetStdFontSize(MicroMeter const &);

	void SetRotation(Degrees const, MicroMeterPnt const&) const override;
	void Reset() const override;

	void DrawLine
	(
		MicroMeterPnt const &, 
		MicroMeterPnt const &,
		MicroMeter    const,
		D2D1::ColorF  const,
		fPixel        const = 1._fPixel
	) const override;

	void DrawLine
	(
		MicroMeterLine const &, 
		MicroMeter     const,
		D2D1::ColorF   const  
	) const override;

	void FillCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const,
		fPixel           const = 1._fPixel
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
		MicroMeter    const,
		MicroMeter    const,
		D2D1::ColorF  const  
	) const override;

	void FillRectangle       (MicroMeterRect const&, D2D1::ColorF)                   const override;
	void FillRoundedRectangle(MicroMeterRect const&, D2D1::ColorF, MicroMeter const) const override;
	void DrawTranspRect      (MicroMeterRect const&, D2D1::ColorF)                   const override;

	void DisplayText
	(
		MicroMeterRect      const &,
		wstring             const &,
		IDWriteTextFormat * const = nullptr
	) const override;

private:
	inline static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

	D2D_driver * m_pGraphics { nullptr };
};