// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Knot.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "Pipeline.h"
#include "InputNeuron.h"

using namespace std::chrono;

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
  : BaseKnot( tShapeType::inputNeuron, upCenter, 50.0_MicroMeter ),
	m_timeSinceLastPulse( 0ms ),
	m_pulseFrequency( 0_Hertz )
{ 
	m_timeSinceLastPulse = PEAK_TIME;
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

void InputNeuron::Prepare( )
{
	// nothing to prepare
}

void InputNeuron::Step( )
{
	static float        const FACTOR    ( PEAK_VOLTAGE.GetValue() * CastToFloat(TIME_RESOLUTION.count()) );
	static microseconds const DECAY_TIME( PEAK_TIME );
	static mV           const DECAY_INC ( FACTOR / DECAY_TIME.count() );

//	mV mVexternalInput( FACTOR / CastToFloat( PulseDuration( m_pulseFrequency ) );
	mV mVexternalInput( (FACTOR / 1000000.f ) * m_pulseFrequency.GetValue()  );

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
	return mVoutput;
}

PERCENT InputNeuron::GetFillLevel( ) const
{
	return PERCENT( CastToShort( (m_mVinputBuffer * 100) / PEAK_VOLTAGE ) );
}

void InputNeuron::Draw
( 
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{         ///// draw frame

	COLORREF const colorFrame = IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 0, 255 );
	Graphics.AddRect
	( 
		coord.convert2fPixelPos( GetPosition() ), 
		colorFrame, 
		coord.convert2fPixel( GetExtension() )
	);
	      ///// draw interior

	PERCENT  const fillLevel = GetFillLevel();
	int      const colElem   = ( 255 * fillLevel.GetValue() ) / 100;
	COLORREF const color     = RGB( colElem, 0, 0 );
	Graphics.AddRect
	( 
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

Pipeline const * Cast2Pipeline( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::pipeline );
	return static_cast<Pipeline const *>(shape);
}

Pipeline * Cast2Pipeline( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::pipeline );
	return static_cast<Pipeline *>(shape);
}
