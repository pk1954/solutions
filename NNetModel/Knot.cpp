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
		m_mVinputBuffer += model.GetConstTypedShape<Pipeline>( idPipeline )->GetNextOutput( model );
	CheckInputBuffer( model );
}

mV Knot::GetNextOutput( NNetModel const & model ) const
{
	CheckInputBuffer( model );
	return m_mVinputBuffer;
}

void Knot::DrawExterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	drawPolygon( coord, 24, model.GetFrameColor( * this ), model.IsHighlighted( * this ) ? 30.0_MicroMeter : GetExtension( ) );
}

void Knot::DrawInterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	drawPolygon( coord, 24, GetInteriorColor( model ), GetExtension( ) * PIPELINE_INTERIOR );
}
