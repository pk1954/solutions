// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "Knot.h"

void Knot::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pModel->GetPipeline( idPipeline )->GetNextOutput();
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );
}

void Knot::DrawExterior( ) const
{
	fPixelPoint const fPosition { m_pCoord->convert2fPixelPos( GetPosition ( ) ) };
	fPIXEL      const fPixWidth { m_pCoord->convert2fPixel( IsHighlighted( ) ? 30.0_MicroMeter : GetExtension( ) ) };
	COLORREF    const color     { IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ) };

	m_pGraphics->DrawPolygon( 24, fPosition, color, fPixWidth );
}

void Knot::DrawInterior( ) const
{
	fPixelPoint const fPosition     { m_pCoord->convert2fPixelPos( GetPosition( ) ) };
	fPIXEL      const fPixWidth     { m_pCoord->convert2fPixel( GetExtension( ) ) * PIPELINE_INTERIOR };
	mV          const mVperColLevel { PEAK_VOLTAGE / 255.0f };
	int         const iLevel        { CastToInt( m_mVinputBuffer / mVperColLevel ) };
	COLORREF    const color         { RGB( iLevel, 0, 0 ) };

	m_pGraphics->DrawPolygon( 24, fPosition, color, fPixWidth );
}
