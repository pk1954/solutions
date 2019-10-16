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
	m_timeSinceLastPulse = PEAK_TIME;
}

void InputNeuron::SetPulseFrequency( Hertz const freq )
{
	m_pulseFrequency = freq;
}

void InputNeuron::Trigger( )
{
	m_mVinputBuffer = PEAK_VOLTAGE;
}

void InputNeuron::Prepare( NNetModel & )
{
	// nothing to prepare
}

void InputNeuron::Step( )
{
	static float        const FACTOR    ( PEAK_VOLTAGE.GetValue() * CastToFloat(TIME_RESOLUTION.count()) );
	static microseconds const DECAY_TIME( PEAK_TIME );
	static mV           const DECAY_INC ( FACTOR / DECAY_TIME.count() );

	mV mVexternalInput( (FACTOR / 1000000.f ) * m_pulseFrequency.GetValue() );

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

void InputNeuron::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	MicroMeterPoint  const umCenter   { GetPosition() };
	ShapeId          const idAxon     { * m_outgoing.begin() };
	Pipeline const * const pAxon      { model.GetConstPipeline( idAxon ) };
	MicroMeterPoint  const umStart    { pAxon->GetStartPoint( model ) };
	MicroMeterPoint  const umEnd      { pAxon->GetEndPoint( model ) };
	MicroMeterPoint  const umVector   { umEnd - umStart };
	MicroMeter       const umHypot    { Hypot( umVector ) };
	MicroMeterPoint  const umExtVector{ umVector * (GetExtension() / umHypot) };
	MicroMeterPoint  const umStartPnt { umCenter + umExtVector };
	MicroMeterPoint  const umEndPnt   { umCenter - umExtVector };

	fPixelPoint const fStartPoint{ coord.convert2fPixelPos( umStartPnt ) };
	fPixelPoint const fEndPoint  { coord.convert2fPixelPos( umEndPnt   ) };
	fPIXEL      const fPixWidth  { coord.convert2fPixel( GetExtension() ) };
	COLORREF    const color      { IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ) };

	Graphics.StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
	Graphics.AddPipelinePoint( fEndPoint, color );
	Graphics.RenderPipeline( );
}

void InputNeuron::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{ 
	MicroMeterPoint  const umCenter   { GetPosition() };
	ShapeId          const idAxon     { * m_outgoing.begin() };
	Pipeline const * const pAxon      { model.GetConstPipeline( idAxon ) };
	MicroMeterPoint  const umStart    { pAxon->GetStartPoint( model ) };
	MicroMeterPoint  const umEnd      { pAxon->GetEndPoint( model ) };
	MicroMeterPoint  const umVector   { umEnd - umStart };
	MicroMeter       const umHypot    { Hypot( umVector ) };
	MicroMeterPoint  const umExtVector{ umVector * (GetExtension() / umHypot) };
	MicroMeterPoint  const umStartPnt { umCenter + umExtVector * 0.8f};
	MicroMeterPoint  const umEndPnt   { umCenter - umExtVector };			         
	fPixelPoint      const fStartPoint{ coord.convert2fPixelPos( umStartPnt ) };
	fPixelPoint      const fEndPoint  { coord.convert2fPixelPos( umEndPnt   ) };
	fPIXEL           const fPixWidth  { coord.convert2fPixel( GetExtension() * 0.8f ) };
	int              const colElem    { ( 255 * GetFillLevel().GetValue() ) / 100 };
	COLORREF         const color      { RGB( colElem, 0, 0 ) };

	Graphics.StartPipeline( fStartPoint, fEndPoint, fPixWidth, color );
	Graphics.AddPipelinePoint( fEndPoint, color );
	Graphics.RenderPipeline( );
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
