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

Neuron::Neuron( Neuron const & src ) :  // copy constructor
	BaseKnot( src )
{ 
	init( src );
}

Neuron & Neuron::operator=( Neuron const & rhs )
{
	BaseKnot::operator=( rhs );
	init( rhs );
	return * this;
}

Neuron::~Neuron( ) { }

bool Neuron::operator==( Shape const & rhs ) const
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

fMicroSecs Neuron::PulseWidth() const 
{ 
	return fMicroSecs( m_pParameters->GetParameterValue( ParameterType::Value::pulseWidth ) ); 
}

fMicroSecs Neuron::RefractPeriod() const 
{ 
	return fMicroSecs( m_pParameters->GetParameterValue( ParameterType::Value::refractPeriod ) ); 
}

mV Neuron::Threshold() const 
{ 
	return mV( m_pParameters->GetParameterValue( ParameterType::Value::threshold ) ); 
}

mV Neuron::PeakVoltage() const 
{ 
	return mV( m_pParameters->GetParameterValue( ParameterType::Value::peakVoltage ) ); 
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
	MicroMeterPoint const umPosHalfHeight { 0._MicroMeter, umRect.GetHeight()/2 };
	context.DisplayText( umRect + umPosHalfHeight, text, D2D1::ColorF::GreenYellow );
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
	Pipe            const & axon         { m_connections.GetFirstOutgoing() };
	MicroMeterPoint const   vectorScaled { axon.GetVector() * ( GetExtension() / axon.GetLength() ) };
	return GetPosition( ) + vectorScaled * NEURON_INTERIOR;
}

void Neuron::DrawExterior( DrawContext const & context, tHighlightType const type ) const
{
	if ( HasAxon() )
	{
		if ( m_bStopOnTrigger )
		{
			context.FillCircle( GetCircle() * 1.4f, GetExteriorColor( type ) );
			context.FillCircle( GetCircle() * 1.2f, NNetColors::INT_TRIGGER );
		}
		context.FillCircle( GetCircle(), GetExteriorColor( type ) );
		context.FillCircle( MicroMeterCircle( getAxonHillockPos(), GetExtension() * 0.5f ), GetExteriorColor( type ) );
	}
	else 
		drawPlug( context, 0.8f, 0.8f, GetExteriorColor( type ) );
}

void Neuron::DrawInterior( DrawContext const & context, tHighlightType const type ) const
{ 
	if ( HasAxon() )
	{
		D2D1::ColorF const color { m_bTriggered ? NNetColors::INT_TRIGGER : GetInteriorColor( type ) };
		context.FillCircle( GetCircle() * NEURON_INTERIOR, color );
		context.FillCircle( MicroMeterCircle( getAxonHillockPos(), GetExtension() * (NEURON_INTERIOR - 0.5f) ), color );
	}
	else
		drawPlug( context, 0.4f, 0.6f, GetInteriorColor( type ) );
	m_bTriggered = false;
}

void Neuron::DrawRectExterior
( 
	DrawContext     const & context, 
	tHighlightType  const   type, 
	MicroMeterPoint const   direction
) const
{
	drawPlug( context, 0.8f, 0.8f, GetExteriorColor( type ), & direction );
}

void Neuron::DrawRectInterior
( 
	DrawContext     const & context, 
	tHighlightType  const   type, 
	MicroMeterPoint const   direction
) const
{
	drawPlug( context, 0.4f, 0.6f, GetInteriorColor( type ), & direction );
}

void Neuron::drawPlug
( 
	DrawContext     const & context, 
	float           const   M,       // overall width/height                        
	float           const   VSM,     // vertical offset of start point (all sections)  
	D2D1::ColorF    const   colF,
	MicroMeterPoint const * pumVector
) const
{
	MicroMeterPoint umVector { MicroMeterPoint::ZERO_VAL() };
	if (pumVector) 
		umVector = * pumVector;
	else if	(m_connections.GetNrOfIncomingConnections() > 0)
		m_connections.Apply2AllInPipes( [&](Pipe & pipe) { umVector += pipe.GetVector(); } );
	else
		umVector = MicroMeterPoint(0._MicroMeter, 1._MicroMeter);

	MicroMeterPoint const umExtVector { Normalize(umVector) * GetExtension().GetValue() };
	MicroMeterPoint const umCenter    { GetPosition() };
	float           const W           { M + 1.2f };       // width of left/right section                 
	float           const VSS         { VSM - 0.8f };     // vertical offset of startpoint left/right sections  
	MicroMeterPoint const umP         { umCenter - umExtVector * (W * 0.5f) };

	context.DrawLine( umCenter + umExtVector * VSM, umP, GetExtension() * M, colF );
	context.DrawLine( umCenter + umExtVector * VSS, umP, GetExtension() * W, colF );
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
