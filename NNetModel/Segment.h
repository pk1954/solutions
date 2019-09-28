// Segment.h
//
// NNetModel

#pragma once

#include "NNetPoint.h"

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
		NNetPoint const npStart,
		NNetPoint const npEnd,
		MicroMeter const nmWidth
	) :
		m_npStart( npStart ),
		m_npEnd  ( npEnd   ),
		m_nmWidth( nmWidth )
	{}

	NNetPoint GetStartPoint( ) const { return m_npStart; }
	NNetPoint GetEndPoint  ( ) const { return m_npEnd; }
	MicroMeter GetWidth     ( ) const { return m_nmWidth; }

private:
	NNetPoint m_npStart;
	NNetPoint m_npEnd;
	MicroMeter m_nmWidth;
};
