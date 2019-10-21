// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Neuron.h"

using namespace std::chrono;

Neuron::Neuron( MicroMeterPoint const upCenter, tShapeType const type )
  : BaseKnot( upCenter, type, NEURON_RADIUS ),
	m_timeSinceLastPulse( PEAK_TIME )
{ 
}

mV Neuron::waveFunction( microseconds time ) const
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

void Neuron::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pModel->GetPipeline( idPipeline )->GetNextOutput();
	if (m_mVinputBuffer > PEAK_VOLTAGE)
		m_mVinputBuffer = PEAK_VOLTAGE;
}

void Neuron::Step( )
{
	static float        const FACTOR    ( PEAK_VOLTAGE.GetValue() * CastToFloat(TIME_RESOLUTION.count()) );
	static microseconds const DECAY_TIME( PEAK_TIME );
	static mV           const DECAY_INC ( FACTOR / DECAY_TIME.count() );

	if ( m_mVinputBuffer >= PEAK_VOLTAGE )  
	{
		m_timeSinceLastPulse = 0ms;   
	}
	else
	{
		m_timeSinceLastPulse += TIME_RESOLUTION;
	}

	if ( m_timeSinceLastPulse < DECAY_TIME )
		m_mVinputBuffer = (m_mVinputBuffer > DECAY_INC) 
						? m_mVinputBuffer - DECAY_INC 
						: 0._mV;
}

mV Neuron::GetNextOutput( ) const
{
	mV mVoutput = BASE_POTENTIAL;
	mV mVWave( waveFunction( m_timeSinceLastPulse ) );
	mVoutput += mVWave;
	assert( mVoutput <= PEAK_VOLTAGE );
	return mVoutput;
}

void Neuron::DrawExterior( ) const
{
	drawExterior( 24 );
}

void Neuron::DrawInterior( ) const
{ 
	drawInterior( 24 );
}

void Neuron::drawExterior( int const iNrOfEdges ) const
{
//	COLORREF const colorFrame = IsHighlighted( ) ? RGB( 0, 127, 127 ) : RGB( 0, 127, 255 );
	COLORREF const colorFrame 
	{ 
		IsSuperHighlighted( ) 
		? RGB( 255, 0, 0 ) 
		: IsHighlighted( ) 
    	  ? RGB( 0, 127, 127 ) 
    	  : RGB( 0, 127, 255 ) 
	};
	m_pGraphics->DrawPolygon
	( 
		iNrOfEdges,
		m_pCoord->convert2fPixelPos( GetPosition() ), 
		colorFrame, 
		m_pCoord->convert2fPixel( GetExtension() )
	);
}

void Neuron::drawInterior( int const iNrOfEdges ) const
{ 
	int      const colElem { CastToInt(GetFillLevel() * 255.0f) };
	COLORREF const color   { RGB( colElem, 0, 0 ) };
	m_pGraphics->DrawPolygon
	( 
		iNrOfEdges,
		m_pCoord->convert2fPixelPos( GetPosition() ), 
		color, 
		m_pCoord->convert2fPixel( GetExtension() * NEURON_INTERIOR )
	);
}

Neuron const * Cast2Neuron( Shape const * shape )
{
	assert( shape->GetShapeType() == tShapeType::neuron );
	return static_cast<Neuron const *>(shape);
}

Neuron * Cast2Neuron( Shape * shape )
{
	assert( shape->GetShapeType() == tShapeType::neuron );
	return static_cast<Neuron *>(shape);
}
