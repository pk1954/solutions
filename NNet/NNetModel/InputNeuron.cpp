// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include <iomanip>
#include "Direct2D.h"
#include "PixelCoordsFp.h"
#include "tHighlightType.h"
#include "ShapeType.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "InputNeuron.h"

using std::chrono::microseconds;
using std::wostringstream;
using std::setprecision;
using std::fixed;

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
  : Neuron( upCenter, ShapeType::Value::inputNeuron ),
	m_mvFactor( 0.0_mV ),
	m_pulseFrequency( STD_PULSE_FREQ ),
	m_pulseDuration( PulseDuration( STD_PULSE_FREQ ) )
{ }

InputNeuron::~InputNeuron( )
{ }

void InputNeuron::Recalc( )
{
	m_mvFactor = mV( m_pNNetModel->GetParameterValue( tParameter::peakVoltage ) / m_pulseDuration.GetValue() );
}

void InputNeuron::SetPulseFrequency( fHertz const freq )
{
	m_pulseFrequency = freq;
	m_pulseDuration  = PulseDuration( m_pulseFrequency );
	Recalc( );
}

void InputNeuron::drawInputNeuron
( 
	PixelCoordsFp const & coord,
	D2D1::ColorF  const   colF,
	float         const   fReductionFactor
) const
{
	MicroMeterPoint const axonVector
	{
		m_outgoing.empty( )
		? MicroMeterPoint { 0._MicroMeter, 1._MicroMeter } 
	    : m_outgoing[0]->GetVector( )
	};
	MicroMeter      const umHypot    { Hypot( axonVector ) };
	MicroMeterPoint const umExtVector{ axonVector * (GetExtension() / umHypot) };
	MicroMeterPoint const umCenter   { GetPosition() };
	MicroMeterPoint const umStartPnt { umCenter + umExtVector * fReductionFactor };
	MicroMeterPoint const umEndPnt   { umCenter - umExtVector };
	fPixelPoint     const fStartPoint{ coord.convert2fPixelPos( umStartPnt ) };
	fPixelPoint     const fEndPoint  { coord.convert2fPixelPos( umEndPnt   ) };
	fPIXEL          const fPixWidth  { coord.convert2fPixel( GetExtension() * fReductionFactor ) };

	m_pGraphics->DrawLine( fStartPoint, fEndPoint, fPixWidth * 2, colF );
}

void InputNeuron::DrawExterior( PixelCoordsFp & coord, tHighlightType const type ) const
{
	drawInputNeuron( coord, GetFrameColor( type ), 1.0f );
}

void InputNeuron::DrawInterior( PixelCoordsFp & coord )
{ 
	drawInputNeuron( coord, GetInteriorColor( ), NEURON_INTERIOR );
}

void InputNeuron::DrawNeuronText( PixelCoordsFp & coord ) const
{ 
	PixelRect const pixRect { GetPixRect4Text( coord ) };

	wostringstream m_wBuffer;

	m_wBuffer.clear( );
	m_wBuffer.str( std::wstring() );
	m_wBuffer << fixed << setprecision(2) 
		      << GetPulseFrequency().GetValue() 
		      << L" " 
		      << GetParameterUnit( tParameter::pulseRate );

	DisplayText( pixRect, m_wBuffer.str( ) );
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
