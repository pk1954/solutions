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
		m_mVinputBuffer += m_pModel->GetPipeline( idPipeline )->GetNextOutput();
	assert( m_mVinputBuffer <= NNetModel::GetPeakVoltage() );
}

mV Knot::GetNextOutput( ) const
{
	assert( m_mVinputBuffer <= NNetModel::GetPeakVoltage() );
	return m_mVinputBuffer;
}

void Knot::DrawExterior( ) const
{
	drawPolygon( 24, GetFrameColor( ), IsHighlighted( ) ? 30.0_MicroMeter : GetExtension( ) );
}

void Knot::DrawInterior( ) const
{
	drawPolygon( 24, GetInteriorColor( ), GetExtension() * PIPELINE_INTERIOR );
}
