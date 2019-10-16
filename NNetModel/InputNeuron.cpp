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
	drawExterior( model, Graphics, coord, 24 );
}

void InputNeuron::DrawInterior
( 
	NNetModel     const & model,
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{ 
	drawInterior( model, Graphics, coord, 24 );
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
