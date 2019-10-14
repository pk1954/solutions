// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "Knot.h"

void Knot::Prepare( NNetModel & model )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incomming )
		m_mVinputBuffer += model.GetPipeline( idPipeline )->GetNextOutput();
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );
}

void Knot::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	fPixelPoint const fPosition { coord.convert2fPixelPos( GetPosition ( ) ) };
	fPIXEL      const fPixWidth { coord.convert2fPixel( IsHighlighted( ) ? 30.0_MicroMeter : GetExtension( ) ) };
	COLORREF    const color     { IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ) };

	Graphics.DrawPolygon( 24, fPosition, color, fPixWidth );
}

void Knot::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	fPixelPoint const fPosition     { coord.convert2fPixelPos( GetPosition( ) ) };
	fPIXEL      const fPixWidth     { coord.convert2fPixel( GetExtension( ) ) * 0.6f };
	mV          const mVperColLevel { PEAK_VOLTAGE / 255.0f };
	int         const iLevel        { CastToInt( m_mVinputBuffer / mVperColLevel ) };
	COLORREF    const color         { RGB( iLevel, 0, 0 ) };

	Graphics.DrawPolygon( 24, fPosition, color, fPixWidth );
}
