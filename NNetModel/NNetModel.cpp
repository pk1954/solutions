// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NNetModel.h"

NNetModel::NNetModel( )
  : m_timeStamp( microseconds( 0 ) ),
	m_neuron  ( MicroMeterPoint( 400.0_MicroMeter,  200.0_MicroMeter ) ),
	m_knot    ( MicroMeterPoint( 400.0_MicroMeter, 1400.0_MicroMeter ) ),
	m_pipeline( 0.1_meterPerSec ), // STD_IMPULSE_SPEED ),
	m_Shapes( ),
	m_shapeHighlighted( NO_SHAPE )
{
	AddShape( m_neuron );
	AddShape( m_knot );
	AddShape( m_pipeline );

	m_neuron.AddOutgoing( & m_pipeline );
	m_knot.AddIncomming( & m_pipeline );
}

NNetModel * NNetModel::CreateModel( )
{
	return new NNetModel( );
}

void NNetModel::DestroyCore( NNetModel * pCore )
{
	delete pCore;
}

void NNetModel::Compute( )
{
	m_neuron.Step( );
	m_pipeline.Step( );

	m_timeStamp += TIME_RESOLUTION;
}

void NNetModel::ResetAll( )
{

}

Shape const * NNetModel::GetShapeUnderPoint( MicroMeterPoint const pnt ) const
{
	if ( m_neuron.IsPointInShape( pnt ) )
		return & m_neuron;
	else if ( m_knot.IsPointInShape( pnt ) )
		return & m_knot;
	else if ( m_pipeline.IsPointInShape( pnt ) )
		return & m_pipeline;
	else
		return nullptr;
}
