// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NNetModel.h"

NNetModel::NNetModel( )
  : m_timeStamp( microseconds( 0 ) ),
	m_neuron1  ( MicroMeterPoint( 400.0_MicroMeter,  200.0_MicroMeter ) ),
	m_neuron2  ( MicroMeterPoint( 400.0_MicroMeter, 1400.0_MicroMeter ) ),
	m_pipeline( 0.1_meterPerSec ), // STD_IMPULSE_SPEED ),
	m_Shapes( ),
	m_shapeHighlighted( NO_SHAPE )
{
	AddShape( m_neuron1 );
	AddShape( m_neuron2 );
	AddShape( m_pipeline );

	m_neuron1.AddOutgoing( & m_pipeline );
	m_neuron2.AddIncomming( & m_pipeline );
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
	mV pot;
	
	pot = m_neuron1.Step( );
	pot = m_pipeline.Step( pot );
	m_neuron2.Step( pot );

	m_timeStamp += TIME_RESOLUTION;
}

void NNetModel::ResetAll( )
{

}

Shape const * NNetModel::GetShapeUnderPoint( MicroMeterPoint const pnt ) const
{
	if ( m_neuron1.IsPointInShape( pnt ) )
		return & m_neuron1;
	else if ( m_neuron2.IsPointInShape( pnt ) )
		return & m_neuron2;
	else if ( m_pipeline.IsPointInShape( pnt ) )
		return & m_pipeline;
	else
		return nullptr;
}
