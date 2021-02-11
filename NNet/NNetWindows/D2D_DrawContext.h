// D2D_DrawContext.h
//
// NNetWindows

#pragma once

#include "NNetColors.h"
#include "DrawContext.h"
#include "Direct2D.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "scale.h"

class D2D_DrawContext: public DrawContext
{
public:

	void Start( D2D_driver * const );
	void Stop( );

	bool StartFrame( HDC const hDC ) { return m_pGraphics->StartFrame( hDC );	}
	void EndFrame( ) { m_pGraphics->EndFrame( ); }

	void Resize( int const, int const );
	void SetStdFontSize( MicroMeter const & );

	virtual void DrawLine
	( 
		MicroMeterPoint const &, 
		MicroMeterPoint const &,
		MicroMeter      const  ,
		D2D1::ColorF    const  
	) const;

	virtual void FillCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const    
	) const;

	virtual void FillGradientCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const,  
		D2D1::ColorF     const  
	) const;

	virtual void DrawCircle
	(
		MicroMeterCircle const &,
		D2D1::ColorF     const,
		MicroMeter       const
	) const;

	virtual void FillEllipse
	(
		MicroMeterEllipse const &,
		D2D1::ColorF      const  
	) const;

	virtual void DrawEllipse
	(
		MicroMeterEllipse const &,
		D2D1::ColorF      const,
		MicroMeter        const   
	) const;

	virtual void FillArrow
	( 
		MicroMeterPoint const &,
		MicroMeterPoint const &,
		MicroMeter      const,
		MicroMeter      const,
		D2D1::ColorF    const  
	) const;

	virtual void FillRectangle ( MicroMeterRect const &, D2D1::ColorF ) const; 
	virtual void DrawTranspRect( MicroMeterRect const &, D2D1::ColorF ) const; 

	void SetPixelSize( MicroMeter const ); 

	virtual void DisplayText
	(
		MicroMeterRect      const &,
		std::wstring        const &,
		D2D1::ColorF        const,
		IDWriteTextFormat * const = nullptr
	) const;

private:
	inline static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

	D2D_driver * m_pGraphics;
};