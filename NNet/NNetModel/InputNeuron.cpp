// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include <iomanip>
#include "DrawContext.h"
#include "tHighlightType.h"
#include "ParameterType.h"
#include "ShapeType.h"
#include "NNetColors.h"
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
}

InputNeuron::~InputNeuron() { }

void InputNeuron::CheckShape( ) const
{
	Neuron::CheckShape();
	assert( ! m_connections.HasIncoming() );
}

bool InputNeuron::operator==( Shape const & rhs ) const
{
	InputNeuron const & inputNeuronRhs { static_cast<InputNeuron const &>(rhs) };
	return 
	( this->Neuron::operator== (inputNeuronRhs) )           &&
	( m_mvFactor       == inputNeuronRhs.m_mvFactor )       &&
	( m_pulseFrequency == inputNeuronRhs.m_pulseFrequency ) &&
	( m_pulseDuration  == inputNeuronRhs.m_pulseDuration );
}

void InputNeuron::Recalc( )
{
	m_mvFactor = mV( m_pParameters->GetParameterValue( ParameterType::Value::peakVoltage ) / m_pulseDuration.GetValue() );
}

fHertz const InputNeuron::SetPulseFrequency( fHertz const freq )
{
	fHertz const fOldValue { m_pulseFrequency };
	m_pulseFrequency = freq;
	m_pulseDuration  = PulseDuration( m_pulseFrequency );
	Recalc( );
	return fOldValue;
}

bool InputNeuron::CompStep( )
{
	m_timeSinceLastPulse += m_pParameters->GetTimeResolution( );
	bool bTrigger { m_timeSinceLastPulse >= m_pulseDuration };
	if ( bTrigger )
		m_timeSinceLastPulse = 0._MicroSecs;   
	return m_bStopOnTrigger && bTrigger;
}

void InputNeuron::DrawExterior(DrawContext const & context, tHighlightType const type) const
{
	drawSocket( context, 2.0f, 0.2f, GetExteriorColor(type) );
}

void InputNeuron::DrawInterior(DrawContext const & context, tHighlightType const type) const
{
	drawSocket( context, 1.6f, 0.0f, GetInteriorColor(type) );
}

void InputNeuron::drawSocket
( 
	DrawContext  const & context, 
	float        const   M,       // overall width/height                        
	float        const   VEM,     // vertical offset of end point middle section 
	D2D1::ColorF const   colF
) const
{
	MicroMeterPoint const umExtVector  { DetermineVector(Connections::Type::out) };
	MicroMeterPoint const umCenter     { GetPosition() };

	MicroMeterPoint const umOrthoVector{ umExtVector.OrthoVector( GetExtension() ) * 0.7f };
	MicroMeterPoint const umExtVectorVS{ umExtVector * M * 0.5f };
	MicroMeter      const umWidthLR    { GetExtension() * (M - 1.4f) };  // width of left/right section                 
	MicroMeterPoint const umPosL       { umCenter + umExtVectorVS };
	MicroMeterPoint const umPosR       { umCenter - umExtVectorVS };

	context.DrawLine( umPosL,                 umCenter - umExtVector * VEM, GetExtension() * M, colF );
	context.DrawLine( umPosL + umOrthoVector, umPosR + umOrthoVector,       umWidthLR,          colF );
	context.DrawLine( umPosL - umOrthoVector, umPosR - umOrthoVector,       umWidthLR,          colF );
}

void InputNeuron::DrawNeuronText( DrawContext const & context ) const
{ 
	wostringstream m_wBuffer;

	m_wBuffer.clear( );
	m_wBuffer.str( std::wstring() );
	m_wBuffer << fixed << setprecision(2) 
		      << GetPulseFrequency().GetValue() 
		      << L" " 
		      << ParameterType::GetUnit( ParameterType::Value::pulseRate );

	DisplayText( context, GetRect4Text(), m_wBuffer.str( ) );
}

