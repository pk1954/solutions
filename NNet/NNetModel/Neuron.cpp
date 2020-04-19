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
#include "NNetColors.h"
#include "Neuron.h"

using std::chrono::microseconds;
using std::wostringstream;

static void CALLBACK BeepFunc
(
	PTP_CALLBACK_INSTANCE instance,
	PVOID                 arg,
	PTP_WORK              pWork
)
{
	Neuron * pNeuron { static_cast<Neuron *>( arg ) };
	Sound::Beep( pNeuron->GetTriggerSoundFrequency(), pNeuron->GetTriggerSoundDuration() );
}

Neuron::Neuron( MicroMeterPoint const upCenter, ShapeType const type )
  : BaseKnot( upCenter, type, NEURON_RADIUS )
{
	Recalc();
}

void Neuron::SetTriggerSoundOn( bool const bMode )
{
	if ( m_bTriggerSoundOn != bMode )
	{
		if ( bMode  )
			m_pTpWork = CreateThreadpoolWork( BeepFunc, this, nullptr );
		else 
			CloseThreadpoolWork( m_pTpWork );
		m_bTriggerSoundOn = bMode;
	}
}

fMicroSecs Neuron::PulseWidth() const 
{ 
	return fMicroSecs( m_pParameters->GetParameterValue( tParameter::pulseWidth ) ); 
}

fMicroSecs Neuron::RefractPeriod() const 
{ 
	return fMicroSecs( m_pParameters->GetParameterValue( tParameter::refractPeriod ) ); 
}

mV Neuron::Threshold() const 
{ 
	return mV( m_pParameters->GetParameterValue( tParameter::threshold ) ); 
}

mV Neuron::PeakVoltage() const 
{ 
	return mV( m_pParameters->GetParameterValue( tParameter::peakVoltage ) ); 
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

void Neuron::Clear( )
{
	m_timeSinceLastPulse = 0._MicroSecs;   
	Shape::Clear( );
}

void Neuron::Step( )
{
	if ( (m_mVinputBuffer >= Threshold( )) && (m_timeSinceLastPulse >= PulseWidth() + RefractPeriod()) )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;
		m_bTriggered = true;
		if ( HasTriggerSound() )
			SubmitThreadpoolWork( m_pTpWork );
	}
	else
	{
		m_timeSinceLastPulse += m_pParameters->GetTimeResolution( );
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
		Pipe     const * const pAxon        { m_outgoing[0] };
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
	D2D1::ColorF    color          { m_bTriggered ? NNetColors::INT_TRIGGER : GetInteriorColor( ) };
	MicroMeterPoint axonHillockPos { getAxonHillockPos( coord ) };
	drawCircle( coord, color, GetExtension() * NEURON_INTERIOR);
	if ( axonHillockPos != NP_NULL )
		drawCircle( coord, color, axonHillockPos, GetExtension() * (NEURON_INTERIOR - 0.5f) );
	m_bTriggered = false;
}

void Neuron::drawExterior( PixelCoordsFp & coord, tHighlightType const type ) const
{
	drawCircle( coord, GetFrameColor( type ), GetExtension() );
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
