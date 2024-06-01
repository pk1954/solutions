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
		Color         const,
		fPixel        const = 1._fPixel
	) const override;

	void DrawLine
	(
		MicroMeterLine const &, 
		MicroMeter     const,
		Color          const  
	) const override;

	void FillCircle
	(
		MicroMeterCircle const &,
		Color            const,
		fPixel           const = 1._fPixel
	) const override;

	void FillGradientCircle
	(
		MicroMeterCircle const &,
		Color            const,  
		Color            const  
	) const override;

	void DrawCircle(MicroMeterCircle const&, ID2D1SolidColorBrush* const, fPixel const) const override;
	void DrawCircle(MicroMeterCircle const&, Color const,                 fPixel const) const override;
	void DrawCircle(MicroMeterCircle const&,                              fPixel const) const override;

	void FillEllipse(MicroMeterEllipse const&, ID2D1SolidColorBrush* const) const override;
	void FillEllipse(MicroMeterEllipse const&, Color const) const override;
	void FillEllipse(MicroMeterEllipse const&) const override;

	void DrawEllipse
	(
		MicroMeterEllipse const &,
		Color             const,
		MicroMeter        const   
	) const override;

	void FillArrow
	(
		MicroMeterPnt const &,
		MicroMeterPnt const &,
		MicroMeter    const,
		MicroMeter    const,
		Color         const  
	) const override;

	void FillRectangle       (MicroMeterRect const&, ID2D1Brush * const)      const override;
	void FillRectangle       (MicroMeterRect const&, Color)                   const override;
	void FillRoundedRectangle(MicroMeterRect const&, Color, MicroMeter const) const override;
	void DrawTranspRect      (MicroMeterRect const&, Color)                   const override;

	void DisplayText
	(
		MicroMeterRect      const &,
		wstring             const &,
		IDWriteTextFormat * const = nullptr
	) const override;

	void DrawBezier
	(
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		ID2D1SolidColorBrush*,
		fPixel        const = 1._fPixel
	) const override;

	void DrawBezier
	(
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		fPixel        const = 1._fPixel
	) const override;

	void DrawBezier
	(
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		MicroMeterPnt const&,
		Color         const,
		fPixel        const = 1._fPixel
	) const override;

	D2D_driver* GetGraphics() {	return m_pGraphics; }

private:
	inline static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

	D2D_driver * m_pGraphics { nullptr };
};