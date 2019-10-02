// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Knot.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "Pipeline.h"
#include "InputNeuron.h"

InputNeuron::InputNeuron( MicroMeterPoint const upCenter )
	: Knot( upCenter ),
	m_fTriggered( false ),
	m_timeSinceTrigger( microseconds( 0 ) ),
	m_iCounter( 0 ),
	m_pulseFrequency( MilliHertz( 50000 ) )
{ 
	m_iStepsBetweenTrigger = 
	CastToInt
	( 
		( microseconds::period::den * 100 )
		/ ( TIME_RESOLUTION.count() * m_pulseFrequency.GetValue() ) 
	);
}

void InputNeuron::Trigger( )
{
	m_fTriggered = true;
	m_timeSinceTrigger = microseconds( 0 );
}

mV InputNeuron::Step( )
{
	if ( m_iCounter == 0 )
	{
		Trigger();
		m_iCounter = m_iStepsBetweenTrigger;
	}
	else
	{
		--m_iCounter;
	}
	{
		static microseconds const AP_DURATION = microseconds( 4000 );

		mV mVoutput = BASE_POTENTIAL;
		if ( m_fTriggered )
		{
			mVoutput += mV( waveFunction( static_cast<double>(m_timeSinceTrigger.count()) / 1000.0 ) );
			m_timeSinceTrigger += TIME_RESOLUTION;
			if ( m_timeSinceTrigger > AP_DURATION )  
				m_fTriggered = false;
		}
		return mVoutput;
	}
}

PERCENT InputNeuron::GetFillLevel( ) const
{
	return PERCENT( (m_iCounter * 100 ) / m_iStepsBetweenTrigger );
}

void InputNeuron::Draw
( 
	GraphicsInterface   & Graphics,
	PixelCoordsFp const & coord
) const
{
	COLORREF const colorFrame = IsHighlighted( ) ? RGB( 255, 0, 0 )	: RGB( 0, 0, 0 );
	Graphics.AddRect
	( 
		coord.convert2fPixelPos( GetPosition() ), 
		colorFrame, 
		coord.convert2fPixel( GetExtension() )
	);

	PERCENT  const fillLevel = GetFillLevel();
	int      const colElem   = 255 - ( 255 * fillLevel.GetValue() ) / 100;
	COLORREF const color     = RGB( colElem, colElem, colElem );
	Graphics.AddRect
	( 
		coord.convert2fPixelPos( GetPosition() ), 
		color, 
		coord.convert2fPixel( GetExtension() * 0.9 )
	);
}
