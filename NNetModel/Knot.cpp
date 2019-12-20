// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "NNetModel.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
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
	COLORREF color = m_pNNetModel->GetFrameColor( * this );
	drawCircle( coord, color, m_pNNetModel->IsHighlighted( * this ) ? 30.0_MicroMeter : GetExtension( ) );
}

void Knot::DrawInterior( PixelCoordsFp & coord ) const
{
	drawCircle( coord, GetInteriorColor( ), GetExtension( ) * PIPELINE_INTERIOR );
}
