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
	m_pulseFrequency( 50_Hertz )
{ 
}

void InputNeuron::Trigger( )
{
	m_timeSinceLastPulse = microseconds( 0 );
}

mV InputNeuron::waveFunction( microseconds time )
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

mV InputNeuron::Step( )
{
	m_timeSinceLastPulse += TIME_RESOLUTION;

	if ( m_timeSinceLastPulse >= PulseDuration( m_pulseFrequency ) )
	{
		Trigger();
	}
	{
		mV mVoutput = BASE_POTENTIAL;
		mV mVWave( waveFunction( m_timeSinceLastPulse ) );
		mVoutput += mVWave;
		return mVoutput;
	}
}

PERCENT InputNeuron::GetFillLevel( ) const
{
	return PERCENT( CastToShort((m_timeSinceLastPulse * 100 ) / PulseDuration( m_pulseFrequency ) ) );
}

void InputNeuron::Draw
( 
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	COLORREF const colorFrame = IsHighlighted( ) ? RGB( 255, 0, 0 )	: RGB( 0, 0, 255 );
	Graphics.AddRect
	( 
		coord.convert2fPixelPos( GetPosition() ), 
		colorFrame, 
		coord.convert2fPixel( GetExtension() )
	);

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
