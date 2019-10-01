// Segment.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class Segment
{
public:
	Segment( )
	:	m_npStart( NP_NULL ),
		m_npEnd  ( NP_NULL ),
		m_nmWidth( 0 )
	{}

	Segment
	(  
		MicroMeterPoint const npStart,
		MicroMeterPoint const npEnd,
		MicroMeter const nmWidth
	) :
		m_npStart( npStart ),
		m_npEnd  ( npEnd   ),
		m_nmWidth( nmWidth )
	{}

	MicroMeterPoint GetStartPoint( ) const { return m_npStart; }
	MicroMeterPoint GetEndPoint  ( ) const { return m_npEnd; }
	MicroMeter GetWidth     ( ) const { return m_nmWidth; }

private:
	MicroMeterPoint m_npStart;
	MicroMeterPoint m_npEnd;
	MicroMeter      m_nmWidth;
};
