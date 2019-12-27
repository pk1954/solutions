// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Direct2D.h"
#include "PixelCoordsFp.h"
#include "tHighlightType.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "InputNeuron.h"

using std::chrono::microseconds;
using std::wostringstream;

InputNeuron::InputNeuron( NNetModel * pModel, MicroMeterPoint const upCenter )
  : Neuron( pModel, upCenter, tShapeType::inputNeuron ),
	m_pulseFrequency( STD_PULSE_FREQ ),
	m_pulseDuration( PulseDuration( STD_PULSE_FREQ ) )
{ }

InputNeuron::~InputNeuron( )
{ }

void InputNeuron::SetPulseFrequency( fHertz const freq )
{
	m_pulseFrequency = freq;
	m_pulseDuration  = PulseDuration( m_pulseFrequency );
}

void InputNeuron::Prepare( )
{
	float fillLevel { m_timeSinceLastPulse.GetValue() / m_pulseDuration.GetValue() };
	m_mVinputBuffer = mV( m_pNNetModel->GetParameterValue( tParameter::peakVoltage ) * fillLevel );
}

void InputNeuron::Step( )
{
	if ( m_timeSinceLastPulse >= m_pulseDuration )  
	{
		m_timeSinceLastPulse = 0._MicroSecs;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}
}

void InputNeuron::drawInputNeuron
( 
	PixelCoordsFp const & coord,
	COLORREF      const   color,
	float         const   fReductionFactor
) const
{
	MicroMeterPoint const axonVector
	{
		m_outgoing.empty( )
		? MicroMeterPoint { 0._MicroMeter, 1._MicroMeter } 
	    : m_pNNetModel->GetConstTypedShape<Pipeline>( * m_outgoing.begin() )->GetVector( )
	};
	MicroMeter      const umHypot    { Hypot( axonVector ) };
	MicroMeterPoint const umExtVector{ axonVector * (GetExtension() / umHypot) };
	MicroMeterPoint const umCenter   { GetPosition() };
	MicroMeterPoint const umStartPnt { umCenter + umExtVector * fReductionFactor };
	MicroMeterPoint const umEndPnt   { umCenter - umExtVector };
	fPixelPoint     const fStartPoint{ coord.convert2fPixelPos( umStartPnt ) };
	fPixelPoint     const fEndPoint  { coord.convert2fPixelPos( umEndPnt   ) };
	fPIXEL          const fPixWidth  { coord.convert2fPixel( GetExtension() * fReductionFactor ) };

	m_pGraphics->DrawLine( fStartPoint, fEndPoint, fPixWidth * 2, color );
}

void InputNeuron::DrawExterior( PixelCoordsFp & coord, tHighlightType const type ) const
{
	drawInputNeuron( coord, m_pNNetModel->GetFrameColor( type ), 1.0f );
}

void InputNeuron::DrawInterior( PixelCoordsFp & coord ) const
{ 
	drawInputNeuron( coord, GetInteriorColor( ), NEURON_INTERIOR );
}

void InputNeuron::DrawNeuronText( PixelCoordsFp & coord ) const
{ 

	PixelRect const pixRect { GetPixRect4Text( coord ) };

	wostringstream m_wBuffer;

	m_wBuffer.clear( );
	m_wBuffer.str( std::wstring() );
	m_wBuffer << GetPulseFrequency().GetValue() << L" " << GetParameterUnit( tParameter::pulseRate );

	DisplayText( pixRect, m_wBuffer.str( ) );
}

InputNeuron const * Cast2InputNeuron( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<InputNeuron const *>(shape);
}

InputNeuron * Cast2InputNeuron( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<InputNeuron *>(shape);
}
