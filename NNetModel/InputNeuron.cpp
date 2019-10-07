// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Knot.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetModel.h"
#include "Pipeline.h"
#include "InputNeuron.h"

using namespace std::chrono;

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
  : Knot( upCenter, tShapeType::inputNeuron ),
	m_timeSinceLastPulse( microseconds( 0 ) ),
	m_pulseFrequency( 0_Hertz )
{ 
	SetPulseFrequency( 50_Hertz );
	m_timeSinceLastPulse = NNetModel::PEAK_TIME;
}

void InputNeuron::Trigger( )
{
	m_timeSinceLastPulse = microseconds( 0 );
}

mV InputNeuron::waveFunction( microseconds time ) const
{
	assert( time >= 0ms );
	if ( time <= NNetModel::PEAK_TIME )
	{
		double x = time.count() / 1000.0 - 1;
		return NNetModel::PEAK_VOLTAGE * ( 1 - x * x );
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
	m_timeSinceLastPulse += TIME_RESOLUTION;

	if ( m_timeSinceLastPulse >= m_pulseDuration )
		Trigger();
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
	return PERCENT( CastToShort((m_timeSinceLastPulse * 100 ) / m_pulseDuration ) );
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
		coord.convert2fPixel( GetExtension() * 0.8 )
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
