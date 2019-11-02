// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Neuron.h"

using namespace std::chrono;

Neuron::Neuron( MicroMeterPoint const upCenter, tShapeType const type )
  : BaseKnot( upCenter, type, NEURON_RADIUS ),
	m_timeSinceLastPulse( 0ms )
{ 
}

mV Neuron::waveFunction( microseconds time ) const
{
	assert( time >= 0ms );
	if ( time <= NNetModel::GetPulseWidth() )
	{
		float x = CastToFloat(time.count()) / 1000.0f - 1.0f;
		return NNetModel::GetPeakVoltage() * ( 1.0f - x * x );
	}
	else 
		return BASE_POTENTIAL;
}

void Neuron::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pModel->GetPipeline( idPipeline )->GetNextOutput();
}

void Neuron::Step( )
{
	if ( 
		  (m_mVinputBuffer >= NNetModel::GetThresholdPotential()) &&
		  (m_timeSinceLastPulse >= NNetModel::GetPulseWidth() + NNetModel::GetRefractoryPeriod())
	   )  
	{
		m_timeSinceLastPulse = 0ms;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}
}

mV Neuron::GetNextOutput( ) const
{
	return ( m_timeSinceLastPulse <= NNetModel::GetPulseWidth() )
		   ? waveFunction( m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

void Neuron::DrawExterior( ) const
{
	drawExterior( 24 );
}

void Neuron::DrawInterior( ) const
{ 
	drawInterior( 24 );
}

void Neuron::drawExterior( int const iNrOfEdges ) const
{
	drawPolygon( iNrOfEdges, GetFrameColor( ), GetExtension() );
}

void Neuron::drawInterior( int const iNrOfEdges ) const
{ 
	drawPolygon( iNrOfEdges, GetInteriorColor( ), GetExtension()  * NEURON_INTERIOR);
}

Neuron const * Cast2Neuron( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::neuron );
	return static_cast<Neuron const *>(shape);
}

Neuron * Cast2Neuron( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::neuron );
	return static_cast<Neuron *>(shape);
}
