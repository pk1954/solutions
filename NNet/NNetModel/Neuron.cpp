// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "win32_thread.h"
#include "win32_fatalError.h"
#include "win32_graphicsInterface.h"
#include "win32_beeperThread.h"
#include "PixelCoordsFp.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Neuron.h"

using std::chrono::microseconds;
using std::wostringstream;

BeeperThread * Neuron::m_pBeeperThread { nullptr };

Neuron::Neuron( MicroMeterPoint const upCenter, ShapeType const type )
  : BaseKnot( upCenter, type, NEURON_RADIUS )
{
	Recalc();
	if ( ! m_pBeeperThread )
		m_pBeeperThread = new BeeperThread;
}

fMicroSecs Neuron::PulseWidth() const 
{ 
	return fMicroSecs( m_pNNetModel->GetParameterValue( tParameter::pulseWidth ) ); 
}

fMicroSecs Neuron::RefractPeriod() const 
{ 
	return fMicroSecs( m_pNNetModel->GetParameterValue( tParameter::refractPeriod ) ); 
}

mV Neuron::Threshold() const 
{ 
	return mV( m_pNNetModel->GetParameterValue( tParameter::threshold ) ); 
}

mV Neuron::PeakVoltage() const 
{ 
	return mV( m_pNNetModel->GetParameterValue( tParameter::peakVoltage ) ); 
}

void Neuron::Recalc( ) 
{
	m_factorW = 1.0f / PulseWidth().GetValue();
	m_factorU = 4.0f * m_factorW * PeakVoltage().GetValue();
};

mV Neuron::waveFunction( fMicroSecs const time ) const
{
	return mV( m_factorU * time.GetValue() * ( 1.0f - time.GetValue() * m_factorW ) );
}


void Neuron::Step( )
{
	if ( (m_mVinputBuffer >= Threshold( )) && (m_timeSinceLastPulse >= PulseWidth() + RefractPeriod()) )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;
		if ( HasTriggerSound() )
			m_pBeeperThread->Beep( GetTriggerSoundFrequency(), GetTriggerSoundDuration() );
	}
	else
	{
		m_timeSinceLastPulse += m_pNNetModel->GetTimeResolution( );
	}
}

mV Neuron::GetNextOutput( ) const
{
	return ( m_timeSinceLastPulse <= PulseWidth() )
		   ? waveFunction( m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

MicroMeterPoint Neuron::getAxonHillockPos( PixelCoordsFp & coord ) const
{
	MicroMeterPoint axonHillockPos { NP_NULL };
	if ( HasAxon() )
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

void Neuron::DrawInterior( PixelCoordsFp & coord )
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

Neuron const * Cast2Neuron( Shape const * pShape )
{
	assert( pShape->IsAnyNeuron() );
	return static_cast<Neuron const *>(pShape);
}

Neuron * Cast2Neuron( Shape * pShape )
{
	assert( pShape->IsAnyNeuron() );
	return static_cast<Neuron *>(pShape);
}
