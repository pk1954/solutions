// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include <sstream> 
#include <iomanip>
#include "win32_sound.h"
#include "win32_thread.h"
#include "win32_graphicsInterface.h"
#include "win32_beeperThread.h"
#include "DrawContext.h"
#include "NNetParameters.h"
#include "NNetColors.h"
#include "Neuron.h"

using std::chrono::microseconds;
using std::fixed;
using std::wstring;
using std::wostringstream;

static void CALLBACK BeepFunc
(
	PTP_CALLBACK_INSTANCE instance,
	PVOID                 arg,
	PTP_WORK              pWork
)
{
	Neuron * pNeuron { static_cast<Neuron *>( arg ) };
	Neuron::m_pSound->Beep( pNeuron->GetTriggerSound() );
}

Neuron::Neuron( MicroMeterPoint const upCenter, ShapeType const type )
  : BaseKnot( upCenter, type, NEURON_RADIUS )
{
	Recalc();
}

Neuron::~Neuron( ) { }

SoundDescr const Neuron::SetTriggerSound( SoundDescr const & sound ) 
{
	SoundDescr oldValue { m_triggerSound };
	if ( m_triggerSound.m_bOn != sound.m_bOn )
	{
		if ( sound.m_bOn  )
			m_pTpWork = CreateThreadpoolWork( BeepFunc, this, nullptr );
		else
		{
			CloseThreadpoolWork( m_pTpWork );
			m_pTpWork = nullptr;
		}
	}
	m_triggerSound = sound;
	return oldValue;
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

bool Neuron::CompStep( )
{
	bool bTrigger { (m_mVinputBuffer >= Threshold( )) && (m_timeSinceLastPulse >= PulseWidth() + RefractPeriod()) };

	if ( bTrigger )
	{
		m_timeSinceLastPulse = 0._MicroSecs;
		m_bTriggered = true;
		if ( HasTriggerSound() && m_pTpWork )
			SubmitThreadpoolWork( m_pTpWork );
	}
	else
	{
		m_timeSinceLastPulse += m_pParameters->GetTimeResolution( );
	}

	return m_bStopOnTrigger && bTrigger;
}

mV Neuron::GetNextOutput( ) const
{
	return ( m_timeSinceLastPulse <= PulseWidth() )
		   ? waveFunction( m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

void const Neuron::DisplayText( DrawContext const & context, MicroMeterRect const & umRect, wstring const text ) const
{
	static D2D1::ColorF const colF { 0.0f, 255.0f, 0.0f, 1.0f };

	context.DisplayText( umRect, text, colF );
}

void Neuron::DrawNeuronText( DrawContext const & context ) const
{ 
	wostringstream m_wBuffer;
	m_wBuffer.precision(2);
	m_wBuffer << fixed << setw(6) << GetFillLevel() * 100.0f << L"%";
	DisplayText( context, GetRect4Text( ), m_wBuffer.str( ) );
}

MicroMeterPoint Neuron::getAxonHillockPos( ) const
{
	Pipe            const & axon         { m_connections.GetFirstOutgoingConst() };
	MicroMeterPoint const   vectorScaled { axon.GetVector() * ( GetExtension() / axon.GetLength() ) };
	return GetPosition( ) + vectorScaled * NEURON_INTERIOR;
}

void Neuron::DrawExterior( DrawContext const & context, tHighlightType const type ) const
{
	if ( m_bStopOnTrigger )
	{
		context.DrawCircle( GetPosition(), GetExtension() * 1.4f, GetFrameColor( type ) );
		context.DrawCircle( GetPosition(), GetExtension() * 1.2f, NNetColors::INT_TRIGGER );
	}
	context.DrawCircle( GetPosition(), GetExtension(), GetFrameColor( type ) );
	if ( HasAxon() )
		context.DrawCircle( getAxonHillockPos( ), GetExtension() * 0.5f, GetFrameColor( type ) );
}

void Neuron::DrawInterior( DrawContext const & context ) const
{ 
	D2D1::ColorF const color { m_bTriggered ? NNetColors::INT_TRIGGER : GetInteriorColor( ) };
	context.DrawCircle( GetPosition(), GetExtension() * NEURON_INTERIOR, color );
	if ( HasAxon() )
		context.DrawCircle( getAxonHillockPos( ), GetExtension() * (NEURON_INTERIOR - 0.5f), color );
	m_bTriggered = false;
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
