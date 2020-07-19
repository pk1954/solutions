// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include <iomanip>
#include "DrawContext.h"
#include "tHighlightType.h"
#include "ShapeType.h"
#include "NNetParameters.h"
#include "InputNeuron.h"

using std::chrono::microseconds;
using std::wostringstream;
using std::setprecision;
using std::fixed;

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
	: Neuron( upCenter, ShapeType::Value::inputNeuron )
{ 
	SetPulseFrequency( STD_PULSE_FREQ );
	++ m_counter;
}

InputNeuron::~InputNeuron( )
{ 
	-- m_counter;
}

void InputNeuron::Recalc( )
{
	m_mvFactor = mV( m_pParameters->GetParameterValue( tParameter::peakVoltage ) / m_pulseDuration.GetValue() );
}

fHertz const InputNeuron::SetPulseFrequency( fHertz const freq )
{
	fHertz const fOldValue { m_pulseFrequency };
	m_pulseFrequency = freq;
	m_pulseDuration  = PulseDuration( m_pulseFrequency );
	Recalc( );
	return fOldValue;
}

void InputNeuron::drawInputNeuron
( 
	DrawContext   const & context,  
	D2D1::ColorF  const   colF,
	float         const   fReductionFactor
) const
{
	MicroMeterPoint const axonVector
	{
		HasAxon()
		? m_connections.GetFirstOutgoingConst().GetVector( )
	    : MicroMeterPoint { 0._MicroMeter, 1._MicroMeter } 
	};
	MicroMeter      const umHypot    { Hypot( axonVector ) };
	MicroMeterPoint const umExtVector{ axonVector * (GetExtension() / umHypot) };
	MicroMeterPoint const umCenter   { GetPosition() };
	MicroMeterPoint const umStartPnt { umCenter + umExtVector * fReductionFactor };
	MicroMeterPoint const umEndPnt   { umCenter - umExtVector };

	context.DrawLine( umStartPnt, umEndPnt, GetExtension() * fReductionFactor * 2, colF );
}

void InputNeuron::DrawExterior( DrawContext const & context, tHighlightType const type ) const
{
	drawInputNeuron( context, GetFrameColor( type ), 1.0f );
}

void InputNeuron::DrawInterior( DrawContext const & context ) const
{ 
	drawInputNeuron( context, GetInteriorColor( ), NEURON_INTERIOR );
}

void InputNeuron::DrawNeuronText( DrawContext const & context ) const
{ 
	wostringstream m_wBuffer;

	m_wBuffer.clear( );
	m_wBuffer.str( std::wstring() );
	m_wBuffer << fixed << setprecision(2) 
		      << GetPulseFrequency().GetValue() 
		      << L" " 
		      << GetParameterUnit( tParameter::pulseRate );

	DisplayText( context, GetRect4Text(), m_wBuffer.str( ) );
}

InputNeuron const * Cast2InputNeuron( Shape const * pShape )
{
	assert( pShape != nullptr );
	assert( pShape->IsInputNeuron() );
	return static_cast<InputNeuron const *>(pShape);
}

InputNeuron * Cast2InputNeuron( Shape * pShape )
{
	assert( pShape != nullptr );
	assert( pShape->IsInputNeuron() );
	return static_cast<InputNeuron *>(pShape);
}
