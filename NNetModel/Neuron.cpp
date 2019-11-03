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

mV Neuron::waveFunction
( 
	NNetModel    const & model,
	microseconds const   time 
) const
{
	assert( time >= 0ms );
	if ( time <= model.GetPulseWidth() )
	{
		float x = CastToFloat(time.count()) / 1000.0f - 1.0f;
		return model.GetPeakVoltage() * ( 1.0f - x * x );
	}
	else 
		return BASE_POTENTIAL;
}

void Neuron::Prepare( NNetModel const & model )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += model.GetConstPipeline( idPipeline )->GetNextOutput( model );
}

void Neuron::Step( NNetModel const & model )
{
	if ( 
		  (m_mVinputBuffer >= model.GetThresholdPotential()) &&
		  (m_timeSinceLastPulse >= model.GetPulseWidth() + model.GetRefractoryPeriod())
	   )  
	{
		m_timeSinceLastPulse = 0ms;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}
}

mV Neuron::GetNextOutput( NNetModel const & model ) const
{
	return ( m_timeSinceLastPulse <= model.GetPulseWidth() )
		   ? waveFunction( model, m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

void Neuron::DrawExterior( NNetModel const & model, PixelCoordsFp  & coord ) const
{
	drawExterior( coord, 24 );
}

void Neuron::DrawInterior( NNetModel const & model, PixelCoordsFp  & coord ) const
{ 
	drawInterior(model, coord, 24 );
}

void Neuron::drawExterior( PixelCoordsFp & coord, int const iNrOfEdges ) const
{
	drawPolygon( coord, iNrOfEdges, GetFrameColor( ), GetExtension() );
}

void Neuron::drawInterior( NNetModel const & model, PixelCoordsFp & coord, int const iNrOfEdges ) const
{ 
	drawPolygon( coord, iNrOfEdges, GetInteriorColor( model ), GetExtension() * NEURON_INTERIOR);
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
