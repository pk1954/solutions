// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Neuron.h"

using namespace std::chrono;

Neuron::Neuron( NNetModel * pModel, MicroMeterPoint const upCenter, tShapeType const type )
  : BaseKnot( pModel, upCenter, type, NEURON_RADIUS ),
	m_timeSinceLastPulse( 0._MicroSecs )
{ 
}

mV Neuron::waveFunction( MicroSecs const time ) const
{
	assert( time >= 0._MicroSecs );
	if ( time <= MicroSecs( m_pNNetModel->GetParameterValue( tParameter::pulseWidth ) ) )
	{
		float x { time.GetValue() };
		float w { m_pNNetModel->GetParameterValue( tParameter::pulseWidth ) };
		float p { m_pNNetModel->GetParameterValue( tParameter::peakVoltage ) };
		float u { 4.0f * p / w };
		float y { u * x * ( 1.0f - x / w ) };
		return mV( y );
	}
	else 
		return BASE_POTENTIAL;
}

void Neuron::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pNNetModel->GetConstTypedShape<Pipeline>( idPipeline )->GetNextOutput( );
}

void Neuron::Step( )
{
	if ( 
		  (m_mVinputBuffer >= mV( m_pNNetModel->GetParameterValue( tParameter::threshold ) )) &&
		  (m_timeSinceLastPulse >= MicroSecs( m_pNNetModel->GetParameterValue( tParameter::pulseWidth ) + m_pNNetModel->GetParameterValue( tParameter::refractoryPeriod )) )
	   )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}
}

mV Neuron::GetNextOutput( ) const
{
	return ( m_timeSinceLastPulse <= MicroSecs( m_pNNetModel->GetParameterValue( tParameter::pulseWidth ) ) )
		   ? waveFunction( m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

MicroMeterPoint Neuron::getAxonHillockPos( PixelCoordsFp & coord ) const
{
	MicroMeterPoint axonHillockPos { NP_NULL };
	if ( m_outgoing.size() > 0 )
	{
		ShapeId          const idAxon       { * m_outgoing.begin() };
		Pipeline const * const pAxon        { m_pNNetModel->GetConstTypedShape<Pipeline>( idAxon ) };
		MicroMeterPoint  const vectorScaled { pAxon->GetVector( ) * ( GetExtension() / pAxon->GetLength( ) ) };
		axonHillockPos = GetPosition( ) + vectorScaled * NEURON_INTERIOR;
	}
	return axonHillockPos;
}

void Neuron::DrawExterior( PixelCoordsFp & coord ) const
{
	MicroMeterPoint axonHillockPos { getAxonHillockPos( coord ) };
	drawExterior( coord, 24 );
	if ( axonHillockPos != NP_NULL )
		drawPolygon( coord, 12, m_pNNetModel->GetFrameColor( * this ), axonHillockPos, GetExtension() * 0.5f );
}

void Neuron::DrawInterior( PixelCoordsFp & coord ) const
{ 
	MicroMeterPoint axonHillockPos { getAxonHillockPos( coord ) };
	drawInterior( coord, 24 );
	if ( axonHillockPos != NP_NULL )
		drawPolygon( coord, 12, GetInteriorColor( ), axonHillockPos, GetExtension() * (NEURON_INTERIOR - 0.5f) );
}

void Neuron::drawExterior( PixelCoordsFp & coord, int const iNrOfEdges ) const
{
	drawPolygon( coord, iNrOfEdges, m_pNNetModel->GetFrameColor( * this ), GetExtension() );
}

void Neuron::drawInterior( PixelCoordsFp & coord, int const iNrOfEdges ) const
{ 
	drawPolygon( coord, iNrOfEdges, GetInteriorColor( ), GetExtension() * NEURON_INTERIOR);
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
