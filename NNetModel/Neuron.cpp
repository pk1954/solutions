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
	m_timeSinceLastPulse( 0._MicroSecs )
{ 
}

mV Neuron::waveFunction
( 
	NNetModel const & model,
	MicroSecs const   time 
) const
{
	assert( time >= 0._MicroSecs );
	if ( time <= MicroSecs( model.GetParameter( tParameter::pulseWidth ) ) )
	{
		float x { time.GetValue() };
		float w { model.GetParameter( tParameter::pulseWidth ) };
		float p { model.GetParameter( tParameter::peakVoltage ) };
		float u { 4.0f * p / w };
		float y { u * x * ( 1.0f - x / w ) };
		return mV( y );
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
		  (m_mVinputBuffer >= mV( model.GetParameter( tParameter::threshold ) )) &&
		  (m_timeSinceLastPulse >= MicroSecs( model.GetParameter( tParameter::pulseWidth ) + model.GetParameter( tParameter::refractoryPeriod )) )
	   )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}
}

mV Neuron::GetNextOutput( NNetModel const & model ) const
{
	return ( m_timeSinceLastPulse <= MicroSecs( model.GetParameter( tParameter::pulseWidth ) ) )
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
