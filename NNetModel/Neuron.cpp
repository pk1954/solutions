// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "win32_graphicsInterface.h"
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
	if ( time <= MicroSecs( model.GetParameterValue( tParameter::pulseWidth ) ) )
	{
		float x { time.GetValue() };
		float w { model.GetParameterValue( tParameter::pulseWidth ) };
		float p { model.GetParameterValue( tParameter::peakVoltage ) };
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
		m_mVinputBuffer += model.GetConstTypedShape<Pipeline>( idPipeline )->GetNextOutput( model );
}

void Neuron::Step( NNetModel const & model )
{
	if ( 
		  (m_mVinputBuffer >= mV( model.GetParameterValue( tParameter::threshold ) )) &&
		  (m_timeSinceLastPulse >= MicroSecs( model.GetParameterValue( tParameter::pulseWidth ) + model.GetParameterValue( tParameter::refractoryPeriod )) )
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
	return ( m_timeSinceLastPulse <= MicroSecs( model.GetParameterValue( tParameter::pulseWidth ) ) )
		   ? waveFunction( model, m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

MicroMeterPoint Neuron::getAxonHillockPos( NNetModel const & model, PixelCoordsFp & coord ) const
{
	MicroMeterPoint axonHillockPos { NP_NULL };
	if ( m_outgoing.size() > 0 )
	{
		ShapeId          const idAxon       { * m_outgoing.begin() };
		Pipeline const * const pAxon        { model.GetConstTypedShape<Pipeline>( idAxon ) };
		MicroMeterPoint  const vectorScaled { pAxon->GetVector( model ) * ( GetExtension() / pAxon->GetLength( model ) ) };
		axonHillockPos = GetPosition( ) + vectorScaled * NEURON_INTERIOR;
	}
	return axonHillockPos;
}

void Neuron::DrawExterior( NNetModel const & model, PixelCoordsFp & coord ) const
{
	MicroMeterPoint axonHillockPos { getAxonHillockPos( model, coord ) };
	drawExterior( model, coord, 24 );
	if ( axonHillockPos != NP_NULL )
		drawPolygon( coord, 12, model.GetFrameColor( * this ), axonHillockPos, GetExtension() * 0.5f );
}

void Neuron::DrawInterior( NNetModel const & model, PixelCoordsFp & coord ) const
{ 
	MicroMeterPoint axonHillockPos { getAxonHillockPos( model, coord ) };
	drawInterior( model, coord, 24 );
	if ( axonHillockPos != NP_NULL )
		drawPolygon( coord, 12, GetInteriorColor( model ), axonHillockPos, GetExtension() * (NEURON_INTERIOR - 0.5f) );
}

void Neuron::drawExterior( NNetModel const & model, PixelCoordsFp & coord, int const iNrOfEdges ) const
{
	drawPolygon( coord, iNrOfEdges, model.GetFrameColor( * this ), GetExtension() );
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
