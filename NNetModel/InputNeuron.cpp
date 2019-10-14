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
  : BaseKnot( tShapeType::inputNeuron, upCenter, 50.0_MicroMeter ),
	m_timeSinceLastPulse( 0ms ),
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

mV InputNeuron::waveFunction( microseconds time ) const
{
	assert( time >= 0ms );
	if ( time <= PEAK_TIME )
	{
		float x = CastToFloat(time.count()) / 1000.0f - 1.0f;
		return PEAK_VOLTAGE * ( 1.0f - x * x );
	}
	else 
		return BASE_POTENTIAL;
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

PERCENT InputNeuron::GetFillLevel( ) const
{
	return PERCENT( CastToShort( (m_mVinputBuffer * 100) / PEAK_VOLTAGE ) );
}

void InputNeuron::DrawExterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	COLORREF const colorFrame = IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 );
	Graphics.DrawPolygon
	( 
		24,
		coord.convert2fPixelPos( GetPosition() ), 
		IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 ), 
		coord.convert2fPixel( GetExtension() )
	);
}

void InputNeuron::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{ 
	PERCENT  const fillLevel = GetFillLevel();
	int      const colElem   = ( 255 * fillLevel.GetValue() ) / 100;
	COLORREF const color     = RGB( colElem, 0, 0 );
	Graphics.DrawPolygon
	( 
		24,
		coord.convert2fPixelPos( GetPosition() ), 
		color, 
		coord.convert2fPixel( GetExtension() * 0.8f )
	);
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
