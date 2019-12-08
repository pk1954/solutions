// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "NNetModel.h"
#include "Knot.h"

void Knot::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pNNetModel->GetConstTypedShape<Pipeline>( idPipeline )->GetNextOutput( );
	CheckInputBuffer( );
}

mV Knot::GetNextOutput( ) const
{
	CheckInputBuffer( );
	return m_mVinputBuffer;
}

void Knot::DrawExterior( PixelCoordsFp & coord ) const
{
	drawPolygon( coord, 24, m_pNNetModel->GetFrameColor( * this ), m_pNNetModel->IsHighlighted( * this ) ? 30.0_MicroMeter : GetExtension( ) );
}

void Knot::DrawInterior( PixelCoordsFp & coord ) const
{
	drawPolygon( coord, 24, GetInteriorColor( ), GetExtension( ) * PIPELINE_INTERIOR );
}
