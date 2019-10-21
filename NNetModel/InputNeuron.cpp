// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "InputNeuron.h"

using namespace std::chrono;

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
  : Neuron( upCenter, tShapeType::inputNeuron ),
	m_pulseFrequency( 0_Hertz )
{ 
}

void InputNeuron::SetPulseFrequency( Hertz const freq )
{
	m_pulseFrequency = freq;
}

void InputNeuron::Trigger( )
{
	m_mVinputBuffer = PEAK_VOLTAGE;
}

void InputNeuron::Prepare( )
{
	// nothing to prepare
}

void InputNeuron::Step( )
{
	static float        const FACTOR    ( PEAK_VOLTAGE.GetValue() * CastToFloat(TIME_RESOLUTION.count()) );
	static microseconds const DECAY_TIME( PEAK_TIME );
	static mV           const DECAY_INC ( FACTOR / DECAY_TIME.count() );

	mV mVexternalInput( (FACTOR / 1000000.f ) * m_pulseFrequency.GetValue() );  // TODO make clearer

	if ( m_mVinputBuffer >= PEAK_VOLTAGE )  
	{
		m_timeSinceLastPulse = 0ms;   
	}
	else
	{
		m_mVinputBuffer      += mVexternalInput;
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}

	if ( m_timeSinceLastPulse < DECAY_TIME )
		m_mVinputBuffer = (m_mVinputBuffer > DECAY_INC) 
		                ? m_mVinputBuffer - DECAY_INC 
		                : 0._mV;
}

mV InputNeuron::GetNextOutput( ) const
{
	mV mVoutput = BASE_POTENTIAL;
	mV mVWave( waveFunction( m_timeSinceLastPulse ) );
	mVoutput += mVWave;
	assert( mVoutput <= PEAK_VOLTAGE );
	return mVoutput;
}

void InputNeuron::drawInputNeuron
( 
	COLORREF const color,
	float    const fReductionFactor
) const
{
	ShapeId          const idAxon     { * m_outgoing.begin() };
	Pipeline const * const pAxon      { m_pModel->GetConstPipeline( idAxon ) };
	MicroMeterPoint  const umStart    { pAxon->GetStartPoint( ) };
	MicroMeterPoint  const umEnd      { pAxon->GetEndPoint( ) };
	MicroMeterPoint  const umVector   { umEnd - umStart };
	MicroMeter       const umHypot    { Hypot( umVector ) };
	MicroMeterPoint  const umExtVector{ umVector * (GetExtension() / umHypot) };
	MicroMeterPoint  const umCenter   { GetPosition() };
	MicroMeterPoint  const umStartPnt { umCenter + umExtVector  * fReductionFactor };
	MicroMeterPoint  const umEndPnt   { umCenter - umExtVector };
	fPixelPoint      const fStartPoint{ m_pCoord->convert2fPixelPos( umStartPnt ) };
	fPixelPoint      const fEndPoint  { m_pCoord->convert2fPixelPos( umEndPnt   ) };
	fPIXEL           const fPixWidth  { m_pCoord->convert2fPixel( GetExtension() * fReductionFactor ) };

	m_pGraphics->StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
	m_pGraphics->AddPipelinePoint( fEndPoint, color );
	m_pGraphics->RenderPipeline( );
}

void InputNeuron::DrawExterior( ) const
{
	COLORREF const color { IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ) };
	drawInputNeuron( color,	1.0f );
}

void InputNeuron::DrawInterior( ) const
{ 
	int      const colElem { CastToInt(GetFillLevel() * 255.0f) };
	COLORREF const color   { RGB( colElem, 0, 0 ) };
	drawInputNeuron( color,	NEURON_INTERIOR );
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
