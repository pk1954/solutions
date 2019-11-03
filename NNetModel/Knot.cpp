// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "NNetModel.h"
#include "Knot.h"

void Knot::Prepare( NNetModel const & model )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += model.GetConstPipeline( idPipeline )->GetNextOutput( model );
	assert( m_mVinputBuffer <= model.GetPeakVoltage() );
}

mV Knot::GetNextOutput( NNetModel const & model ) const
{
	assert( m_mVinputBuffer <= model.GetPeakVoltage() );
	return m_mVinputBuffer;
}

void Knot::DrawExterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	drawPolygon( coord, 24, GetFrameColor( ), IsHighlighted( ) ? 30.0_MicroMeter : GetExtension( ) );
}

void Knot::DrawInterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	drawPolygon( coord, 24, GetInteriorColor( model ), GetExtension( ) * PIPELINE_INTERIOR );
}
