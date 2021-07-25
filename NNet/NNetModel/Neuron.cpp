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
#include "IoNeuron.h"
#include "Pipe.h"
#include "Neuron.h"

using std::chrono::microseconds;
using std::fixed;
using std::wstring;
using std::wostringstream;

Neuron::Neuron( MicroMeterPnt const & upCenter, NobType const type )
  : BaseKnot(upCenter, type, NEURON_RADIUS )
{
	Recalc();
}

void Neuron::Check() const
{
	BaseKnot::Check();
}

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

void Neuron::init( const Neuron & rhs )
{
	m_bStopOnTrigger     = rhs.m_bStopOnTrigger;
	m_timeSinceLastPulse = rhs.m_timeSinceLastPulse;
	m_bTriggered         = rhs.m_bTriggered;
	m_factorW            = rhs.m_factorW;
	m_factorU            = rhs.m_factorU;
	m_triggerSound       = rhs.m_triggerSound;
	m_pTpWork = ( rhs.m_triggerSound.m_bOn  )
		      ? CreateThreadpoolWork( BeepFunc, this, nullptr )
		      : nullptr;
}

Neuron::Neuron( Neuron const & src )  // copy constructor
   : BaseKnot( src )
{ 
	init( src );
}

Neuron & Neuron::operator=( Neuron const & rhs )
{
	BaseKnot::operator=( rhs );
	init( rhs );
	return * this;
}

Neuron::~Neuron() { }

bool Neuron::operator==( Nob const & rhs ) const
{
	Neuron const & neuronRhs { static_cast<Neuron const &>(rhs) };
	return
	( this->BaseKnot::operator== (neuronRhs) )                             &&
	IsCloseToZero(m_factorW - neuronRhs.m_factorW)                         &&
	IsCloseToZero(m_factorU - neuronRhs.m_factorU)                         &&
	( m_triggerSound.m_bOn       == neuronRhs.m_triggerSound.m_bOn )       &&
	( m_triggerSound.m_frequency == neuronRhs.m_triggerSound.m_frequency ) &&
	( m_triggerSound.m_duration  == neuronRhs.m_triggerSound.m_duration );
}

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

fMicroSecs const Neuron::PulseWidth() const 
{ 
	return fMicroSecs( m_pParameters->GetParameterValue( ParamType::Value::pulseWidth ) ); 
}

fMicroSecs const Neuron::RefractPeriod() const 
{ 
	return fMicroSecs( m_pParameters->GetParameterValue( ParamType::Value::refractPeriod ) ); 
}

mV const Neuron::Threshold() const 
{ 
	return mV( m_pParameters->GetParameterValue( ParamType::Value::threshold ) ); 
}

mV const Neuron::PeakVoltage() const 
{ 
	return mV( m_pParameters->GetParameterValue( ParamType::Value::peakVoltage ) ); 
}

void Neuron::Recalc() 
{
	m_factorW = 1.0f / PulseWidth().GetValue();
	m_factorU = 4.0f * m_factorW * PeakVoltage().GetValue();
};

mV Neuron::waveFunction( fMicroSecs const time ) const
{
	return mV( m_factorU * time.GetValue() * ( 1.0f - time.GetValue() * m_factorW ) );
}

void Neuron::Clear()
{
	m_timeSinceLastPulse = 0._MicroSecs;   
	Nob::Clear();
}

bool const Neuron::CompStep()
{
	bool bTrigger { (m_mVinputBuffer >= Threshold()) && (m_timeSinceLastPulse >= PulseWidth() + RefractPeriod()) };

	if ( bTrigger )
	{
		m_timeSinceLastPulse = 0._MicroSecs;
		m_bTriggered = true;
		if ( HasTriggerSound() && m_pTpWork )
			SubmitThreadpoolWork( m_pTpWork );
	}
	else
	{
		m_timeSinceLastPulse += m_pParameters->GetTimeResolution();
	}

	return m_bStopOnTrigger && bTrigger;
}

mV const Neuron::GetNextOutput() const
{
	return ( m_timeSinceLastPulse <= PulseWidth() )
		   ? waveFunction( m_timeSinceLastPulse )
		   : BASE_POTENTIAL;
}

void const Neuron::DisplayText( DrawContext const & context, MicroMeterRect const & umRect, wstring const text ) const
{
//	MicroMeterPnt const umPosHalfHeight { 0._MicroMeter, umRect.GetHeight()/2 };
//	context.DisplayText( umRect + umPosHalfHeight, text, D2D1::ColorF::GreenYellow );
}

void Neuron::DrawNeuronText( DrawContext const & context ) const
{ 
	wostringstream m_wBuffer;
	m_wBuffer.precision(2);
	m_wBuffer << fixed << setw(6) << GetFillLevel() * 100.0f << L"%";
	DisplayText( context, GetRect4Text(), m_wBuffer.str() );
}

MicroMeterPnt Neuron::getAxonHillockPos() const
{
	Pipe          const & axon         { GetFirstOutgoing() };
	MicroMeterPnt const   vectorScaled { axon.GetVector() * (GetExtension() / axon.GetLength()) };
	return GetPos() + vectorScaled * NEURON_INTERIOR;
}

void Neuron::DrawExterior( DrawContext const & context, tHighlight const type ) const
{
	if ( m_bStopOnTrigger )
	{
		context.FillCircle(GetCircle() * 1.4f, GetExteriorColor(type));
		context.FillCircle(GetCircle() * 1.2f, NNetColors::INT_TRIGGER);
	}
	context.FillCircle(GetCircle(), GetExteriorColor(type));
	if ( HasAxon() )
		context.FillCircle(MicroMeterCircle(getAxonHillockPos(), GetExtension() * 0.5f), GetExteriorColor(type));
}

void Neuron::DrawInterior( DrawContext const & context, tHighlight const type ) const
{ 
	D2D1::ColorF const color { m_bTriggered ? NNetColors::INT_TRIGGER : GetInteriorColor(type) };
	context.FillCircle(GetCircle() * NEURON_INTERIOR, color);
	if ( HasAxon() )
		context.FillCircle(MicroMeterCircle(getAxonHillockPos(), GetExtension() * (NEURON_INTERIOR - 0.5f)), color);
	m_bTriggered = false;
}

Neuron const * Cast2Neuron( Nob const * pNob )
{
	assert( pNob->IsAnyNeuron() );
	return static_cast<Neuron const *>(pNob);
}

Neuron * Cast2Neuron( Nob * pNob )
{
	assert( pNob->IsAnyNeuron() );
	return static_cast<Neuron *>(pNob);
}
