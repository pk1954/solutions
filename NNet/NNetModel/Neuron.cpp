// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "win32_fatalError.h"
#include "win32_graphicsInterface.h"
#include "PixelCoordsFp.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Neuron.h"

using std::chrono::microseconds;
using std::wostringstream;

Neuron::Neuron( MicroMeterPoint const upCenter, tShapeType const type )
  : BaseKnot( upCenter, type, NEURON_RADIUS ),
	m_timeSinceLastPulse( 0._MicroSecs )
{
	Recalc();
}

void Neuron::Recalc( ) 
{
	m_factorW = 1.0f / m_pNNetModel->GetParameterValue( tParameter::pulseWidth );
	m_factorU = 4.0f * m_factorW * m_pNNetModel->GetParameterValue( tParameter::peakVoltage );
};

mV Neuron::waveFunction( MicroSecs const time ) const
{
	return mV( m_factorU * time.GetValue() * ( 1.0f - time.GetValue() * m_factorW ) );
}

void Neuron::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto pipe : m_incoming )
		m_mVinputBuffer += pipe->GetNextOutput( );
}

void Neuron::Step( )
{
	if ( 
		  (m_mVinputBuffer >= mV( m_pNNetModel->GetParameterValue( tParameter::threshold ) )) &&
		  (m_timeSinceLastPulse >= MicroSecs( m_pNNetModel->GetParameterValue( tParameter::pulseWidth ) + m_pNNetModel->GetParameterValue( tParameter::refractPeriod )) )
	   )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;   
	}
	else
	{
		m_timeSinceLastPulse += m_pNNetModel->GetTimeResolution( );
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
		Pipeline const * const pAxon        { m_outgoing[0] };
		MicroMeterPoint  const vectorScaled { pAxon->GetVector( ) * ( GetExtension() / pAxon->GetLength( ) ) };
		axonHillockPos = GetPosition( ) + vectorScaled * NEURON_INTERIOR;
	}
	return axonHillockPos;
}

void Neuron::DrawExterior( PixelCoordsFp & coord, tHighlightType const type ) const
{
	MicroMeterPoint axonHillockPos { getAxonHillockPos( coord ) };
	drawExterior( coord, type );
	if ( axonHillockPos != NP_NULL )
		drawCircle( coord, GetFrameColor( type ), axonHillockPos, GetExtension() * 0.5f );
}

void Neuron::DrawInterior( PixelCoordsFp & coord ) const
{ 
	MicroMeterPoint axonHillockPos { getAxonHillockPos( coord ) };
	drawInterior( coord );
	if ( axonHillockPos != NP_NULL )
		drawCircle( coord, GetInteriorColor( ), axonHillockPos, GetExtension() * (NEURON_INTERIOR - 0.5f) );
}

void Neuron::drawExterior( PixelCoordsFp & coord, tHighlightType const type ) const
{
	drawCircle( coord, GetFrameColor( type ), GetExtension() );
}

void Neuron::drawInterior( PixelCoordsFp & coord ) const
{ 
	drawCircle( coord, GetInteriorColor( ), GetExtension() * NEURON_INTERIOR);
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
